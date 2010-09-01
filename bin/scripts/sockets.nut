local socket = -1;

function onServerInit()
{
        print("Starting creating sockets...");
        socket = sockOpen("irc.gtanet.com", 6667);
}

function onSockClosed(socketid)
{
        print("onSockClosed(" + socketid + ");");
}

function onSockData(socketid, data)
{
        if(socketid != socket) return;
        if(data.len() < 100) {
                print("onSockData(" + socketid + ", " + data + ");");
        }
        local rex = regexp("^PING");
        if(rex.search(data))
        {
                sockWrite(socketid, "PONG :REPLY\r\n");
        }
}

function onSockOpened(socketid)
{
        if(socketid != socket) return;
        print("onSockOpened(" + socketid + ");");
        sockWrite(socketid, "USER vcptest Bot * :SocketBot\r\n");
        sockWrite(socketid, "NICK vcptest\r\n");
        sockWrite(socketid, "JOIN :#vice-players\r\n");
}

function onPlayerText(playerid, text)
{
        if(socket == -1) return;
        local name = getPlayerName(playerid);
        local send = format("[%s][%d]: %s", name, playerid, text);
        sockWrite(socket, "PRIVMSG #vice-players :" + send + "\r\n");
}


function onPlayerConnect(playerid)
{
        if(socket == -1) return;
        local name = getPlayerName(playerid);
        local send = format("[%s] joined to the server.", name);
        sockWrite(socket, "PRIVMSG #vice-players :" + send + "\r\n");
}

