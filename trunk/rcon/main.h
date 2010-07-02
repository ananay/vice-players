#pragma once

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <tchar.h>
#include <process.h>

#include "../raknet/RakPeer.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/BitStream.h"
#include "../raknet/MessageIdentifiers.h"

#include "rconclient.h"

extern bool bQuitApp;

void logprintf(char* format, ...);