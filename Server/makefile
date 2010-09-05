# -*- mode: Makefile; -*-
# -----------------------------------------
# project server

CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

CFLAGS = 
INCLUDES = 
LDFLAGS =  -s
RCFLAGS = 
LDLIBS = $(T_LDLIBS)  -lstdc++

LINK_exe = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_con = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_dll = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS) -shared
LINK_lib = rm -f $@ && ar rcs $@ $^
COMPILE_c = gcc $(CFLAGS_C) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_cpp = g++ $(CFLAGS) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_rc = windres $(RCFLAGS) -J rc -O coff -i $< -o $@ -I$(dir $<)

%.o : %.c ; $(COMPILE_c)
%.o : %.cpp ; $(COMPILE_cpp)
%.o : %.cxx ; $(COMPILE_cpp)
%.o : %.rc ; $(COMPILE_rc)
.SUFFIXES: .o .d .c .cpp .cxx .rc

all: all.before all.targets all.after

all.before :
	-
all.after : $(FIRST_TARGET)
	
all.targets : Release

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)

.PHONY: all clean distclean

# -----------------------------------------
# Release

Release.BIN = ../bin/vcp-svr
Release.OBJ = ../RakNet/BitStream.o ../RakNet/CCRakNetUDT.o ../RakNet/CheckSum.o ../RakNet/CommandParserInterface.o ../RakNet/ConnectionGraph2.o ../RakNet/ConsoleServer.o ../RakNet/DS_BytePool.o ../RakNet/DS_ByteQueue.o ../RakNet/DS_HuffmanEncodingTree.o ../RakNet/DS_Table.o ../RakNet/DataCompressor.o ../RakNet/DirectoryDeltaTransfer.o ../RakNet/EmailSender.o ../RakNet/EncodeClassName.o ../RakNet/EpochTimeToString.o ../RakNet/FileList.o ../RakNet/FileListTransfer.o ../RakNet/FileOperations.o ../RakNet/FormatString.o ../RakNet/FullyConnectedMesh2.o ../RakNet/GetTime.o ../RakNet/Getche.o ../RakNet/GridSectorizer.o ../RakNet/HTTPConnection.o ../RakNet/IncrementalReadInterface.o ../RakNet/Itoa.o ../RakNet/LinuxStrings.o ../RakNet/LogCommandParser.o ../RakNet/MessageFilter.o ../RakNet/NatPunchthroughClient.o ../RakNet/NatPunchthroughServer.o ../RakNet/NatTypeDetectionClient.o ../RakNet/NatTypeDetectionCommon.o ../RakNet/NatTypeDetectionServer.o ../RakNet/NetworkIDManager.o ../RakNet/NetworkIDObject.o ../RakNet/PacketConsoleLogger.o ../RakNet/PacketFileLogger.o ../RakNet/PacketLogger.o ../RakNet/PacketizedTCP.o ../RakNet/PluginInterface2.o ../RakNet/RPC4Plugin.o ../RakNet/RakMemoryOverride.o ../RakNet/RakNetCommandParser.o ../RakNet/RakNetSocket.o ../RakNet/RakNetStatistics.o ../RakNet/RakNetTransport2.o ../RakNet/RakNetTypes.o ../RakNet/RakPeer.o ../RakNet/RakSleep.o ../RakNet/RakString.o ../RakNet/RakThread.o ../RakNet/Rand.o ../RakNet/ReadyEvent.o ../RakNet/ReliabilityLayer.o ../RakNet/ReplicaManager3.o ../RakNet/Router2.o ../RakNet/SHA1.o ../RakNet/SecureHandshake.o ../RakNet/SendToThread.o ../RakNet/SignaledEvent.o ../RakNet/SimpleMutex.o ../RakNet/SocketLayer.o ../RakNet/StringCompressor.o ../RakNet/StringTable.o ../RakNet/SuperFastHash.o ../RakNet/TCPInterface.o ../RakNet/TableSerializer.o ../RakNet/TeamBalancer.o ../RakNet/TelnetTransport.o ../RakNet/ThreadsafePacketLogger.o ../RakNet/UDPForwarder.o ../RakNet/UDPProxyClient.o ../RakNet/UDPProxyCoordinator.o ../RakNet/UDPProxyServer.o ../RakNet/VariableDeltaSerializer.o ../RakNet/VariableListDeltaTracker.o ../RakNet/VariadicSQLParser.o ../RakNet/WSAStartupSingleton.o ../RakNet/_FindFirst.o ../RakNet/gettimeofday.o ../RakNet/rdlmalloc.o config.o spawnselection.o main.o netgame.o netrpc.o player.o playerpool.o rcon.o scripts.o sq_functions.o sq_player_natives.o sq_vehicle_natives.o sq_misc_natives.o ../squirrel/sqapi.o ../squirrel/sqbaselib.o ../squirrel/sqclass.o ../squirrel/sqcompiler.o ../squirrel/sqdebug.o ../squirrel/sqfuncstate.o ../squirrel/sqlexer.o ../squirrel/sqmem.o ../squirrel/sqobject.o ../squirrel/sqstackhandler.o ../squirrel/sqstate.o ../squirrel/sqstdaux.o ../squirrel/sqstdblob.o ../squirrel/sqstdio.o ../squirrel/sqstdmath.o ../squirrel/sqstdrex.o ../squirrel/sqstdstream.o ../squirrel/sqstdstring.o ../squirrel/sqstdsystem.o ../squirrel/sqtable.o ../squirrel/sqvm.o vehicle.o vehiclepool.o timerpool.o sq_timer_natives.o script.o sq_script_natives.o plugins.o plugin.o objectpool.o object.o sq_utils.o
DEP_FILES += ../RakNet/BitStream.d ../RakNet/CCRakNetUDT.d ../RakNet/CheckSum.d ../RakNet/CommandParserInterface.d ../RakNet/ConnectionGraph2.d ../RakNet/ConsoleServer.d ../RakNet/DS_BytePool.d ../RakNet/DS_ByteQueue.d ../RakNet/DS_HuffmanEncodingTree.d ../RakNet/DS_Table.d ../RakNet/DataCompressor.d ../RakNet/DirectoryDeltaTransfer.d ../RakNet/EmailSender.d ../RakNet/EncodeClassName.d ../RakNet/EpochTimeToString.d ../RakNet/FileList.d ../RakNet/FileListTransfer.d ../RakNet/FileOperations.d ../RakNet/FormatString.d ../RakNet/FullyConnectedMesh2.d ../RakNet/GetTime.d ../RakNet/Getche.d ../RakNet/GridSectorizer.d ../RakNet/HTTPConnection.d ../RakNet/IncrementalReadInterface.d ../RakNet/Itoa.d ../RakNet/LinuxStrings.d ../RakNet/LogCommandParser.d ../RakNet/MessageFilter.d ../RakNet/NatPunchthroughClient.d ../RakNet/NatPunchthroughServer.d ../RakNet/NatTypeDetectionClient.d ../RakNet/NatTypeDetectionCommon.d ../RakNet/NatTypeDetectionServer.d ../RakNet/NetworkIDManager.d ../RakNet/NetworkIDObject.d ../RakNet/PacketConsoleLogger.d ../RakNet/PacketFileLogger.d ../RakNet/PacketLogger.d ../RakNet/PacketizedTCP.d ../RakNet/PluginInterface2.d ../RakNet/RPC4Plugin.d ../RakNet/RakMemoryOverride.d ../RakNet/RakNetCommandParser.d ../RakNet/RakNetSocket.d ../RakNet/RakNetStatistics.d ../RakNet/RakNetTransport2.d ../RakNet/RakNetTypes.d ../RakNet/RakPeer.d ../RakNet/RakSleep.d ../RakNet/RakString.d ../RakNet/RakThread.d ../RakNet/Rand.d ../RakNet/ReadyEvent.d ../RakNet/ReliabilityLayer.d ../RakNet/ReplicaManager3.d ../RakNet/Router2.d ../RakNet/SHA1.d ../RakNet/SecureHandshake.d ../RakNet/SendToThread.d ../RakNet/SignaledEvent.d ../RakNet/SimpleMutex.d ../RakNet/SocketLayer.d ../RakNet/StringCompressor.d ../RakNet/StringTable.d ../RakNet/SuperFastHash.d ../RakNet/TCPInterface.d ../RakNet/TableSerializer.d ../RakNet/TeamBalancer.d ../RakNet/TelnetTransport.d ../RakNet/ThreadsafePacketLogger.d ../RakNet/UDPForwarder.d ../RakNet/UDPProxyClient.d ../RakNet/UDPProxyCoordinator.d ../RakNet/UDPProxyServer.d ../RakNet/VariableDeltaSerializer.d ../RakNet/VariableListDeltaTracker.d ../RakNet/VariadicSQLParser.d ../RakNet/WSAStartupSingleton.d ../RakNet/_FindFirst.d ../RakNet/gettimeofday.d ../RakNet/rdlmalloc.d config.d spawnselection.d main.d netgame.d netrpc.d player.d playerpool.d rcon.d scripts.d sq_functions.d sq_player_natives.d sq_vehicle_natives.d sq_misc_natives.d ../squirrel/sqapi.d ../squirrel/sqbaselib.d ../squirrel/sqclass.d ../squirrel/sqcompiler.d ../squirrel/sqdebug.d ../squirrel/sqfuncstate.d ../squirrel/sqlexer.d ../squirrel/sqmem.d ../squirrel/sqobject.d ../squirrel/sqstackhandler.d ../squirrel/sqstate.d ../squirrel/sqstdaux.d ../squirrel/sqstdblob.d ../squirrel/sqstdio.d ../squirrel/sqstdmath.d ../squirrel/sqstdrex.d ../squirrel/sqstdstream.d ../squirrel/sqstdstring.d ../squirrel/sqstdsystem.d ../squirrel/sqtable.d ../squirrel/sqvm.d vehicle.d vehiclepool.d timerpool.d sq_timer_natives.d script.d sq_script_natives.d plugins.d plugin.d objectpool.d object.d sq_utils.d
clean.OBJ += $(Release.BIN) $(Release.OBJ)

Release : Release.before $(Release.BIN) Release.after_always
Release : CFLAGS += -O2 -Os -g
Release : INCLUDES += 
Release : RCFLAGS += 
Release : LDFLAGS +=   
Release : T_LDLIBS = -lpthread -ldl 
ifdef LMAKE
Release : CFLAGS -= -O1 -O2 -g -pipe
endif

Release.before :
	
	
Release.after_always : $(Release.BIN)
	
$(Release.BIN) : $(Release.OBJ)
	$(LINK_con)
	

# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif
