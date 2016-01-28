#include <mudlib.h>
#include <errno.h>
#include <config.h>
#include <login.h>
#include <dirs.h>
#include <move.h>
#include <daemons.h>

inherit DAEMON;

private mapping inmortales = ([ ]);

varargs void registrar(string, string, string); 
string * info(string);
void promote(string, string); 
private nomask string codegen(string, string, string);

void start() { 
    int i, bd, rows;
    string err;
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "SELECT * FROM inmortales;");
	if (rows == -1) err = db_error(bd);
	else {
	    for (i=1; i<=rows; i++) inmortales[db_fetch(bd,i)[1]] = db_fetch(bd,i)[0];
	    foreach(string sysuser in SYSTEM_USERS) {
		if (!inmortales[sysuser]) {
    		    err = catch(registrar(sysuser, "!!", "!!", NOLOGIN));
		    if ((err) && (errno(err) != ERR_DUP_USER)) break;
		    err = catch(promote(sysuser, ADMIN_RANK));
		    if (err) break;
		}
	    }
	}
    }
    db_close(bd);
    if (err) throw(err);
}
 
void stop()  { /* Skip */ }

varargs void registrar(string user, string pass, string mail, int status) { 
    int bd, rows, found;
    string err;

    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "INSERT INTO jugadores VALUES ('"+user+"','"+pass+"','"+mail+"','"+codegen(user,pass,mail)+"',"+status+");");
	if (rows == -1) {
	    if (db_errno(bd) == 1062) err = E1(ERR_DUP_USER,user);
	    else err = db_error(bd);
	}
    }
    db_close(bd);
    if (err) throw(err);
}

int registrado?(string user) {
    string err = catch(info(user));
    if ((err) && (err != ERR_NO_USER)) throw(err);
    else if (err) return 0;
    else return 1;
}

void deregistrar(string user) { 
    int bd, rows;
    string err;

    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "DELETE FROM jugadores WHERE nombre='"+user+"';");
	if (!db_affected_rows(bd)) err = E1(ERR_NO_USER,user);
    }
    db_close(bd);
    if (err) throw(err);
}

string * info(string user) {
    int bd, rows;
    string err, * res;

    user = lower_case(user);
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "SELECT * FROM jugadores WHERE nombre='"+user+"';");
	if (rows == -1) err = db_error(bd);
	else if (rows) res = db_fetch(bd, 1);
	else err = E1(ERR_NO_USER,user);
    }
    db_close(bd);
    if (err) throw(err);
    return res;
}

void promote(string user, string rango) { 
    int bd, rows, no_home;
    object ob, newob;
    string err;

    if (!RANGOS->es_rango?(rango)) throw(E1(ERR_NO_RANK,rango));
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "INSERT INTO inmortales VALUES ('','"+user+"','"+rango+"');");
	if (rows == -1) {
	    if (db_errno(bd) == 1216) err = E1(ERR_NO_USER,user);
	    else if (db_errno(bd) == 1062) err = E1(ERR_DUP_INMORTAL,user); 
	    else err = db_error(bd);
	}
	else {
	    /* La insercion en la BD no produjo errores
	     * Agregamos al inmortal el grupo de trabajo de su rango
	     * Actualizamos estructura de inmortales en memoria
	     */ 
	    err = catch(GRUPOS -> asignar(user, rango));
	    if (err) {
		rows = db_exec(bd, "DELETE FROM inmortales WHERE nombre='"+user+"';");
		if (rows == -1) {
		    err = db_error(bd);
		}
	    }
	    else inmortales[user] = db_insert_id(bd);
	}
    }
    db_close(bd);
    if (err) throw(err);

    // Si estamos aqui, hemos insertado el inmortal en la BD y no hay errores 
    
    if (member_array(user, SYSTEM_USERS) == -1) {  
	// El inmortal es una persona.
	// Si el inmortal esta conectado, lo mutamos a un objeto inmortal.
	// El canal del rango lo agregamos en la inicializacion de ese 
	// objeto para evitar complicaciones aqui.

        if (!mkdir(DIR_HOME+"/"+user)) no_home = 1;	
	else catch(PERMISOS -> crear(DIR_HOME+"/"+user, user));
	
        ob = find_player(user);
        if (ob) {
            tell_object(ob, sprintf("Has sido ascendido a %s.\nHaz honor a tu cargo!\n", capitalize(rango)));
	    if (no_home) tell_object(ob, "Error creando tu directorio de trabajo "+DIR_HOME+"/"+user+"\n");
	    if (!ob -> guardar(1)) {
		tell_object(ob, "Guarda tu ficha y reconecta para completar el proceso.\n");	    
		return;
	    }
    	    if (err = catch(newob = clone_object(INMORTAL))) {
		tell_object(ob, "Error migrandote a un objeto inmortal. Reconecta para completar el proceso.\n");
		destruct(newob);
		return;
	    }
    	    newob -> nombre(user);
	    newob -> inicializar();
    	    if (!newob -> cargar_ficha()) {
		tell_object(ob, "Reconecta para completar el proceso.\n");
		destruct(newob);
		return;
	    }
    	    exec(newob, ob);
	    destruct(ob);
            if (!newob -> guardar(1)) {
		tell_object(newob, "Guarda tu ficha para finalizar el proceso.\n");
		return;
	    }
	}
    }	
}


void demote(string user, string motivo) {
    int bd, rows;
    string err;
    object ob, newob;
    
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "DELETE FROM inmortales WHERE nombre='"+user+"';");
	if (rows == -1) err = db_error(bd);
	else {
	    if (!db_affected_rows(bd)) {
		err = E1(ERR_NO_INMORTAL,user);	    
	    }
	    else {
		/* Inmortal eliminado de la BD correctamente */
		map_delete(inmortales, user);
	    }
	}
    }
    db_close(bd);
    if (err) throw(err);

    /* Si el inmortal esta conectado, lo mutamos a un objeto jugador */
    ob = find_player(user);
    if (ob) {
	tell_object(ob, "Has sido degradado por "+capitalize(TP->nombre?())+".\nMotivo: "+motivo+".\n"); 
	if (err = catch(newob = clone_object(JUGADOR))) {
	    destruct(ob);
	}
	newob -> nombre(user);
	newob -> inicializar();
	if (!newob -> crear_ficha()) {
	    destruct(newob);
	    destruct(ob);
	}
	if (!newob -> guardar(1)) {
	    destruct(newob);
	    destruct(ob);
	}
	exec(newob, ob);
	destruct(ob);
    }
    /* Falta mover el directorio del HOME a /save o similar */
}

int inmortal?(string user) {
    user = lower_case(user);
    return (inmortales[user] ? 1 : 0);
}

int uid?(string user) {
    user = lower_case(user);
    if (!inmortales[user]) throw(E1(ERR_NO_INMORTAL,user));
    return inmortales[user];
}

string inmortal_con_uid?(int uid) {
    int idx;
    idx = member_array(to_int(uid), values(inmortales));    
    if (idx == -1) throw(E1(ERR_NO_INMORTAL,uid));
    return keys(inmortales)[idx];
}

private nomask string codegen(string n, string p, string m) {
    int i,j,k;
    string plaintext = n+m+p, randomtext = "";
    j = strlen(plaintext);
    for (i=0; i<j; i++) {
	k = random(j);
	randomtext += plaintext[k..k];
    }
    return "";
    /* return mkpasswd(randomtext); */
}

void debug() {
    printf("%O\n", inmortales);
}

string * inmortales?() { return keys(inmortales); }
