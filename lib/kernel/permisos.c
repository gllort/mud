#include <daemons.h>
#include <permisos.h>
#include <config.h>

int save_me();

private nosave string base_dir;
private mapping permisos = ([ ]);

class permiso {
    int uid;
    int rid;
    mapping grupos;
}

void create(string dir) {
    if (file_name(PO(0)) != PERMISOS) return;

    if (dir) {
        base_dir = dir;  
        if (find_file(base_dir+ACCESS_FILE)) {
    	    if (!restore_object(base_dir+ACCESS_FILE)) {
		destruct(TO);
	    }
	}
        else {
	    if (!save_me()) {
		destruct(TO);
	    }
	}
    }
}

void crear(string file_or_dir, int uid, int rid) {
    if (file_name(PO(0)) != PERMISOS) return;

    permisos[file_or_dir] = new (class permiso);
    permisos[file_or_dir] -> uid = uid;
    permisos[file_or_dir] -> rid = rid;
    permisos[file_or_dir] -> grupos = ([ ]);
    if (!save_me()) throw("Error almacenando permisos.\n");
}

void grant(string file_or_dir, int gid, int tipo_acceso) {
    int uid_root, rid_root;
    string err, bits;
        
    if (file_name(PO(0)) != PERMISOS) return;

    if (!permisos[file_or_dir]) {
        permisos[file_or_dir] = new (class permiso);
	err = catch(uid_root = USERS -> uid?(ROOT));
	if (err) throw(err);
	err = catch(rid_root = GRUPOS -> gid?(RANGOS -> rango_maximo?()));	if (err) throw(err);
	if (err) throw(err);
        permisos[file_or_dir] -> uid = uid_root;
	permisos[file_or_dir] -> rid = rid_root;
	permisos[file_or_dir] -> grupos = ([ ]);
    }
    if (!permisos[file_or_dir]->grupos[gid]) {
	permisos[file_or_dir]->grupos[gid] = clear_bit("",2);
    }

    switch(tipo_acceso) {
	case C: 
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 0);
	    break;
	case W: 
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 1);
	    break;
	case R: 
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 2);
	    break;
	case WC: 
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 0);
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 1);
	    break;
	case RC: 
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 0);
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 2);
	    break;
	case RW: 
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 1);
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 2);
	    break;
	case RWC: 
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 0);
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 1);
	    permisos[file_or_dir]->grupos[gid] = set_bit(permisos[file_or_dir]->grupos[gid], 2);
	    break;
    }

    if (!save_me()) throw("Error almacenando permisos.\n");
}

void revoke(string file_or_dir, int gid, int tipo_acceso) {
    string bits, err;
    int uid_root, rid_root;
    
    if (file_name(PO(0)) != PERMISOS) return;

    if (!permisos[file_or_dir]) return;

    if (permisos[file_or_dir]->grupos[gid]) {

	bits = permisos[file_or_dir]->grupos[gid];
        switch(tipo_acceso) {
	    case C: 
    		bits = clear_bit(bits, 0);
		break;
	    case W: 
    		bits = clear_bit(bits, 1);
		break;
	    case R: 
    		bits = clear_bit(bits, 2);
		break;
	    case WC: 
    		bits = clear_bit(bits, 0);
    		bits = clear_bit(bits, 1);
		break;
	    case RC: 
    		bits = clear_bit(bits, 0);
    		bits = clear_bit(bits, 2);
		break;
	    case RW: 
    		bits = clear_bit(bits, 1);
    		bits = clear_bit(bits, 2);
		break;
	    case RWC: 
    		bits = clear_bit(bits, 0);
    		bits = clear_bit(bits, 1);
    		bits = clear_bit(bits, 2);
	        break;
	}

	if ((!test_bit(bits, 2)) && (!test_bit(bits, 1)) && (!test_bit(bits, 0))) {
	    map_delete(permisos[file_or_dir]->grupos,gid);
	}

	if (!sizeof(permisos[file_or_dir]->grupos)) {
    	    err = catch(uid_root = USERS -> uid?(ROOT));
	    if (err) throw(err);
	    err = catch(rid_root = GRUPOS -> gid?(RANGOS -> rango_maximo?()));	if (err) throw(err);
	    if (err) throw(err);
	    if ((permisos[file_or_dir] -> uid == uid_root) && (permisos[file_or_dir] -> rid == rid_root)) {
		map_delete(permisos, file_or_dir);	    
	    }
	}
    }
    if (!save_me()) throw("Error almacenando permisos.\n");
}

void dump() {
    printf("%O\n",permisos);
}

string directorio?() { return base_dir; }

private int save_me() {
    return save_object(base_dir+ACCESS_FILE);
}

int owner_uid?(string file_or_dir) {
    int uid;

    if (!permisos[file_or_dir]) return USERS -> uid?(ROOT);
    else {
	uid = permisos[file_or_dir] -> uid;

	if (catch(USERS -> inmortal_con_uid?(uid))) {
	    permisos[file_or_dir] -> uid = USERS -> uid?(NOBODY);
	    permisos[file_or_dir] -> rid = GRUPOS -> gid?(RANGOS -> rango_maximo?());
	    save_me();
	    return permisos[file_or_dir] -> uid;
	}
	else return uid;
    }
}

int rank_access?(string file_or_dir) {
    int rid;

    if (!permisos[file_or_dir]) return GRUPOS -> gid?(RANGOS -> rango_maximo?());
    else {
	rid = permisos[file_or_dir] -> rid;    
	if (catch(GRUPOS -> grupo_con_gid?(rid))) {
	    permisos[file_or_dir] -> uid = USERS -> uid?(NOBODY);
	    permisos[file_or_dir] -> rid = GRUPOS -> gid?(RANGOS -> rango_maximo?());
	    save_me();
    	    return permisos[file_or_dir] -> rid;    
	}
	else return rid;
    }
}

string permisos?(int gid, string file_or_dir) {
    if (!permisos[file_or_dir]) return clear_bit("", 2);
    else {
	if (!permisos[file_or_dir] -> grupos[gid]) return clear_bit("", 2);
	else return permisos[file_or_dir] -> grupos[gid]; 
    }
}


/*
int read_access?(object doer, string file_or_dir) {
    int * gids = ({ });
    int rid, nivel;
        
    if (adminp()) return 1;
    if (!permisos[file_or_dir]) {
	// No hay permisos para ese fichero, de modo que pertenece 
	// a ROOT@ADMIN, y solo un adminp podria haber entrado ya.
	return 0;
    }
    if (USERS -> uid?(doer->nombre?()) == permisos[file_or_dir] -> uid) return 1;

    if ((doer -> nivel_inmortal?()) >= (RANGOS -> nivel((GRUPOS -> grupo_con_gid?(permisos[file_or_dir] -> rid))))) return 1;

    foreach(string grupo in GRUPOS -> grupos?(doer->nombre?())) {
	gids += ({ GRUPOS -> gid?(grupo) });
    }
    foreach(int gid, string perms in permisos[file_or_dir] -> grupos) {
	if ((member_array(gid, gids) != -1) && (test_bit(perms,2))) return 1;
    }
    return 0;    
}

int write_access?(object doer, string file_or_dir) {
    if (adminp()) return 1;
//    printf("WRACC: %O %O\n", doer, file_or_dir);
    return 1;
}

int call_access?(object doer, string file_or_dir) {
    if (adminp()) return 1;
//    printf("CAACC: %O %O\n", doer, file_or_dir);
    return 1;
}
*/

