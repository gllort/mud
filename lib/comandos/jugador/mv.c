#include <comando.h>

int comando(int argc, string * argv) {
    string * ori, origen, destino;    

    if (argc != 2) {
	notify_fail("Sintaxis: mv [origen] [destino]\n");
	return 0;
    }

    origen  = argv[0];
    destino = argv[1];

    ori = explore_path(origen);

    foreach(string file in ori) {
	move(file, destino);
    }
}

int move(string file_or_dir, destino) {
    string * files;
    
    if (find_dir(file_or_dir)) {
	files = explore_path(file_or_dir+"/*");
	foreach(string file in files) {
	    move(file, destino);
	}
    }


}

    numori = sizeof(files = explore_path(origen, 2));
    if (!numori) {
	notify_fail("cp: "+origen+": No existe el archivo origen.\n");
	return 0;
    }
    dirs = explore_path(destino, 1);
    if (sizeof(dirs)) {
	es_dir = 1;
	destdir = (dirs[0] == "/" ? "/" : dirs[0]+"/");
    }
    else {
	es_file = 1;
	if (destino[0]!='/') destino = (TP->cwd?()=="/" ? "/"+destino : TP->cwd?()+"/"+destino);
        size = sizeof(path = explode(destino,"/"));
	destdir = (size == 1 ? "/" : "/"+implode(path[0..size-2],"/")+"/");
	destfile = path[size-1];
	path = explore_path(destdir, 1);
	if (sizeof(path)) destdir = (path[0]=="/" ? "/" : path[0]+"/");
	else {
	    notify_fail("cp: "+destdir+": No existe el directorio.\n");
	    return 0;
	}
    }
    if ((numori > 1) && (es_file)) {
	notify_fail("cp: "+destdir+destfile+": No existe el directorio.\n");
	return 0;
    }
    foreach(string file in files) {
        size = sizeof(path = explode(file,"/"));
	oridir = (size == 1 ? "/" : "/"+implode(path[0..size-2],"/")+"/");
	orifile = path[size-1];

	if (es_dir) tmp = destdir+orifile;
	else if (es_file) tmp = destdir+destfile;
	
	write("Copiando "+oridir+orifile+" a "+tmp+"...\n");
	if (cp(oridir+orifile, tmp) != 1) {
	    write("cp: "+oridir+orifile+": Error copiando archivo a "+tmp+".\n");
	}
    }
    return 1;
}

