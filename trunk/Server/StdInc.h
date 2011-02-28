// Standard
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string>
#include <time.h>

#ifndef WIN32
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#endif

// RakNet
#include "../RakNet/RakPeer.h"
#include "../RakNet/RPC4Plugin.h"
#include "../RakNet/BitStream.h"
#include "../RakNet/RakPeerInterface.h"
#include "../RakNet/WindowsIncludes.h"

// TinyXML
#include "../Shared/TinyXML/tinyxml.h"
#include "../Shared/TinyXML/ticpp.h"

// VCP
#include "main.h"
#include "system.h"
#include "CConfig.h"
#include "CNetworkManager.h"
#include "CRcon.h"
#include "CScripts.h"
#include "CPlugins.h"
#include "CMasterList.h"
#include "sq_utils.h"

// Squirrel
#include "../squirrel/sqstdaux.h"
#include "../squirrel/sqstdblob.h"
#include "../squirrel/sqstdio.h"
#include "../squirrel/sqstdmath.h"
#include "../squirrel/sqstdstring.h"
#include "../squirrel/sqstdsystem.h"
#include "../squirrel/sqstring.h"

// Shared
#include "../Shared/CXML.h"