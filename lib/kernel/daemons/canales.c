#include <mudlib.h>
#include <config.h>
#include <errno.h>
#include <canales.h>
#include <macros.h>
#include <daemons.h>

inherit DAEMON;

#define CHAN_CID(x) canales[x]->cid
#define CHAN_TIPO(x) canales[x]->tipo

string * canales?();
varargs int register(string, int);

mapping canales = ([ ]);

void start() { 
    string err, chan; 
    int i, bd, rows;

    /* Cargamos los canales registrados en memoria */
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "SELECT * FROM canales;");
	if (rows >= 0) {
	    for (i=1; i<=rows; i++) {
		canales[db_fetch(bd,i)[1]] = new (class canal);
		canales[db_fetch(bd,i)[1]] -> cid = db_fetch(bd, i)[0];
		canales[db_fetch(bd,i)[1]] -> users = ({ });
		canales[db_fetch(bd,i)[1]] -> tipo = db_fetch(bd, i)[2];
	    }
    	    /* Primer arranque del mud. Crear canales basicos si no existen. */
	    foreach(string syschan in SYSTEM_CHANNELS) {
		if (!canales[syschan]) register(syschan);	    
	    }
	}
	else err = db_error(bd);
    }
    db_close(bd);
    if (err) throw(err);
} 

void stop()  { }

varargs int register(string chan, int tipo) {
    int cid, bd, rows;
    string err;
        
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	chan = lower_case(chan);
	rows = db_exec(bd, "BEGIN;");
	if ((tipo != READWRITE) && (tipo != READONLY)) tipo = READWRITE;
	rows = db_exec(bd, "INSERT INTO canales VALUES('', '"+chan+"',"+tipo+");");
	if (rows == -1) {
	    if (db_errno(bd) == 1062) err = E1(ERR_DUP_CHAN,chan);
	    else err = db_error(bd);
    	    db_rollback(bd);
	}
        else /* Insercion(es) correcta(s) */ {
	    cid = db_insert_id(bd);
    	    db_commit(bd);
	    canales[chan] = new (class canal);
	    canales[chan] -> cid = cid;
	    canales[chan] -> users = ({ });
	    canales[chan] -> tipo = tipo;
        }
    }
    db_close(bd);
    if (err) throw(err);
    return cid;
}

void drop(string chan) {
    int bd, rows, affected, cid;
    string err;
        
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	chan = lower_case(chan);
	rows = db_exec(bd, "BEGIN;");
	rows = db_exec(bd, "DELETE FROM canales WHERE canal='"+chan+"';");
	if (rows == -1) { 
	    err = db_error(bd); 
	    db_rollback(bd); 
	}
        else {
	    affected = db_affected_rows(bd);
	    db_commit(bd);
	    if (!affected) err = E1(ERR_NO_CHAN, chan);
	    else {
		/* Canal borrado */
    		cid = CHAN_CID(chan);
		foreach(object o in canales[chan]->users) {
		    o -> del_channel(cid);		
		}
		map_delete(canales, chan);
	    }
        }
    }
    db_close(bd);
    if (err) throw(err);
}

int join(string chan) {
    if (PO(0) != TP) return 0;
    if (canales[chan]) {
	canales[chan] -> users += ({ TP });
	return 1;
    }
    return 0;
}

int part(string chan) {
    if (PO(0) != TP) return 0;
    if (canales[chan]) {
        canales[chan] -> users -= ({ TP });
	return 1;
    }
    return 0;
}


int cid?(string chan) {
    if (!chan || chan=="") return -1;
    chan = lower_case(chan);
    if (canales[chan]) return CHAN_CID(chan);
    else return -1;
}

int existe_canal?(string chan) {
    if (!chan || chan=="") return 0;
    chan = lower_case(chan);
    if (canales[chan]) return 1;
    else return 0;
}

void send(string chan, string msg) {

    if ((PO(0) != TP) && (PO(0) != IRC) && (!adminp())) return;

    if ((chan == IRC_CHANNEL)&&(file_name(PO(0))!=IRC)) {
	/* Solo si el send no lo ha ejecutado el mismo IRCd */
	IRC -> send("PRIVMSG #mundo_oscuro :"+msg);
    }
    
    foreach(object user in canales[chan] -> users) {
	/* Por si algun jugador ha salido sin cerrar sus canales */
	if (!user || user == 0) canales[chan] -> users -= ({ 0 });
        else {
	    if ((user -> abierto?(chan)) && (!user -> baneado?(chan))) {
		tell_object(user, "\n[#"+upper_case(chan)+"] "+msg+"\n");    
	    }
	}
    }
}

string * canales?() { 
    return keys(canales); 
}

void debug() {
    printf("%O\n", canales);
}

int read_only_channel?(string chan) {
    if (canales[chan]) return canales[chan] -> tipo;
}
