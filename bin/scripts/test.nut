
function onServerInit()
{
	print("onServerInit()");
}

function onPlayerConnect(playerid)
{
	local message = "Connect: [ID: " + playerid + "] [Name: " + getPlayerName(playerid) + "] [IP: " + getPlayerIP(playerid) + "]";
	print(message);
	sendPlayerMessageToAll(1,message); 
}

function onPlayerDisconnect(playerid, reason)
{
	print("Disconnect: [ID: " + playerid + "] [Reason: " + reason + "]");
}

function onPlayerSpawn(playerid)
{
	print("Spawn: [ID: " + playerid + "]");
	setPlayerArmour(playerid, 50.0);
}

function onPlayerText(playerid, text)
{
	print("OnPlayerText(" + playerid + ", " + text + ")");
}

function onPlayerRequestClass(playerid, classid)
{
	print("onPlayerRequestClass(" + playerid + ", " + classid + ")");
}

function onPlayerDeath(playerid, reason)
{
	print("onPlayerDeath(" + playerid + ", " + reason + ")");
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
