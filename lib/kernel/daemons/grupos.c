#include <mudlib.h>
#include <config.h>
#include <errno.h>
#include <dirs.h>
#include <permisos.h>

inherit DAEMON;

private nosave mapping grupos = ([ ]);

int es_grupo?(string); 
varargs int crear(string, int);

void start() { 
    string err; 
    int i, bd, rows;

    /* Cargamos la lista de grupos en memoria */
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "SELECT * FROM grupos;");
	if (rows >= 0) {
	    for (i=1; i<=rows; i++) grupos[db_fetch(bd,i)[1]] = to_int(db_fetch(bd,i)[0]);
	    /* Crear grupos de sistema si no existen */
    	    foreach(string grupo in SYSTEM_GROUPS) {
		if (!es_grupo?(grupo)) {
		    err = catch(crear(grupo));			
	        }		
	    }
	}
	else err = db_error(bd);
    }
    db_close(bd);
    if (err) throw(err);
} 

void stop()  { /* Skip */ }

varargs string * grupos?(string user) { 
    if (!user) return keys(grupos); /* Todos los grupos */
    else { /* Los grupos de un usuario */
	int bd, rows;
	string * grps = ({ }), err;
	
	user = lower_case(user);
        if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
        else {
	    rows = db_exec(bd, "SELECT grupo FROM miembros WHERE nombre='"+user+"';");
	    if (rows == -1) err = db_error(bd);
    	    else for(int i=1; i<=rows; i++) grps += ({ db_fetch(bd, i)[0] });
        }
	db_close(bd);
	if (err) throw(err);
	return grps;
    }
}

int es_grupo?(string grupo) { 
    return (member_array(lower_case(grupo), keys(grupos)) == -1 ? 0 : 1);
}

string * miembros(string grupo) {
    string err, * miembros;
    int i, bd, rows;
    
    grupo = lower_case(grupo);
    if (!grupo || grupo=="" || !es_grupo?(grupo)) throw(E1(ERR_NO_GROUP, grupo));
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	rows = db_exec(bd, "SELECT nombre FROM miembros WHERE grupo='"+grupo+"';");
	if (rows == -1) err = db_error(bd);
	else for(i=1; i<=rows; i++) miembros += ({ db_fetch(bd, i)[0] });
    }
    db_close(bd);
    if (err) throw(err);
    return miembros;
}

varargs int crear(string grupo, int es_rango) {
    int bd, rows, res;
    string err, sql;
    
    grupo = lower_case(grupo);
    if (es_grupo?(grupo)) throw(E1(ERR_DUP_GROUP, grupo));
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	rows = db_exec(bd, "BEGIN;");
	if (rows == -1) err = db_error(bd);
	else {
    	    sql = "INSERT INTO grupos VALUES ('', '"+grupo+"');";
	    rows = db_exec(bd, sql);

	    if (rows == -1) {
		/* Un error de clave duplicada no puede ser, porque
		 * comprobamos si el grupo ya existe con la funcion
		 * es_grupo?().
		 */
		err = db_error(bd);
	    }
	    else {
		/* Insercion correcta en grupos */
    	        res = db_insert_id(bd);
		if (es_rango) {
		    sql = "INSERT INTO rangos VALUES ('"+grupo+"', "+es_rango+");";
		    rows = db_exec(bd, sql);
		    if (rows == -1) { 
			/* Error de clave duplicada o error SQL. 
	    		 * La clave duplicada solo puede ser el 
			 * level de un rango. 
			 */
			if (db_errno(bd) == 1062) err = E1(ERR_DUP_LEVEL,es_rango);
			else err = db_error(bd);
		    }
		    else {
			/* Insercion correcta en rangos */
			grupos[grupo] = res;
		    }
		}
		else {
		    grupos[grupo] = res;
		}
    	    }
	}	
    }
    if (!err) err = catch(CANALES -> register(grupo));
    if ((!err) && (!mkdir(DIR_CMD+"/"+grupo))) {
	err = "Error creando directorio de comandos del grupo.\n";   
    }
    if (!err) {
	err = catch(PERMISOS -> grant(DIR_CMD+"/"+grupo, grupo, R));
    }
    if (err) {
	/* Algun error? Rollback y fuera */
	db_rollback(bd);
	db_close(bd);
	throw(err);
    }
    /* Si estamos aqui, no ha habido errores. Commit y final */
    db_commit(bd);
    db_close(bd);
    return res;
}

