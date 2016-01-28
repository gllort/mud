#include <comando.h>
#include <daemons.h>

#define COLUMNAS 160


void create() {
    ::create();
    valid_flags("l");
}

int comando(int argc, string * argv) {
    int i, size;
    string file, * files, * tmp, base, file_or_dir;
    mapping ficheros = ([ ]);
    
    if (!argc) files = explore_path(TP -> cwd?()+"/*");
    else files = explore_path(argv[0]);
    
    foreach(string f in files) {
	size = sizeof(tmp = explode(f, "/"));
	if (size == 1) {
	    base = "/";
	    file_or_dir = tmp[0];
	}
	else {
	    base = "/"+implode(tmp[0..size-2],"/")+"/";
	    file_or_dir = tmp[size-1];
	}	
	if (find_dir(base+file_or_dir)) file_or_dir = "/"+file_or_dir;
	if (!ficheros[base]) ficheros[base] = ({ file_or_dir });
	else ficheros[base] += ({ file_or_dir });   
    }
    foreach(string dir in keys(ficheros)) {
	ficheros[dir] = sort_array(ficheros[dir], 1);
	size = sizeof(ficheros[dir]);
	for(i=0; i<size; i++) {

	    file = ficheros[dir][i];
	    if (file[0] == '/') file = file[1..];

	    if (PERMISOS -> read_access?(TP, dir+file)) {
		if (find_dir(dir+file)) ficheros[dir][i] = "$N$1"+ficheros[dir][i]+"$R"; 	    	
		else ficheros[dir][i] = "$N$2"+file+"$R"; 	    	
	    }
	    else {
		if (find_dir(dir+file)) ficheros[dir][i] = "$4"+ficheros[dir][i]+"$R";
		else ficheros[dir][i] = "$4"+ficheros[dir][i]+"$R";
	    }
	}
	write(dir+":\n");
        if (flag?("l")) write(implode(ficheros[dir],"\n")+"\n");
	else printf("%#-*s\n", COLUMNAS, color(implode(ficheros[dir],"\n")));
	write("\n");
    }
    return 1;
}

