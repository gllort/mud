#include <mudlib.h>
#include <errno.h>

inherit DAEMON;

void start() { /* Skip */ } 
void stop()  { /* Skip */ }

string error_message(string errno) {
    int i, k;
    string msg, * error;

    if ((!errno) || (errno == "")) return "Error inesperado.";   
    error = explode(errno," ");
    switch(error[0]) {
	case ERR_LOADING_DAEMON: msg = "Error inicializando daemon $1."; break;
	case ERR_NO_ACCESS:	 msg = "$1: Acceso denegado."; break;
	case ERR_DB_CONNECT:	 msg = "Error conectando a la Base de Datos."; break;
	case ERR_NO_USER:	 msg = "No existe el jugador $1."; break;
	case ERR_NO_INMORTAL:	 msg = "No existe el inmortal $1."; break;
	case ERR_NO_GROUP:	 msg = "No existe el grupo de trabajo $1."; break;
	case ERR_DUP_RANK:	 msg = "El rango de inmortales $1 ya existe."; break;
	case ERR_DUP_LEVEL: 	 msg = "El nivel de inmortales $1 ya existe."; break;
	case ERR_DUP_GROUP:	 msg = "El grupo de trabajo $1 ya existe."; break;
	case ERR_DUP_MEMBER:	 msg = "El inmortal $1 ya es miembro del grupo $2."; break;
	case ERR_DUP_DOMAIN:	 msg = "El dominio $1 ya existe."; break;
	case ERR_DUP_USER:	 msg = "El usuario $1 ya existe."; break;
	case ERR_DUP_INMORTAL:	 msg = "El usuario $1 ya es un inmortal."; break;
	case ERR_DUP_CHAN:	 msg = "El canal $1 ya existe."; break;
	case ERR_NO_RANK:	 msg = "No existe el rango $1 de inmortales."; break;
	case ERR_NO_MEMBER:	 msg = "$1 no es miebro del grupo $2."; break;
	case ERR_NO_CHAN:	 msg = "No existe el canal $1."; break;
	case ERR_NO_DOMAIN:	 msg = "No existe el dominio $1."; break;
	default: return errno;	
    }	
    k = sizeof(error); 
    for (i=1; i<k; i++) { msg = replace_string(msg, "$"+i, error[i]); }
    return msg;   
}

