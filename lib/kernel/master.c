#include <log.h>
#include <daemons.h>
#include <mudlib.h>
#include <parser_errno.h>

inherit __DIR__ "valid.c";

object connect() {
    string err;
    object new_user;

    if (err = catch(new_user = clone_object(LOGIN)) || !new_user) {
	if (!find_file(LOGIN+".c")) err = "El fichero "+LOGIN+" no existe.\n";
	write("\nmaster: Error cargando el objeto login.\n"+err);
	return 0;
    }
    else {
	/* Filtrar conexiones, logs, ... */
	return new_user;
    }
}

object compile_object(string path) {
    return 0;
}

/* Error Handling 
 * Codigo original de las librerias Lima.
 * Modificado por Cain@MundoOscuro 10/Jul/2003
 */ 

string linea_traza(object obj, string prog, string file, int line) {
    string trace;
    
    trace = obj ? file_name(obj) : "<no object>";
    if (explode(trace, "#")[0]+".c" != prog) {
        trace += " - Programa: "+prog;
    }
    if (prog != file) {
	trace += " - Fichero: "+file;
    } 
    trace += " - Linea: "+line;
    return "Objeto: "+trace;
}

varargs string traza(mapping error, int flag) {
    string str;
    int i, size;
    mapping trace;
    
    str = ctime(time());
    str += "\n";
    str += error["error"] + linea_traza(error["object"], error["program"], error["file"], error["line"]);
    str += "\n";
    trace = error["trace"];

    size = sizeof(trace);
    for (i=0; i<size; i++) {
	if (flag) str += sprintf("#%d: ", i);
	str += sprintf("Funcion '%s' en %s", trace[i]["function"], linea_traza(trace[i]["object"], trace[i]["program"], trace[i]["file"], trace[i]["line"])+"\n");	
    }
    /* Contexto detallado del error */
    str += sprintf("\n-- Context --\n%O\n", error);
    return str;
}

void error_handler(mapping error, int caught) {
    string trace, errmsg, log;

    errmsg = error["error"];

    /* Log trace */
    log = (caught ? LOG_CATCH : LOG_RUNTIME);
    write_file(log, traza(error,1)+"\n******************\n");    

    /* Los errores de compilacion se muestran aparte */
    if (errmsg[0..23] == "*Error in loading object") return;

//    errmsg = replace_string(errmsg[0..<2], "\n", "\n-> ");
    write(errmsg+"["+linea_traza(error["object"], error["program"], error["file"], error["line"])+"]\n\n");
}


/* Errores de Compilacion */
void log_error(string file, string message) {
    write(message);
    /* Logging del error */
}

void crash(string message, object cmd_giver, object current_obj) {
    foreach(object user in users()) {
       tell_object(user, "MudOS grita: Nos vamos a pique!\n");
       user -> quit();
    }
}

string * epilog(int load_empty) {
    return ({ INITD });
}

void preload(string filename) {
    load_object(filename);
}

string object_name(object ob) {
    string name;

    name = efun::call_other(ob, "nombre?");
    if (!name || name=="") name = file_name(ob);
    return name;
}

int view_errors(object user) {
    if (inherits(INMORTAL, user)>0) return 1;
    else return 0;
}

string * parse_command_id_list() {
    return ({ "cosa" });
}

string * parse_command_adjectiv_id_list() {
    return ({ });
}

string * parse_command_plural_id_list() {
    return ({ "cosas", "todo" });
}

string parse_command_all_word() { 
    return "todo";
}


void refresh_parser() {
    parse_refresh();
}


object * parse_command_users() {
    return users();
}

string * parse_command_prepos_list() {
    return ({ "a","ante","bajo","cabe","con","contra","de","desde","en", 
	      "entre","hacia","hasta","para","por","segun","si","sobre",
	      "tras", "at"  });
}

string parser_error_message(int error, object ob, mixed args, int plural) {
    switch(error) {
	case PARSE_NOT_HERE:
	    printf("error: %O\n",error);
	    printf("ob: %O\n",ob);
	    printf("args: %O\n",args);
	    printf("plural: %O\n",plural);

	    return capitalize(args)+" no esta por aqui.\n";

	case PARSE_NOT_ALIVE:
	    if (plural) 
		return "pluralize(arg) no estan vivos.\n";
	    else
		return capitalize(args)+" no esta vivo.\n";
	case PARSE_UNACCESSIBLE:
	    if (plural) 
		return "Estan fuera de tu alcance.\n";
	    else
		return "Esta fuera de tu alcance.\n";
	case PARSE_AMBIGUOUS:
	    return "A cual de ellos te refieres?\n";
	case PARSE_WRONG_NUMBER:
	    return "No tienes suficientes.\n";
	case PARSE_ALLOCATED:
	    if ((strlen(args)>=8) && (args[0..8] == "You can't")) return 0;
	    else return args;

	case PARSE_NOT_FOUND:
	    printf("error: %O\n",error);
	    printf("ob: %O\n",ob);
	    printf("args: %O\n",args);
	    printf("plural: %O\n",plural);

	    return "No hay ningun "+args+" por aqui.\n";
	case PARSE_TOO_MANY:
	    return "No puedes hacer eso con tantas cosas a la vez.\n";
    }
}

/*
string privs_file(string file) {
    object ob;
    
    ob = find_object(file);
    if ((inherits(DAEMON, ob)) || (inherits(INMORTAL, ob))) return "";
    return PERMISOS -> owner_uid?(file) + " " + PERMISOS -> rank_access?(file);
}
*/
