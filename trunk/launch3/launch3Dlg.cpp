// launch3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "launch3.h"
#include "launch3Dlg.h"
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

#include <htmlhelp.h>
#pragma comment(lib,"htmlhelp.lib ")

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

bool GetProcessIdFromProcessName(char * szProcessName, DWORD * dwProcessId)
{
	bool bReturn = false;

	// Create a tool help 32 process snapshot
	HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(!hProcessSnapShot)
		return false;

	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(ProcessEntry);

	if(Process32First(hProcessSnapShot, &ProcessEntry))
	{
		// Enumerate through all processes
		while(Process32Next(hProcessSnapShot, &ProcessEntry))
		{
			// Check the process name to see if it matches szProcessName
			if(!strcmp(ProcessEntry.szExeFile, szProcessName))
			{
				// It matches, set the process id and return true
				*dwProcessId = ProcessEntry.th32ProcessID;
				bReturn = true;
				break;
			}
		}
	}

	// Close the snapshot handle
	CloseHandle(hProcessSnapShot); 

	return bReturn;
}

/////////////////////////////////////////////////////////////////////////////

bool InjectLibraryIntoProcess(DWORD dwProcessId, char * szLibraryPath)
{
	bool bReturn = true;

	// Open our target process
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);

	if(!hProcess) {
		// Failed to open the process
		MessageBoxA(NULL, "Failed to open the remote process.", NULL, NULL);
		return false;
	}

	// Get the length of the library path
	size_t sLibraryPathLen = (strlen(szLibraryPath) + 1);

	// Allocate the a block of memory in our target process for the library name
	void * pRemoteLibraryPath = VirtualAllocEx(hProcess, NULL, sLibraryPathLen, MEM_COMMIT, PAGE_READWRITE);

	// Write our library name to the allocated block of memory
	SIZE_T sBytesWritten = 0;
	WriteProcessMemory(hProcess, pRemoteLibraryPath, (void *)szLibraryPath, sLibraryPathLen, &sBytesWritten);

	if(sBytesWritten != sLibraryPathLen) {
		MessageBoxA(NULL, "Failed to write library path into remote process.", NULL, NULL);
		bReturn = false;
	} else {
		// Get the handle of Kernel32.dll
		HMODULE hKernel32 = GetModuleHandle("Kernel32");

		// Get the address of the LoadLibraryA function from Kernel32.dll
		FARPROC pfnLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

		// Create a thread inside the target process to load our library
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibraryA, pRemoteLibraryPath, 0, NULL);
		
		if(hThread) {
			// Wait for the created thread to end
			WaitForSingleObject(hThread, INFINITE);

			// Get the remote thread exit code
			/*DWORD dwExitCode;
			GetExitCodeThread(hThread, &dwExitCode);
			
			if(dwExitCode != 0)
			{
				IVMessageBox("Failed to inject library into remote process. Cannot launch IV:MP.");
				bReturn = false;
			}*/

			// Close our thread handle
			CloseHandle(hThread);
		} else {
			// Thread creation failed
			MessageBoxA(NULL, "Failed to create remote thread in remote process.", NULL, NULL);
			bReturn = false;
		}
	}

	// Free the allocated block of memory inside the target process
	VirtualFreeEx(hProcess, pRemoteLibraryPath, sizeof(pRemoteLibraryPath), MEM_RELEASE);

	// If the injection failed terminate the target process
	if(!bReturn)
		TerminateProcess(hProcess, 0);

	// Close our target process
	CloseHandle(hProcess);

	return bReturn;
}

/////////////////////////////////////////////////////////////////////////////
// CLaunch3Dlg dialog

CLaunch3Dlg::CLaunch3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaunch3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaunch3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLaunch3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaunch3Dlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK1, m_windowedCheckBox);
}

BEGIN_MESSAGE_MAP(CLaunch3Dlg, CDialog)
	//{{AFX_MSG_MAP(CLaunch3Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_NICK, OnChangeNick)
	ON_BN_CLICKED(IDC_LAUNCH, OnLaunch)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnHelpButton)
	ON_BN_CLICKED(IDC_START_SERVER, OnStartServer)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CString GetAppPath ()
{
TCHAR app_path[_MAX_PATH];

GetModuleFileName((HMODULE)AfxGetApp()->m_hInstance, app_path, MAX_PATH);
CString app_str = app_path;
app_str = app_str.Left(app_str.ReverseFind('\\')+1);

return app_str;
}

