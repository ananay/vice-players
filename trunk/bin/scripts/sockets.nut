function onServerInit()
{
	print("Starting creating sockets...");
	sockOpen("irc.gtanet.com", 6667);
}

function onSockClosed(socketid)
{
	print("onSockClosed(" + socketid + ");");
}

function onSockData(socketid, data)
{
	if(data.len() > 100) return;
	print("onSockData(" + socketid + ", " + data + ");");
	sockWrite(socketid, "PONG :REPLY\r\n");
}

function onSockOpened(socketid)
{
	print("onSockOpened(" + socketid + ");");
	sockWrite(socketid, "USER vcptest Bot * :SocketBot\r\n");
	sockWrite(socketid, "NICK vcptest\r\n");
	sockWrite(socketid, "JOIN :#vice-players\r\n");
}