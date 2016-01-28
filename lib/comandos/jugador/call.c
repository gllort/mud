#include <comando.h>
#include <errno.h>

/* REVISAR COMMENTS 
 * -> usar los arrays de argumentos [0..]
 */
 
object * present?(string);

int comando(int argc, string * argv) {
    string args, fargs, destobs, funcion, * objetos;
    object retob, * callob = ({ }), caller;
    mixed * argumentos;
    int i, k, size;
        
    args = implode(argv, " ");

/* Esto se jode en caso de call (({ asdf }))me -> pilla )me */
    if (sscanf(args, "%s(%s)%s", funcion, fargs, destobs) != 3) {
	notify_fail("Sintaxis de mierda.\n");
	return 0;
    }

    while(destobs[0] == ')') {
	fargs += ")";
	destobs = destobs[1..strlen(destobs)-1];
    }
//    fargs = replace_string(fargs, "\"", "\\\"");

    argumentos = explode(fargs, ",");
    
    if (sizeof(argumentos) < 8) argumentos += allocate(8 - sizeof(argumentos));

    objetos = explode(destobs, ",");
    size = sizeof(objetos);
    for (i=0; i<size; i++) {
	callob += present?(objetos[i]);
    }
    if (callob == ({ })) {
	if (size > 1) 
    	    notify_fail("call: Los objetos '"+implode(objetos[0..size-2],", ")+" y "+objetos[size-1]+"' no estan en memoria.\n");
	else 
    	    notify_fail("call: El objeto '"+destobs+"' no esta en memoria.\n");
	return 0;
    }

    size = sizeof(callob);
    for (i=0; i<size; i++) {
	int found = 0;
	object shadow = callob[i];
	
	/* Buscamos funciones shadowed */ 
	while (shadow = shadow(shadow, 0)) {
	    if (function_exists(funcion, shadow)) {
		found = 1;	
    	        callob[i] = shadow;
	    }
	}
	if ((found) || ((!found) && (function_exists(funcion, callob[i])))) {
	    string str, err;

	    caller = TP;

	    for(k=0; k<sizeof(argumentos); k++) {
		/* TO_INTEA Posibles INTEGERS */
		/* Anyadir q para poner una STR explicita con "" */ 
		argumentos[k] = trim(argumentos[k]);
		if ((!argumentos[k]) || (argumentos[k] == "") || (argumentos[k] == "0")) argumentos[k] = 0; 
		else if (to_int(argumentos[k]) != 0) argumentos[k] = (int)to_int(argumentos[k]);	    
	    }

	    /* Describir mejor el objeto destino.. funciones short.. name.. */
	    err = catch(retob = call_other(callob[i], funcion, argumentos[0], argumentos[1],argumentos[2],argumentos[3],argumentos[4],argumentos[5],argumentos[6],argumentos[7]));
	    if (err) {
		write("call: "+serror(err)+"\n");	    	    
	    }
	    else {
    		str = (found ? " shadowed " : " ");
		tell_object(caller, "call: Funcion"+str+"'"+funcion+"' encontrada en "+file_name_oid(callob[i])+".\n");
		tell_object(caller, sprintf("Valor de retorno: %O\n", retob));
	    }	
	}
	else {
	    write("call: Funcion '"+funcion+"' no encontrada en "+file_name_oid(callob[i])+".\n");
	}
    }
    return 1;
}

object * present?(string str) {
    object ob;
    if (ob = find_living(str)) return ({ ob });
    if ((str == "me") || (str == "yo")) return ({ TP });
    if ((str == "here") || (str == "aqui")) return ({ ETP });
    if ((str == "everyone") || (str == "todos")) return users();
    if (ob = find_object(str)) return ({ ob });
    return ({ });
}
