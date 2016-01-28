#include <mudlib.h>
#include <sockets.h>
#include <ircd.h>
#include <config.h>
#include <daemons.h>

inherit DAEMON;

#define CR		"\r\n"

object IRCd;
string data_buffer = "";

void start() { }
void stop() { }

private void IRC_read(object socket, string data) {
    int i;
    string * params;

    if (!data) return;

    data = replace_string(data, "\r", "");
    data_buffer += data;

    while ((i=strsrch(data_buffer, "\n")) != -1) {
	data = data_buffer[0..i-1];

	data_buffer = data_buffer[i+1..];

	write(data+"\n");
	write_file("/log/irc", data+"\n");
	
	params = explode(data, " ");
	if (params[0] == "PING") {
	    IRCd -> send("PONG "+params[1]+CR);
	}
	if (params[1] == "376") {
	    /* End of /MOTD */
	    IRCd -> send("JOIN #mundo_oscuro uosux"+CR);
	}
	if (params[1] == "JOIN") {
	    IRCd -> send("PRIVMSG "+params[2][1..]+" :Mundo Oscuro MUD funcionando en IP: 212.170.18.18, Puerto: 4000."+CR);
	    IRCd -> send("PRIVMSG "+params[2][1..]+" :Servidor FTP para Inmortales escuchando puerto 4021."+CR);
	}
	if ((params[1] == "PRIVMSG") && (params[2] == "#mundo_oscuro")) {
	    string nick;
	    nick = explode(params[0],"!")[0];
	    nick = nick[1..];
	    CANALES -> send("irc", nick+" dice: "+implode(params[3..]," ")[1..]);
	}
	if (params[1] == "NOTICE") {
	    switch(params[2]) {
		case "IP_LOOKUP": 
		    /* *** Found your hostname! */
	    	    IRCd -> send("USER mud mud mud :Mundo Oscuro MUD"+CR);
	    	    IRCd -> send("NICK Mundo_Oscuro:BkMNQkBS2wBQ"+CR);
		    break;	
	    }    
	}
    }
}

private void IRC_close(object socket) {

}

void send(string msg) {
    IRCd -> send(msg+CR);
}

void create() {
//    IRCd = new(SOCKET, SOCKET_CONNECT, IRC_ADDRESS, (: IRC_read :), (: IRC_close :)); 
}
