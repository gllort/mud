#include <mudlib.h>
#include <daemons.h>
#include <errno.h>
#include <dirs.h>
#include <permisos.h>

inherit JUGADOR;

private nosave int inmortal_level;

private nosave string cwd;

void inicializar() {
    string err;

    /* Recuperar el nivel de inmortal de la BD */
    if (err = catch(inmortal_level = RANGOS -> nivel_inmortal(nombre?()))) {
	write(serror(err)+"\n");
	write("Error inicializando objeto inmortal.\n");
	destruct(TO);
    }

//    set_privs(TO, TO->nombre?() + " " + inmortal_level);

    ::inicializar();
}	

int inmortal?() { return 1; }

int nivel_inmortal?() { return inmortal_level; }

/*
string process_input(string input) {
    return ::process_input();
}
*/

/*
    else {
	grp = ({ }) + grupos;
	grp -= ({ RANGOS -> rango(rank) });
	k = sizeof(grp);
	for (int i=0; i<k; i++) {
	    if (find_file(DIR_CMD+"/"+grp[i]+"/"+verbo+".c")) {
    		cmd = load_object(DIR_CMD+"/"+rank+"/"+verbo+".c");
		argv = explode(argumentos, " ");
		argc = sizeof(argv);	    
        	return cmd -> comando(argc, argv);
	    }
	}    
    }
*/

int cargar_ficha() {
    string chan;

    if (!::cargar_ficha()) return 0;

    /* Si nos acaban de hacer inmortales o estabamos desconectados, no tenemos
     * agregado el canal de nuestro rango. Lo anyadimos ahora.
     */ 
    if (!acceso_al_canal?((chan = RANGOS -> rango(inmortal_level)))) {
	if (!add_channel(chan)) {
	    write("Error agregando canal "+capitalize(chan)+".\n");
	    return 0;
	}
    }
    return 1;
}

int cwd(string nwd) {
    string * dirs;
    dirs = explore_path(nwd, 1);

    if (dirs == ({ })) {
	notify_fail("cd: No existe el directorio '"+nwd+"'.\n"); 
	return 0;
    }
    if (PERMISOS -> read_access?(TP, dirs[0])) {
	cwd = dirs[0];
	return 1;
    }
    else {
	notify_fail("cd: "+dirs[0]+": Acceso denegado.\n");
        return 0;
    }
}

string cwd?() { 
    if (!cwd) {
	if (find_dir(DIR_HOME+"/"+TP->nombre?())) cwd = DIR_HOME+"/"+TP->nombre?();
	else cwd = "/";
    }
    return cwd; 
}
