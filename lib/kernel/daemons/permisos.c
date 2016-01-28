#include <mudlib.h>
#include <daemons.h>
#include <permisos.h>
#include <config.h>

inherit DAEMON;

#define GRANT	1
#define REVOKE  2

object find_access_object(string);
void base_y_destino(string, string ref, string ref);

void start() { ; }

void stop() { ; }

void create() { ; }

/* Generacion de permisos de nuevos archivos o directorios */
void crear(string path, string owner) {
    object permisos;
    int uid, rid, size, uidroot;
    string err, rnk, base, file_or_dir, * tmp;

    if ((!find_dir(path)) && (!find_file(path))) return;

    err = catch(rnk = RANGOS -> rango_inmortal(owner));    
    if (err) throw(err);
    err = catch(uid = USERS -> uid?(owner));
    if (err) throw(err);
    err = catch(uidroot = USERS -> uid?(ROOT));
    if (err) throw(err);
    if (uid == uidroot) return;
    err = catch(rid = GRUPOS -> gid?(rnk));
    if (err) throw(err);

    size = sizeof(tmp = explode(path,"/"));
    if (size == 1) { 
        base = "/";
        file_or_dir = tmp[0];
    }
    else { /* size > 1 */
        base = "/"+implode(tmp[0..size-2],"/")+"/";
        file_or_dir = tmp[size-1];
    } 	

    err = catch(permisos = find_access_object(base));    
    if (err) throw(err);

    err = catch(permisos -> crear(file_or_dir, uid, rid));
    if (err) throw(err);
}

/* Cambia los permisos de un fichero o directorio */
private void chmod(int operacion, string path, string grupo, int tipo_acceso) {
    int gid, admin_gid;
    object permisos;
    string base, file_or_dir, err;

    if ((tipo_acceso < C) && (tipo_acceso > RWC)) return;
    
    if ((find_dir(path)) || (find_file(path))) {
	base_y_destino(path, ref base, ref file_or_dir);
    }
    else { 
	/* Path inexistente */
	return;
    }

    err = catch(gid = GRUPOS -> gid?(grupo));
    if (err) throw(err); 

    err = catch(admin_gid = GRUPOS -> gid?(ADMIN_RANK));
    if (err) throw(err); 

    if (gid == admin_gid) return;
    
    err = catch(permisos = find_access_object(base));    
    if (err) throw(err);

    switch(operacion) {
	case GRANT:
	    err = catch(permisos -> grant(file_or_dir, gid, tipo_acceso));
	    break;
	case REVOKE:
	    err = catch(permisos -> revoke(file_or_dir, gid, tipo_acceso));
	    break;
    }
    if (err) throw(err);
}

/* Da permiso de tipo_acceso al archivo o directorio path para el grupo indicado */
void grant(string path, string grupo, int tipo_acceso) {
    string err;
    err = catch(chmod(GRANT, path, grupo, tipo_acceso));
    if (err) throw(err);    
}

/* Revoca permisos de un grupo sobre un archivo o directorio */  
void revoke(string path, string grupo, int tipo_acceso) {
    string err;
    err = catch(chmod(REVOKE, path, grupo, tipo_acceso));
    if (err) throw(err);    
}

private nomask int objeto_de_permisos?(object ob) {
    if (file_name(ob) == OB_PERMISOS) return 1;
    else return 0;
}

private nomask object find_access_object(string base) {
    object ob, * obs;

    obs = objects( (: objeto_de_permisos? :) );
    foreach(ob in obs) if (ob -> directorio?() == base) return ob;     
    /* El fichero de permisos no esta en memoria. Clonamos uno. */
    ob = new(OB_PERMISOS, base);    
    if (!ob) error("Error creando o restaurando el fichero de permisos.\n");    
    return ob;
}

private nomask int valid_access?(object doer, string destino, int operacion) {
    int * gids = ({ }), bit, uid;
    object permisos;
    string base, file_or_dir;

    if (adminp()) return 1;

    if (destino == "/") {
	switch(operacion) {
	    case READ: return 1;
	    case WRITE: return 0;
	    case CALL: return 0;
	    default: return 0;
	}
    }

    base_y_destino(destino, ref base, ref file_or_dir);

    permisos = find_access_object(base);    

    if (catch(uid = USERS -> uid?(doer -> nombre?()))) return 0;
    if (permisos -> owner_uid?(file_or_dir) == uid) return 1;

    if ((doer -> nivel_inmortal?()) >= (RANGOS -> nivel((GRUPOS -> grupo_con_gid?(permisos -> rank_access?(file_or_dir)))))) return 1;

    foreach(string grupo in GRUPOS -> grupos?(doer->nombre?())) {
	gids += ({ GRUPOS -> gid?(grupo) });
    }
    switch(operacion) {
	case READ: bit = 2; break;
	case WRITE: bit = 1; break;
	case CALL: bit = 0; break;
	default: return 0;
    }
    foreach(int gid in gids) {
	if (test_bit(permisos -> permisos?(gid, file_or_dir), bit)) return 1;
    }
    return 0;    
}

nomask int read_access?(object doer, string dest) {
    return valid_access?(doer, dest, READ);
}

nomask int write_access?(object doer, string dest) {

    printf("wr_acc: %O %O\n", doer, dest);

    return valid_access?(doer, dest, WRITE);
}

nomask int call_access?(object doer, string dest) {
    return valid_access?(doer, dest, CALL);
}


private nomask void base_y_destino(string path, string ref base, string ref file_or_dir) {
    int size;
    string * tmp;

    size = sizeof(tmp = explode(path,"/"));
    if (size == 1) { 
        base = "/";
        file_or_dir = tmp[0];
    }
    else { /* size > 1 */
        base = "/"+implode(tmp[0..size-2],"/")+"/";
        file_or_dir = tmp[size-1];
    } 	
    file_or_dir = explode(file_or_dir, "#")[0];
} 

int owner_uid?(string path) {
    object permisos;
    string base, file_or_dir;

    base_y_destino(path, ref base, ref file_or_dir);

    permisos = find_access_object(base);    
    return permisos->owner_uid?(file_or_dir);
}

int rank_access?(string path) {
    object permisos;
    string base, file_or_dir;

    base_y_destino(path, ref base, ref file_or_dir);

    permisos = find_access_object(base);    
    return permisos->rank_access?(file_or_dir);
}