/////////////////////////////////////////////////////////////////////////////
// CLaunch3Dlg message handlers

BOOL CLaunch3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	HKEY hKey=0;
	char szBuf[256];

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\VCMP",0,KEY_READ,&hKey);

	if(hKey) {

		DWORD dwSize=256;
		DWORD dwType=REG_SZ;

		RegQueryValueEx(hKey,"nick",0,&dwType,(PUCHAR)szBuf,&dwSize);
		if(dwSize) {
			SetDlgItemText(IDC_NICK,szBuf);
		}

		dwSize=256;
		dwType=REG_SZ;

		RegQueryValueEx(hKey,"server",0,&dwType,(PUCHAR)szBuf,&dwSize);
		if(dwSize) {
			SetDlgItemText(IDC_SERVER_IP,szBuf);
		}
		
		dwSize=256;
		dwType=REG_SZ;

		RegQueryValueEx(hKey,"port",0,&dwType,(PUCHAR)szBuf,&dwSize);
		if(dwSize) {
			SetDlgItemText(IDC_PORT,szBuf);
		}

		RegCloseKey(hKey);
	}
	
	GetDlgItemText(IDC_NICK,szBuf,256);
	if(strlen(szBuf) == 0) {
		SetDlgItemText(IDC_NICK,"Player");
	}

	GetDlgItemText(IDC_SERVER_IP,szBuf,256);
	if(strlen(szBuf) == 0) {
		SetDlgItemText(IDC_SERVER_IP,"127.0.0.1");
	}

	GetDlgItemText(IDC_PORT,szBuf,256);
	if(strlen(szBuf) == 0) {
		SetDlgItemText(IDC_PORT,"5192");
	}

	SetDlgItemText(IDC_SERVER_CONFIG,"vcmp\\vanilla.ini");

	m_windowedCheckBox.SetCheck(0);

	if(strlen((char *)GetCommandLine())) {
		CString csCmdLine((char *)GetCommandLine());
		int vcmp_pos = csCmdLine.Find("vcmp://");

		if(vcmp_pos != (-1)) {
			csCmdLine = csCmdLine.Right(csCmdLine.GetLength() - (vcmp_pos+7));
			
			if(csCmdLine.Right(1) == "/") {
				csCmdLine = csCmdLine.Left(csCmdLine.GetLength() - 1);
			}

			int colon_pos = csCmdLine.Find(':');
			
			CString csHost;
			CString csPort;

			if(colon_pos != (-1)) {
				csHost = csCmdLine.Left(colon_pos);
				csPort = csCmdLine.Right(csCmdLine.GetLength() - (colon_pos+1));
				SetDlgItemText(IDC_SERVER_IP,csHost);
				SetDlgItemText(IDC_PORT,csPort);
			}
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaunch3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CLaunch3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLaunch3Dlg::OnChangeNick() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}

void CLaunch3Dlg::OnLaunch() 
{
	char szParams[1024];
	char szNick[32];
	char szServerIP[128];
	char szPort[32];
	char szPassword[32];

	// Get the nick box text
	GetDlgItemText(IDC_NICK, szNick, sizeof(szNick));
	
	// Get the ip box text
	GetDlgItemText(IDC_SERVER_IP, szServerIP, sizeof(szServerIP));
	
	// Get the port box text
	GetDlgItemText(IDC_PORT, szPort, sizeof(szPort));

	// Get the password box text
	GetDlgItemText(IDC_SERVER_PASSWORD, szPassword, sizeof(szPassword));

	// Try to open the VCMP registry key
	HKEY hKey = 0;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\VCMP",0,KEY_WRITE,&hKey);

	// If the key doesn't exist create it
	if(!hKey) {
		RegCreateKey(HKEY_LOCAL_MACHINE,"Software\\VCMP",&hKey);
	}
	
	if(hKey) {
		// Set the registry key size and type
		DWORD dwSize = sizeof(szNick);
		DWORD dwType = REG_SZ;

		// Save the registry key
		RegSetValueEx(hKey, "nick", 0, dwType, (PUCHAR)szNick, dwSize);

		// Set the registry key size and type
		dwSize = sizeof(szServerIP);
		dwType = REG_SZ;

		// Save the registry key
		RegSetValueEx(hKey, "server", 0, dwType, (PUCHAR)szServerIP, dwSize);

		// Set the registry key size and type
		dwSize = sizeof(szPort);
		dwType = REG_SZ;

		// Save the registry key
		RegSetValueEx(hKey, "port", 0, dwType, (PUCHAR)szPort, dwSize);

		// Close the registry key
		RegCloseKey(hKey);
	}

	// Format the command line
	sprintf(szParams,"\"gta-vc.exe\" -c -h %s -p %s -n %s",szServerIP,szPort,szNick);

	// If we have a password add it to the command line
	if(strlen(szPassword) > 0) {
		strcat(szParams," -z ");
		strcat(szParams,szPassword);
	}

	// If the windowed checkbox is checked add a '-w' to the command line
	if(m_windowedCheckBox.GetCheck()) {
		strcat(szParams, " -w ");
	}

	// Get the library path
	char szLibraryPath[1024];
	sprintf(szLibraryPath, "%svcmp.dll", GetAppPath());

	// Check if vcmp.dll exists
	WIN32_FIND_DATA fdFileInfo;
	if(FindFirstFile(szLibraryPath, &fdFileInfo) == INVALID_HANDLE_VALUE) {
		MessageBox("Couldn't find vcmp.dll.");
		return;
	}

	// Get the gta exe path
	char szGtaExe[1024];
	sprintf(szGtaExe, "%sgta-vc.exe", GetAppPath());

	// Start gta-vc.exe
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);
	if(!CreateProcess(szGtaExe, szParams, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, GetAppPath(), &siStartupInfo, 
		&piProcessInfo)) {
		MessageBox("Couldn't launch gta-vc.exe.\nDid you install vc-mp to your Vice City directory?");
		return;
	}

	// Inject our code into LaunchGTAIV.exe
	if(!InjectLibraryIntoProcess(piProcessInfo.dwProcessId, szLibraryPath)) {
		TerminateProcess(piProcessInfo.hProcess, 0);
		MessageBox("Couldn't inject vcmp.dll.");
		return;
	}

	// Resume the gta-vc.exe thread
	ResumeThread(piProcessInfo.hThread);
}