void eliminar(string grupo) {
    int bd, rows;
    string err;
    
    grupo = lower_case(grupo);
    if (!es_grupo?(grupo)) throw(E1(ERR_NO_GROUP, grupo));
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	db_exec(bd, "BEGIN;"); 
	rows = db_exec(bd, "DELETE FROM grupos WHERE grupo='"+grupo+"';");
	if (rows == -1) {
	    err = db_error(bd);
	    db_rollback(bd);
	}
	else {

	    err = catch(CANALES -> drop(grupo));
	    if (err) db_rollback(bd);
	    else {
    		db_commit(bd);
		map_delete(grupos, grupo);
	    }
	}
    }    
    db_close(bd);
    if (err) throw(err);

    /* Mover los comandos del grupo al directorio de un rango superior 
     * y cargarse el directorio!
     */
    
}
    
int miembro?(string user, string grupo) {
    int bd, rows, res;
    string err;
    
    grupo = lower_case(grupo);
    if (!es_grupo?(grupo)) throw(E1(ERR_NO_GROUP,grupo));
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	user = lower_case(user);
	rows = db_exec(bd, "SELECT * FROM miembros WHERE nombre='"+user+"' and grupo='"+grupo+"';");
	if (rows == -1) err = db_error(bd);
	else res = rows;
    }
    db_close(bd);
    if (err) throw(err);
    return res;
}

void asignar(string user, string grupo) {
    int bd, rows;
    string err;
    
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
        user = lower_case(user);
	grupo = lower_case(grupo);
	rows = db_exec(bd, "INSERT INTO miembros VALUES ('"+user+"', '"+grupo+"');");
	if (rows == -1) {
	    if (db_errno(bd) == 1062) err = E2(ERR_DUP_MEMBER,user,grupo);
	    else if (db_errno(bd) == 1216) {
		/* Error de FK */
	        if (!es_grupo?(grupo)) err = E1(ERR_NO_GROUP,grupo);
		else err = E1(ERR_NO_INMORTAL,user);
	    }
	    else err = db_error(bd); 
	}
	else {
	    /* Ok */
	}
    }    
    db_close(bd);
    if (err) throw(err);
}

void deasignar(string user, string grupo) {
    int bd, rows, affected;
    string err;

    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
        user = lower_case(user);
	grupo = lower_case(grupo);
	rows = db_exec(bd, "DELETE FROM miembros WHERE nombre='"+user+"' AND grupo='"+grupo+"';");
	if (rows == -1) err = db_error(bd);
	else {
	    affected = db_affected_rows(bd);
	    if (!affected) {
		if (!es_grupo?(grupo)) err = E1(ERR_NO_GROUP, grupo);
		else err = E2(ERR_NO_MEMBER, user, grupo);
	    }
	    else {
		/* Ok */
	    }
	}
    }
    db_close(bd);
    if (err) throw(err);
}

int gid?(string grupo) {
    grupo = lower_case(grupo);
    if (!grupos[grupo]) throw(E1(ERR_NO_GROUP,grupo));
    return grupos[grupo];
}

string grupo_con_gid?(int gid) {
    int idx;
    idx = member_array(to_int(gid), values(grupos));    
    if (idx == -1) throw(E1(ERR_NO_GROUP,gid));
    return keys(grupos)[idx];
}

void debug() {
    printf("%O\n", grupos);
}
