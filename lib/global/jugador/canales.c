#include <canales.h>
#include <daemons.h>
#include <errno.h>

private mapping canales = ([ ]);
/* keys: canales
 * values: ({ chanID, Open/Close?, Baneado? }) 
 */
 
int add_channel(string chan) {
    int cid;

    if (!chan || chan == "") return 0;
    chan = lower_case(chan);
    if ((cid = CANALES -> cid?(chan)) == -1) {
	write(serror(E1(ERR_NO_CHAN, chan))+"\n");
	return 0;
    }
    canales[chan] = ({ cid, OPEN, 0 });
    return CANALES -> join(chan);
}

int del_channel(string chan) {
    chan = lower_case(chan);
    if (canales[chan]) {
	if (canales[chan][1] == OPEN) CANALES -> part(chan);
	map_delete(canales, chan);
    }
    return 1;    
}

void open_channel(string chan) {
    chan = lower_case(chan);
    if (canales[chan]) {
	canales[chan][1] = OPEN;
	if (!CANALES -> join(chan)) write("Error abriendo canal "+chan+".\n");
	else write("Canal "+capitalize(chan)+" abierto.\n");
    }
}

void close_channel(string chan) {
    chan = lower_case(chan);
    if (canales[chan]) {
	canales[chan][1] = CLOSE;
	if (!CANALES -> part(chan)) write("Error cerrando canal "+chan+".\n");
	else write("Canal "+capitalize(chan)+" cerrado.\n");
    }
}

void join_all() {
    foreach(string chan in keys(canales)) {
	if (canales[chan][1] == OPEN) {
	    if (!CANALES -> join(chan)) {
    		write("Error abriendo canal "+chan+".\n");
	    }
	}
    }
}

void part_all() {
    foreach(string chan in keys(canales)) {
	if (canales[chan][1] == OPEN) {
	    if (!CANALES -> part(chan)) {
    		write("Error cerrando canal "+chan+".\n");
	    }
	}
    }
}

void send_channel(string chan, string msg) {
    string err;

    chan = lower_case(chan);
    if (!canales[chan]) {
	write("El canal "+capitalize(chan)+" no existe!\n");
	return;
    }
    if (canales[chan][1] == CLOSE) {
	write("El canal "+capitalize(chan)+" esta cerrado.\n"); 
	return;
    }
    if (canales[chan][2] == BANNED) {
	write("Estas baneado del canal "+capitalize(chan)+".\n"); 
	return;
    }
    if (CANALES -> read_only_channel?(chan)) {
	write("El canal "+capitalize(chan)+" solo es de lectura.\n");
	return;    
    }
    CANALES -> send(chan, capitalize(TP->nombre?())+" dice: "+msg);
}

void create() {
    int cid;
    
    /* Comprobar si se ha borrado alguno de nuestros  
     * canales mientras estabamos desconectados.
     */
    foreach(string chan in keys(canales)) {
	cid = CANALES -> cid?(chan);
	if ((cid == -1) || (cid != canales[chan][0])) {
	    map_delete(canales, chan);
	}
    }    
    /* Haremos JOIN en todos los canales OPEN al RESTAURAR LA FICHA */
}

void debug() {
    printf("%O\n", canales);
}

string * canales?() { 
    return keys(canales); 
}

int acceso_al_canal?(string canal) {
    if (canales[canal]) return 1;
    else return 0;
}

void banear(string canal) {
    if (canales[canal]) canales[canal][2] = BANNED;
}

void desbanear(string canal) {
    if (canales[canal]) canales[canal][2] = 0;
}

int abierto?(string canal) {
    if ((canales[canal]) && (canales[canal][1] == OPEN)) return 1;
    return 0;
}

int cerrado?(string canal) {
    if ((canales[canal]) && (canales[canal][1] == CLOSE)) return 1;
    return 0;
}

int baneado?(string canal) {
    if ((canales[canal]) && (canales[canal][2] == BANNED)) return 1;
    return 0;
}
