#include <comando.h>

int comando(int argc, string * argv) {
    int i, size;
    string newdir, previo, * path, * named = ({ }), * unnamed = ({ });
    
    if (!argc) {
	notify_fail("Sintaxis: mkdir [directorio]\n");
	return 0;
    }
    if (argv[0][0] == '/') newdir = argv[0];
    else newdir = (TP->cwd?() == "/" ? "/"+argv[0] : TP->cwd?()+"/"+argv[0]);
    size = sizeof(path = explode(newdir, "/"));
    for(i=size-1; i>=0; i--) {
	if ((strsrch(path[i], "*") != -1) || (strsrch(path[i], "?") != -1) || (path[i]==".") || (path[i]=="..")) {
	    unnamed = path[0..i];
	    break;
	}
	named = ({ path[i] }) + named;
    }
    if (named == ({ })) {
	notify_fail("mkdir: "+newdir+": Especifica el directorio a crear.\n");
	return 0;
    }
    previo = "/";
    if (unnamed != ({ })) {
	path = explore_path("/"+implode(unnamed,"/"),1);
	if (!sizeof(path)) {
    	    notify_fail("mkdir: /"+implode(unnamed,"/")+": No existe el directorio.\n");
	    return 0;
	}
	previo = path[0];
	if (previo != "/") previo += "/";
    }
    newdir = previo+implode(named,"/");
    if (find_dir(newdir)) {
	notify_fail("mkdir: "+newdir+": El directorio ya existe.\n");
	return 0;
    }
    if (!mkdir(newdir)) {
	notify_fail("mkdir: "+newdir+": Error creando directorio.\n");
	return 0;
    }
    write("mkdir: "+newdir+": Directorio creado correctamente.\n");
    return 1;    
}
