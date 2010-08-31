function onServerInit()
{
	print("---------------------------\n");
	print("Vice Madness started\n");
	print("---------------------------\n");


	// Player classes
	addPlayerClass(0, 1, -673.2837, -1325.3040, 11.0715, 116.6886, 4, 1, 3, 35, 19, 15);
	addPlayerClass(1, 87, -673.2837, -1325.3040, 11.0715, 116.6886, 9, 1, 22, 70, 21, 13);
	addPlayerClass(2, 93, -673.2837, -1325.3040, 11.0715, 116.6886, 1, 1, 23, 80, 19, 23);
	
	// Some test vehicles
	//local v1 = createVehicle(149, -681.5270, -1329.4580, 10.8290, 292.6041, 5, 5);
	local v2 = createVehicle(150, -683.3698, -1326.5831, 10.8105, 290.5928, 5, 5);
	local v3 = createVehicle(151, -684.9037, -1323.8152, 10.9755, 292.5149, 5, 5);
	local v4 = createVehicle(152, -687.1147, -1321.3459, 11.1368, 292.0442, 5, 5);
	local v5 = createVehicle(162, -674.5535, -1343.4515, 11.2688, 294.7513, 5, 5);
	local v6 = createVehicle(162, -677.5064, -1342.6252, 11.2689, 289.3499, 5, 5);
	local v7 = createVehicle(164, -680.6094, -1333.6694, 10.8321, 290.6213, 5, 5);
	local v8 = createVehicle(164, -679.5953, -1337.7463, 10.8321, 292.4391, 5, 5);

	setScriptAuthor("adamix & jenksta & chris");
	setScriptVersion("0.1");
}

function onServerPulse()
{

}

function onPlayerSync(playerid)
{

}

function onVehicleSync(vehicleid)
{

}

function onPlayerConnect(playerid)
{
	print("Connect: [ID: " + playerid + "] [Name: " + getPlayerName(playerid) + "] [IP: " + getPlayerIP(playerid) + "]");
	sendPlayerMessage(playerid, 0xFFFF00FF, "-> Welcome to Vice Madness");
	sendPlayerMessage(playerid, 0xFFFF00FF, "-> For help type /help");
	sendPlayerMessage(playerid, 0xFFFF00FF, "-> Thanks for playing at Vice Madness!");
	
	sendPlayerMessageToAll(0xE60000FF, "[Join]: "  + getPlayerName(playerid) + " has joined Vice Maddness!.");
	loadClientScript(playerid, "clienttest.nut");
	
}


function onPlayerDisconnect(playerid, reason)
{
	print("Disconnect: [ID: " + playerid + "] [Reason: " + returnQuitReason(reason) + "]");
	sendPlayerMessageToAll(0xE60000FF, "[Join]: "  + getPlayerName(playerid) + " has left Vice Maddness!. Reason: " + returnQuitReason(reason));
}


function onPlayerSpawn(playerid)
{
	print("Spawn: [ID: " + playerid + "]");
	sendPlayerMessageToAll(0xE60000FF, "[Spawn]: " + getPlayerName(playerid) + " has spawned on Vice Maddness!");
	setPlayerTime(playerid, 12, 12);
}


function onPlayerText(playerid, text)
{
	return true;
}

