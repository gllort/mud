#include <mudlib.h>
#include <sockets.h>
#include <config.h>

#define SMTP_SERVER "127.0.0.1 25"

inherit DAEMON;

/* Configurar el SMTP server ! */

private nomask void SMTP_read() { ; }
private nomask void SMTP_close(object socket) { ; }

nomask void send_mail(string destino, string asunto, string msg) {
    object smtp;
/* 
  reply_to=unguarded(1,
		     (: call_other, 
		        USER_D,
		        "query_variable",
		        this_user()->query_userid(),
		      ({"email"}) :) )[0];
*/
    smtp = new(SOCKET, SOCKET_CONNECT, SMTP_SERVER, (: SMTP_read :), (: SMTP_close :));
    catch {
	smtp->send("HELO "+query_host_name()+"\n");
        smtp->send("MAIL FROM: <"MUD_NAME" MUD>\n");
        smtp->send("RCPT TO: " + destino + "\n");
        smtp->send("DATA\n");
        smtp->send("From: \"" + MUD_NAME + "\" <" + ADMIN_EMAIL + ">\n");
        smtp->send("To: <" + destino + ">\n");
/*
        if(reply_to!=""||!reply_to)
	    smtp->send(sprintf("Reply-To: <%s> (%s@%s)\n",
			     reply_to,
			     capitalize(this_user()->query_userid()),
			     MUD_NAME) );
*/
        smtp->send("Subject: " + asunto + "\n\n");
        smtp->send(msg + "\n");
        smtp->send(".\n");
        smtp->send("QUIT\n");
        destruct(smtp);
    };
}
