#include <dirs.h>
#include <errno.h>
#include <mudlib.h>
#include <daemons.h>

int valid_asm(string file) {
    return 0;
}

/* 
int valid_bind(object binder, object old_owner, object new_owner) {

}
*/

int valid_compile_to_c(string file) {
    return 1;
}

int valid_hide(object ob) {
    if (adminp()) return 1;
    else return 0;
}

int valid_link(string from, string to) {
    return 0;
}

int valid_object(object ob) {
    return 1;
}

int valid_override(string file, string efun_name) {

    return 1;

    if (file == "/kernel/simul_efun") { return 1; }
    if (file == "/kernel/master") { return 1; }
    return 0; 
}

int valid_save_binary(string file) {
    return 1;
}

int valid_seteuid(object ob, string euid) {
    return 1;
}

int valid_socket(object eff_user, string fun, mixed * info) {
    return 1;
}

int valid_shadow(object ob) {
    return 1;
}

int valid_read(string file, mixed user, string func) {
    int size;
    string base, * tmp;
    
    return 1;
    
//    printf("R %O: %O (by %O)\n", func, file, file_name(user));
    if (relative_name(file) == relative_name(OB_PERMISOS)+".o") {
	if ((file_name(PO(0)) == OB_PERMISOS) || (PO(0) == master())) return 1;
        else return 0;
    }
    if ((TP) && (inherits(JUGADOR, TP)>0)) {

	if (subdir(file, DIR_CMD)) {
	    return 1;
	}

	switch(func) {
	    case "file_size":
		size = sizeof(tmp = explode(file,"/"));
		if (size == 1) base = "/";
		else base = "/"+implode(tmp[0..size-2],"/")+"/";
		if (!PERMISOS -> read_access?(TP, base)) throw(E1(ERR_NO_ACCESS,base));
		break;
	    case "restore_object":
		if (file != DIR_FICHAS+"/"+TP->nombre?()[0..0]+"/"+TP->nombre?()+".o") return 0;    
		break;
	    case "read_file":
		if (!PERMISOS -> read_access?(TP, file)) throw(E1(ERR_NO_ACCESS,file));
		break;
	}
	return 1;
    }
    else return 1;
}

int valid_write(string file, mixed user, string func) {
//    printf("W %O: %O (by %O)\n", func, file, file_name(user));

    return 1;

    if (relative_name(file) == relative_name(OB_PERMISOS)+".o") {
	if ((file_name(PO(0)) == OB_PERMISOS) || (PO(0) == master())) return 1;
        else throw(file+": Nombre de fichero reservado.");
    }
/*
    if (TP) {
	if (!PERMISOS -> write_access?(TP, file)) {
	    throw(E1(ERR_NO_ACCESS,file));
	}
    }
*/    
    return 1;
}

mixed valid_database(object ob, string action, mixed * info) {
    return "BN1TKUBr1p7x";
}
