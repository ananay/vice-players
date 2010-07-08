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
Release.OBJ = ../raknet/BitStream.o ../raknet/CCRakNetUDT.o ../raknet/CheckSum.o ../raknet/CommandParserInterface.o ../raknet/ConnectionGraph2.o ../raknet/ConsoleServer.o ../raknet/DS_BytePool.o ../raknet/DS_ByteQueue.o ../raknet/DS_HuffmanEncodingTree.o ../raknet/DS_Table.o ../raknet/DataCompressor.o ../raknet/DirectoryDeltaTransfer.o ../raknet/EmailSender.o ../raknet/EncodeClassName.o ../raknet/EpochTimeToString.o ../raknet/FileList.o ../raknet/FileListTransfer.o ../raknet/FileOperations.o ../raknet/FormatString.o ../raknet/FullyConnectedMesh2.o ../raknet/GetTime.o ../raknet/Getche.o ../raknet/GridSectorizer.o ../raknet/HTTPConnection.o ../raknet/IncrementalReadInterface.o ../raknet/Itoa.o ../raknet/LinuxStrings.o ../raknet/LogCommandParser.o ../raknet/MessageFilter.o ../raknet/NatPunchthroughClient.o ../raknet/NatPunchthroughServer.o ../raknet/NatTypeDetectionClient.o ../raknet/NatTypeDetectionCommon.o ../raknet/NatTypeDetectionServer.o ../raknet/NetworkIDManager.o ../raknet/NetworkIDObject.o ../raknet/PacketConsoleLogger.o ../raknet/PacketFileLogger.o ../raknet/PacketLogger.o ../raknet/PacketizedTCP.o ../raknet/PluginInterface2.o ../raknet/RPC4Plugin.o ../raknet/RakMemoryOverride.o ../raknet/RakNetCommandParser.o ../raknet/RakNetSocket.o ../raknet/RakNetStatistics.o ../raknet/RakNetTransport2.o ../raknet/RakNetTypes.o ../raknet/RakPeer.o ../raknet/RakSleep.o ../raknet/RakString.o ../raknet/RakThread.o ../raknet/Rand.o ../raknet/ReadyEvent.o ../raknet/ReliabilityLayer.o ../raknet/ReplicaManager3.o ../raknet/Router2.o ../raknet/SHA1.o ../raknet/SecureHandshake.o ../raknet/SendToThread.o ../raknet/SignaledEvent.o ../raknet/SimpleMutex.o ../raknet/SocketLayer.o ../raknet/StringCompressor.o ../raknet/StringTable.o ../raknet/SuperFastHash.o ../raknet/TCPInterface.o ../raknet/TableSerializer.o ../raknet/TeamBalancer.o ../raknet/TelnetTransport.o ../raknet/ThreadsafePacketLogger.o ../raknet/UDPForwarder.o ../raknet/UDPProxyClient.o ../raknet/UDPProxyCoordinator.o ../raknet/UDPProxyServer.o ../raknet/VariableDeltaSerializer.o ../raknet/VariableListDeltaTracker.o ../raknet/VariadicSQLParser.o ../raknet/WSAStartupSingleton.o ../raknet/_FindFirst.o ../raknet/gettimeofday.o ../raknet/rdlmalloc.o config.o gamemode/generic.o gamemode/genericrpc.o main.o netgame.o netrpc.o player.o playerpool.o rcon.o scripts.o sq_functions.o sq_player_natives.o sq_vehicle_natives.o sq_misc_natives.o squirrel/sqapi.o squirrel/sqbaselib.o squirrel/sqclass.o squirrel/sqcompiler.o squirrel/sqdebug.o squirrel/sqfuncstate.o squirrel/sqlexer.o squirrel/sqmem.o squirrel/sqobject.o squirrel/sqstackhandler.o squirrel/sqstate.o squirrel/sqstdaux.o squirrel/sqstdblob.o squirrel/sqstdio.o squirrel/sqstdmath.o squirrel/sqstdrex.o squirrel/sqstdstream.o squirrel/sqstdstring.o squirrel/sqstdsystem.o squirrel/sqtable.o squirrel/sqvm.o vehicle.o vehiclepool.o timerpool.o sq_timer_natives.o 
DEP_FILES += ../raknet/BitStream.d ../raknet/CCRakNetUDT.d ../raknet/CheckSum.d ../raknet/CommandParserInterface.d ../raknet/ConnectionGraph2.d ../raknet/ConsoleServer.d ../raknet/DS_BytePool.d ../raknet/DS_ByteQueue.d ../raknet/DS_HuffmanEncodingTree.d ../raknet/DS_Table.d ../raknet/DataCompressor.d ../raknet/DirectoryDeltaTransfer.d ../raknet/EmailSender.d ../raknet/EncodeClassName.d ../raknet/EpochTimeToString.d ../raknet/FileList.d ../raknet/FileListTransfer.d ../raknet/FileOperations.d ../raknet/FormatString.d ../raknet/FullyConnectedMesh2.d ../raknet/GetTime.d ../raknet/Getche.d ../raknet/GridSectorizer.d ../raknet/HTTPConnection.d ../raknet/IncrementalReadInterface.d ../raknet/Itoa.d ../raknet/LinuxStrings.d ../raknet/LogCommandParser.d ../raknet/MessageFilter.d ../raknet/NatPunchthroughClient.d ../raknet/NatPunchthroughServer.d ../raknet/NatTypeDetectionClient.d ../raknet/NatTypeDetectionCommon.d ../raknet/NatTypeDetectionServer.d ../raknet/NetworkIDManager.d ../raknet/NetworkIDObject.d ../raknet/PacketConsoleLogger.d ../raknet/PacketFileLogger.d ../raknet/PacketLogger.d ../raknet/PacketizedTCP.d ../raknet/PluginInterface2.d ../raknet/RPC4Plugin.d ../raknet/RakMemoryOverride.d ../raknet/RakNetCommandParser.d ../raknet/RakNetSocket.d ../raknet/RakNetStatistics.d ../raknet/RakNetTransport2.d ../raknet/RakNetTypes.d ../raknet/RakPeer.d ../raknet/RakSleep.d ../raknet/RakString.d ../raknet/RakThread.d ../raknet/Rand.d ../raknet/ReadyEvent.d ../raknet/ReliabilityLayer.d ../raknet/ReplicaManager3.d ../raknet/Router2.d ../raknet/SHA1.d ../raknet/SecureHandshake.d ../raknet/SendToThread.d ../raknet/SignaledEvent.d ../raknet/SimpleMutex.d ../raknet/SocketLayer.d ../raknet/StringCompressor.d ../raknet/StringTable.d ../raknet/SuperFastHash.d ../raknet/TCPInterface.d ../raknet/TableSerializer.d ../raknet/TeamBalancer.d ../raknet/TelnetTransport.d ../raknet/ThreadsafePacketLogger.d ../raknet/UDPForwarder.d ../raknet/UDPProxyClient.d ../raknet/UDPProxyCoordinator.d ../raknet/UDPProxyServer.d ../raknet/VariableDeltaSerializer.d ../raknet/VariableListDeltaTracker.d ../raknet/VariadicSQLParser.d ../raknet/WSAStartupSingleton.d ../raknet/_FindFirst.d ../raknet/gettimeofday.d ../raknet/rdlmalloc.d config.d gamemode/generic.d gamemode/genericrpc.d main.d netgame.d netrpc.d player.d playerpool.d rcon.d scripts.d sq_functions.d sq_player_natives.d sq_vehicle_natives.d sq_misc_natives.d squirrel/sqapi.d squirrel/sqbaselib.d squirrel/sqclass.d squirrel/sqcompiler.d squirrel/sqdebug.d squirrel/sqfuncstate.d squirrel/sqlexer.d squirrel/sqmem.d squirrel/sqobject.d squirrel/sqstackhandler.d squirrel/sqstate.d squirrel/sqstdaux.d squirrel/sqstdblob.d squirrel/sqstdio.d squirrel/sqstdmath.d squirrel/sqstdrex.d squirrel/sqstdstream.d squirrel/sqstdstring.d squirrel/sqstdsystem.d squirrel/sqtable.d squirrel/sqvm.d vehicle.d vehiclepool.d timerpool.d sq_timer_natives.d
clean.OBJ += $(Release.BIN) $(Release.OBJ)

Release : Release.before $(Release.BIN) Release.after_always
Release : CFLAGS += -O2 -Os
Release : INCLUDES += 
Release : RCFLAGS += 
Release : LDFLAGS +=   
Release : T_LDLIBS = -lpthread 
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
