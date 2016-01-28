#include <comando.h>
#include <errno.h>

int comando(int argc, string * argv) {
    int i, size;
    string err, * dirs, * files, directorio, actual;
    
    if (!argc) {
	notify_fail("Sintaxis: rmdir [directorio]\n");
	return 0;
    }
    if (argv[0][0] == '/') directorio = argv[0];
    else directorio = (TP->cwd?() == "/" ? "/"+argv[0] : TP->cwd?()+"/"+argv[0]);   

    dirs = explore_path(directorio, 1);
    if (!sizeof(dirs)) {
	notify_fail("rmdir: "+directorio+": No existe el directorio.\n");
	return 0;
    }    
    for(i=0; i<sizeof(dirs); i++) {
	actual = dirs[i];
	if (actual != "/") actual += "/";

	err = catch(files = get_dir(actual));
	if (err) {
	    write("rmdir: "+serror(err)+"\n");
	    continue;    
	}
	
	if (sizeof(files)) {
	    write("rmdir: "+dirs[i]+": El directorio no esta vacio.\n");
	    continue;
	}
	if (!rmdir(dirs[i])) {
	    write("rmdir: "+dirs[i]+": Error borrando directorio.\n");
	}
	else {
	    write("rmdir: "+dirs[i]+": Directorio borrado correctamente.\n");
	}
    }
    return 1;
}
