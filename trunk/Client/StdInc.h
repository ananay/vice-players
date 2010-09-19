#include <stdio.h>
#include <string>
#include "../raknet/WindowsIncludes.h"

#include "main.h"
#include <process.h>
#include "GUI/CGUI.h"

#include "d3d8/include/d3d8.h"
#include "d3d8/include/d3dfont.h"
#include "d3d8/include/d3dx8tex.h"
#include "d3d8/include/DXUtil.h"

#include "game/common.h"
#include "game/CGame.h"
#include "game/CObject.h"
#include "game/CPickup.h"
#include "game/AimSync.h"
#include "game/util.h"
#include "game/CPools.h"
#include "game/KeySync.h"
#include "game/CPhysical.h"
#include "game/CWorld.h"

#include "d3dhook/IDirect3D8Hook.h"
#include "d3dhook/IDirect3DDevice8Hook.h"
#include "d3dhook/IDirectInput8Hook.h"
#include "d3dhook/IDirectInputDevice8Hook.h"
#include "d3dhook/d3d8hook.h"

#include "../raknet/RakPeer.h"
#include "../raknet/RPC4Plugin.h"
#include "../raknet/BitStream.h"
#include "../raknet/MessageIdentifiers.h"

#include "net/CLocalPlayer.h"
#include "net/CRemotePlayer.h"
#include "net/CSpawnSelection.h"
#include "net/netrpc.h"
#include "net/CPlayerPool.h"
#include "net/CVehiclePool.h"
#include "net/CObjectPool.h"
#include "net/CPickupPool.h"
#include "net/CTextPool.h"
#include "net/CCheckpoints.h"
#include "net/CNetGame.h"

#include "scripting/CScripts.h"
#include "scripting/sq_utils.h"

#include "CChatWindow.h"
#include "CCmdWindow.h"
#include "CBBFont.h"
#include "CScoreBoard.h"
#include "CNameTags.h"
#include "CNetStats.h"
#include "anticheat.h"
#include "CScreenshot.h"

#include "game/scripting.h"

#include "../squirrel/sqstdaux.h"
#include "../squirrel/sqstdblob.h"
#include "../squirrel/sqstdio.h"
#include "../squirrel/sqstdmath.h"
#include "../squirrel/sqstdstring.h"
#include "../squirrel/sqstdsystem.h"

#include "../Shared/TinyXML/tinyxml.h"
#include "../Shared/TinyXML/ticpp.h"
#include "../Shared/CXML.h"