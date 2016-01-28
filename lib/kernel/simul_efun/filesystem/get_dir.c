#include <mudlib.h>
#include <errno.h>

varargs mixed * get_dir(string dir, int flag) {
    int size;
    string base, * path, * files;

    if ((TP) && (inherits(JUGADOR, TP))) {
	size = sizeof(path = explode(dir,"/"));
	if (size == 1) base = "/";
	else base = "/"+implode(path[0..size-2],"/")+"/";

	if (!PERMISOS -> read_access?(TP, base)) {
    	    throw(E1(ERR_NO_ACCESS, base));
	}
    }
    
    files = efun::get_dir(dir, flag);
    if (!sizeof(files)) return ({ });

    foreach(string file in files) {
	if (relative_name(file) == relative_name(OB_PERMISOS)+".o") {
	    files -= ({ file });
	}		
    }
    files -= ({ "." });
    files -= ({ ".." });
    return files;
}    