void CLaunch3Dlg::OnButton2() 
{
	this->OnCancel();	
}


void CLaunch3Dlg::OnHelpButton() 
{
	ShellExecute(m_hWnd, "open","vcmp\\vc-mp.chm", NULL, NULL, SW_SHOWNORMAL);	
	//HtmlHelpA(m_hWnd,"vcmp\\vc-mp.chm",HH_DISPLAY_TOPIC,0);
}

void CLaunch3Dlg::OnStartServer()
{
	char szServerConfig[256];

	GetDlgItemText(IDC_SERVER_CONFIG, szServerConfig, sizeof(szServerConfig));

	ShellExecute(m_hWnd, "open","vcmp-svr.exe", szServerConfig, NULL, SW_SHOWNORMAL);
}

void CLaunch3Dlg::OnButton3() 
{
	char szRConHost[128];
	char szRConPort[32];
	char szRConPass[32];
	char szCated[1024];

	GetDlgItemText(IDC_SERVER_IP_ADMIN, szRConHost, sizeof(szRConHost));
	GetDlgItemText(IDC_SERVER_PORT_ADMIN, szRConPort, sizeof(szRConPort));
	GetDlgItemText(IDC_SERVER_PASS_ADMIN, szRConPass, sizeof(szRConPass));

	sprintf(szCated,"%s %s %s",szRConHost,szRConPort,szRConPass);
	ShellExecute(m_hWnd, "open","rcon.exe", szCated, NULL, SW_SHOWNORMAL);
}