function onPlayerCommand(playerid, cmdtext)
{
	print("onPlayerCommand(" + playerid + ", " + cmdtext + ");");
	local cmd = split(cmdtext, " ");
	if(cmd[0] == "/createcar")
	{
		print("Processing command");
		local pos = getPlayerPosition(playerid);
		print("getPlayerPosition");
		local carid = cmd[1].tointeger();
		print("local carid = cmd[1].tointeger();");
		local car = createVehicle(carid, pos[0], pos[1], pos[2], 0.0, 0, 0);
		print("local car = createVehicle(carid, pos[0], pos[1], pos[2], 0.0, 0, 0);");
		putPlayerInVehicle(playerid, car);
		print("putPlayerInVehicle(playerid, car);");
		setVehicleColor(car, 5, 5);
		print("setVehicleColor(car, 5, 5);");
		
	}
	if(cmd[0] == "/destroycar")
	{
		local car = getPlayerVehicleID(playerid);
		if(car == false) return 0;
		destroyVehicle(car);
	}
		if(cmd[0] == "/setcash")
	{
		setPlayerCash(playerid,1000)
		sendPlayerMessage(playerid, 0x00FF00FF, "Cash set!");
	}
		if(cmd[0] == "/getcash")
	{
		sendPlayerMessage(playerid, 0x00FF00FF, "Cash " + getPlayerCash(playerid));
	}
	if(cmd[0] == "/bleed")
	{
		togglePlayerBleeding(playerid, cmd[1].tointeger());
	}
	if(cmd[0] == "/mycar")
	{
		sendPlayerMessage(playerid, 0x00FF00FF, "Your Vehicle: " + getPlayerVehicleID(playerid));
	}
	if(cmd[0] == "/sky")
	{
		setSkyColor(playerid, cmd[1].tointeger(), cmd[2].tointeger());
	}
	if(cmd[0] == "/classselect")
	{
		forceClassSelection(playerid);
	}
	if(cmd[0] == "/vehicleme") 
	{
		if(isPlayerInVehicle(playerid)) sendPlayerMessage(playerid, 0x00FF00FF, "true");
			else sendPlayerMessage(playerid, 0x00FF00FF, "false");
	}
	if(cmd[0] == "/flashitem")
	{
		setItemFlashing(playerid, cmd[1].tointeger());
		sendPlayerMessage(playerid, 0xFFFF00FF, "Flashing item: " + cmd[1]);
	}
	if(cmd[0] == "/trunk")
	{
		popVehicleTrunk(getPlayerVehicleID(playerid));
	}
	if(cmd[0] == "/mypos")
	{
		local pos = getPlayerPosition(playerid);
		sendPlayerMessage(playerid, 0x00FF00FF, "My Pos: " + pos[0] + " " + pos[1] + " " + pos[2]);
	}
	if(cmd[0] == "/setskin")
	{
		local skin = cmd[1].tointeger();
		setPlayerSkin(playerid, skin);
		sendPlayerMessage(playerid, 0x00FF00FF, "Skin: " + getPlayerSkin(playerid));
	}
}

function onPlayerRequestClass(playerid, classid)
{
	print("onPlayerRequestClass(" + playerid + ", " + classid + ")");
}


function onPlayerDeath(playerid, killerid, reason)
{
	print("onPlayerDeath(" + playerid + ", " + killerid + ", " + reason + ")");
}


function onPlayerEnterVehicle(playerid, vehicle)
{
	print("onPlayerEnterVehicle(" + playerid + ", " + vehicle + ")");
}


function onPlayerExitVehicle(playerid, vehicle)
{
	print("onPlayerExitVehicle(" + playerid + ", " + vehicle + ")");
}


function onRconCommand(command, args)
{
	print("onRconCommand(" + command + ", " + args + ")");
}

function onVehicleCreate(vehicleid)
{
	print("onVehicleCreate(" + vehicleid + ")");
}

function onVehicleDestroy(vehicleid)
{
	print("onVehicleDestroy(" + vehicleid + ")");
}

function onBan(ipmask)
{
	print("onBan(" + ipmask + ")");
}

function onKick(playerid)
{
	print("onKick(" + playerid + ")");
}

function onPlayerDamage(playerid, oldhp, newhp)
{
	print("onPlayerDamage(" + playerid + ", " + oldhp + ", " + newhp + ");");
}

function onVehicleDamage(vehicleid, oldhp, newhp)
{
	print("onVehicleDamage(" + vehicleid + ", " + oldhp + ", " + newhp + ");");
}

function onVehicleSpawn(vehicleid)
{
	print("onVehicleSpawn(" + vehicleid + ")");
}

function onVehicleDeath(vehicleid)
{
	print("onVehicleDeath(" + vehicleid + ")");
}

function onTimerCreate(timerid)
{

}

function onTimerDestroy(timerid)
{

}

function onPlayerKeyEvent(playerid, type, key)
{
	print("onPlayerKeyEvent(" + playerid + ", " + type + ", " + key + ");");
}

function returnQuitReason(reason)
{
	switch(reason)
	{
		case 0:
			return "Crash/Timeout";
		break;
		case 1:
			return "Quit/Left";
		break;
	}
}