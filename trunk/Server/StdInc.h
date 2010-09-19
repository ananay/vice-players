#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string>
#include <time.h>

#include "../RakNet/RakPeer.h"
#include "../RakNet/RPC4Plugin.h"
#include "../RakNet/BitStream.h"

#include "main.h"
#include "CMasterList.h"

#include "system.h"
#include "CConfig.h"

#ifdef WIN32
#include <stdio.h>
#include "../RakNet/WindowsIncludes.h"
#else
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#define Sleep(x) usleep(x * 1000)
#endif

#include "main.h"
#include "CNetGame.h"
#include "CRcon.h"
#include "CScripts.h"
#include "CPlugins.h"

#include "../RakNet/BitStream.h"
#include "../RakNet/RakPeerInterface.h"

#include "../squirrel/sqstdaux.h"
#include "../squirrel/sqstdblob.h"
#include "../squirrel/sqstdio.h"
#include "../squirrel/sqstdmath.h"
#include "../squirrel/sqstdstring.h"
#include "../squirrel/sqstdsystem.h"
#include "../squirrel/sqstring.h"

#include "sq_utils.h"

#include "../Shared/TinyXML/tinyxml.h"
#include "../Shared/TinyXML/ticpp.h"
#include "../Shared/CXML.h"