#include <dirs.h>
#include <daemons.h>

private nosave int mensajes_abreviados = 0;	/* brief / verbose */ 

int nivel_inmortal?(); /* Extern */

nomask int parsear_comando(string input) {
    mixed res;
    object cmd;
    int lvl, argc;
    string rank, verbo, argumentos, * argv;

    if (!input || input=="") return 0;
    
    if (sscanf(input, "%s %s", verbo, argumentos) < 2) {
        verbo = input; argumentos = "";
    }

    /* Miramos si es una direccion */
    load_object(DIR_CMD+"/"+DIR_VERBOS+"/ir.c");
    if (verbo != "ir") {    
	if ((res = parse_sentence("ir "+input)) == 1) return 1;
    }
    else {
	if ((res = parse_sentence(input, 1)) == 1) return 1;
	else if (res == 0) {
	    notify_fail("No hay salida en esa direccion.\n");
	    return 0;
	}
    }
    if (stringp(res)) { notify_fail(res); return 0; }

    /* Miramos si es cualquier verbo */
    if (find_file(DIR_CMD+"/"+DIR_VERBOS+"/"+verbo+".c")) {
	load_object(DIR_CMD+"/"+DIR_VERBOS+"/"+verbo+".c");
	if ((res = parse_sentence(input)) == 1) return 1;
        if (stringp(res)) { notify_fail(res); return 0; }
    }

    /* Miramos si es un comando limitado por rango */
    lvl = nivel_inmortal?();
    for (int i=lvl; i>=0; i=RANGOS -> inferior(i)) {
        rank = RANGOS -> rango(i); 
        if (find_file(DIR_CMD+"/"+rank+"/"+verbo+".c")) {
	    cmd = load_object(DIR_CMD+"/"+rank+"/"+verbo+".c");
	    argv = explode(argumentos, " ");
	    argc = sizeof(argv);	    
	    return cmd -> exec_command(argc, argv); 
	}
	else if (i == 0) break;
    }        
    return 0;
}

string process_input(string input) {
    if ((TP) && ((parsear_comando(input)) || (TP -> ejecutar_comando(input)))) {
        /* Historial */
        TP -> add_history(input);
	return "";
    }
}

void abreviado() { mensajes_abreviados = 1; }
void detallado() { mensajes_abreviados = 0; }
int abreviado?() { return mensajes_abreviados; }
int detallado?() { return mensajes_abreviados ? 0 : 1; }

