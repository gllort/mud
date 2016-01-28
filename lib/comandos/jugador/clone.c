#include <comando.h>
#include <move.h>

/* Lima inspired */


int comando(int argc, string * argv) {
    int i, repeticiones;
    object * obs;
    string * clones;

    if (!argc || argc>2) {
	notify_fail("Sintaxis incorrecta.\nUso: clone <archivo> [repeticiones]\n");
	return 0;
    }

    clones = explore_path(argv[0], 2);
    if (!sizeof(clones)) {
	notify_fail("clone: "+argv[0]+": No se encuentra el archivo.\n");
	return 0;
    }
    if (argc == 1) repeticiones = 1;
    else repeticiones = to_int(argv[1]);
    
    foreach(string clonefile in clones) {
	obs = allocate(repeticiones);
	for(i=0; i<repeticiones; i++) {
	    obs[i] = new(clonefile);
	}
    	foreach(object ob in obs) {	
	    if (!ob) {
		write("clone: "+clonefile+": Error cargando fichero.\n"); 
		continue;
	    }    
	    if (ob -> move(TU) != MOVE_OK) {
    		if (ob -> move(ENV(TU)) != MOVE_OK) {
		    write("Objeto $N"+ob->nombre?()+"$R clonado en memoria.\n");
		}
		else {
		    write("Objeto $N"+ob->nombre?()+"$R clonado en el suelo.\n");
		}
	    }
	    else {
		write("Objeto $N"+ob->nombre?()+"$R clonado en tu inventario.\n");
	    }
    
	    /* Disparamos el trigger ON_CLONE del objeto */
	    ob -> on_clone();
	}
    }
    return 1;
}
