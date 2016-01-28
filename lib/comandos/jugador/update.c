#include <comando.h>
#include <mudlib.h>

int update(string);

int comando(int argc, string * argv) {
    int i, j, size;
    object ob;
    string file;
    string * paths, * files;

    for(i=0; i<argc; i++) {
        size = sizeof(files = explore_path(argv[i], 2));
	if (size) {
	    for(j=0; j<size; j++) {	
		file = files[j];
		if (find_dir(file)) {
		    notify_fail("update: "+file+" es un directorio.\n");
		    return 0;
		}
		if ((ob = find_object(file)) && (ob != find_object(VOID))) {
		    /* pillar los objetos que estan dentro de ob 
	    	    * y llevarlos a void.
	            * Opcion: despues del update, intentar llevarlos de vuelta al origen, 
	            * y sino a void.
		    */
		}
		if (!update(file)) {
		    write("update: "+file+" no necesita actualizarse.\n");
		}
	    }
	}
	else {
    	    write("update: No se encuentra el archivo '"+argv[i]+"'.\n");
	}
    }
    return 1;
}

int update(string file) {
    object ob;
    int parents_updated = 0;

    mixed * info = stat(file);
    ob = find_object(file);
    if (!ob) ob = load_object(file);
    if (ob) {
        foreach(string fn in inherit_list(ob)) {
	    if (update(fn)) parents_updated = 1;
	}
        /* t_touched <= t_loaded && padres no se han updatado => No update */ 
	if ((info[1] <= info[2]) && (!parents_updated)) {
	    return 0;  /* Not needed */
        }
	destruct(ob);
    }   
    load_object(file);
    if (file[0] != '/') file = "/"+file;
    write(file + ": Actualizado y cargado.\n");
    return 1; /* File has been updated */
}
