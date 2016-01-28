#include <comando.h>
#include <errno.h>

int comando(int argc, string * argv) {
    object me;
    string err, str;

    if (argc != 2) {
	notify_fail("Sintaxis: promote [usuario] [rango]\n");
	return 0;
    }
    me = TP;
    err = catch(USERS -> promote(argv[0], argv[1]));    
    if (err) {
	notify_fail("promote: "+serror(err)+"\n");
	return 0;
    }

    /* Debe ser un tell_object a me porque el promote puede hacer
     * un exec que mueve el contexto actual al del jugador que va
     * a ser promoteado 
     */  
    tell_object(me, sprintf("%s ha sido ascendido a %s.\n", capitalize(argv[0]), capitalize(argv[1])));
    return 1;
}
