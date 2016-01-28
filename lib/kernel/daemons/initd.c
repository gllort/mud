#include <mudlib.h>
#include <config.h>
#include <sql.h>
#include <daemons.h>
#include <errno.h>
#include <dirs.h>
#include <log.h>

inherit DAEMON;

void check_database();

void start() { 
    int i, k;
    string * daemons, * path, err;

    /* Comprobar que las tablas de la BD estan creadas */
    
    err = catch(check_database());    
    if (err) {
	write_file(LOG_INITD, err+"\n");
	shutdown(-1);	    
    }
    
    /* Cargar todos los daemons */
    daemons = ({ });
    
    err = catch(path = get_dir(DIR_K_DAEMONS+"/*.c"));
    if (err) {
	write_file(LOG_INITD, err+"\n");
	shutdown(-1);	    
    }    
    foreach(string daemon in path) daemons += ({ DIR_K_DAEMONS+"/"+daemon });

    err = catch(path = get_dir(DIR_G_DAEMONS+"/*.c"));
    if (err) {
	write_file(LOG_INITD, err+"\n");
	shutdown(-1);	    
    }    
    foreach(string daemon in path) daemons += ({ DIR_G_DAEMONS+"/"+daemon });
    
    k = sizeof(daemons);
    for(i=0; i<k; i++) {
	if (daemons[i] == INITD+".c") continue;
	err = catch(load_object(daemons[i]));
	if (err) {
	    /* Error inicializando un daemon */ 
	    /* Log y Shutdown system */
	    write_file(LOG_INITD, err);
	    shutdown(-1);	    
	}
    }
    stop();
} 

void stop()  { 
    destruct(TO);
}

void check_database() {
    int i, bd, rows;
    string err, * tablas = ({ });
    
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "SHOW TABLES;");
	if (rows == -1) err = db_error(bd);
	else {
	    tablas = CREA_ORDEN;
	    for (i=1; i<=rows; i++) tablas -= ({ db_fetch(bd, i)[0] });
	}
    }    
    if (err) {
        db_close(bd);
	throw(err);
    }
    foreach(string tabla in tablas) {
	rows = db_exec(bd, CREA_MAP[tabla]);    
	if (rows == -1) {
	    err = db_error(bd);
	    break;
	}
    }
    db_close(bd);
    if (err) throw(err);
}
