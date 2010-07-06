function onServerInit()
{
	createVehicle(159, 301.7180, 796.4058, 10.7562, 155.0758, 42, 42);
	createVehicle(154, 296.7950, 787.2752, 10.7558, 151.6805, 42, 42);
	createVehicle(226, 289.9197, 777.5255, 10.7558, 143.8585, 42, 42);
	createVehicle(234, 279.5420, 763.7482, 10.7558, 141.6185, 42, 42);
	createVehicle(235, 272.8984, 756.0524, 10.7558, 136.2074, 42, 42);
	createVehicle(236, 261.3709, 744.7074, 10.7558, 134.1633, 42, 42);
	print("---------------------------\n");
	print("Vice Madness started\n");
	print("---------------------------\n");


	// Player classes
	addPlayerClass(0, 1, -657.076294, 762.287720, 11.599755, 130.724121, 4, 1, 3, 35, 19, 15);
	addPlayerClass(1, 87, 82.608452, 1102.561401, 25.491444, 75.496101, 9, 1, 22, 70, 21, 13);
	addPlayerClass(2, 93, -597.619446, 653.861145, 11.071777, 13.441076, 1, 1, 23, 80, 19, 23);
	
	// Some test vehicles
	createVehicle(175,-675.0778,804.6669,11.1172,179.6899,42,42);
	createVehicle(175,-675.2230,778.0158,11.1102,179.6989,42,42);
	createVehicle(175,-666.7488,772.2281,11.0302,266.1138,42,42);
	createVehicle(175,-668.3047,785.4417,11.1171,359.9105,42,42);
	createVehicle(175,-665.1270,750.6769,10.9391,44.7830,42,42);
}


function onPlayerConnect(playerid)
{
	print("Connect: [ID: " + playerid + "] [Name: " + getPlayerName(playerid) + "] [IP: " + getPlayerIP(playerid) + "]");
	sendPlayerMessage(playerid, 0xE60000FF, "Connected to the server " + getPlayerName(playerid));
}


function onPlayerDisconnect(playerid, reason)
{
	print("Disconnect: [ID: " + playerid + "] [Reason: " + reason + "]");
}


function onPlayerSpawn(playerid)
{
	print("Spawn: [ID: " + playerid + "]");
	sendPlayerMessage(playerid, 0xE60000FF, "Spawned on the server " + getPlayerName(playerid));
	setPlayerTime(playerid, 0, 0);
}


function onPlayerText(playerid, text)
{
	sendMessageAsPlayer(playerid, "[" + playerid + "] " + text);
	return 0;
}

function onPlayerCommand(playerid, cmdtext)
{
	print("onPlayerCommand(" + playerid + ", " + cmdtext + ");");
	local cmd = split(cmdtext, " ");
	if(cmd[0] == "/createcar")
	{
		local pos = getPlayerPos(playerid);
		local carid = cmd[1].tointeger();
		local car = createVehicle(carid, pos[0], pos[1], pos[2], 0.0, 0, 0);
		putPlayerInVehicle(playerid, car);
		setVehicleColor(car, 5, 5);
		
	}
	if(cmd[0] == "/destroycar")
	{
		local car = getPlayerVehicleID(playerid);
		if(car == false) return 0;
		destroyVehicle(car);
		
	}
	if(cmd[0] == "/mypos")
	{
		local pos = getPlayerPos(playerid);
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

function onServerPulse()
{

}