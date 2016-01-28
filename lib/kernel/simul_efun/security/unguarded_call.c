#include <daemons.h>

mixed unguarded_call(mixed ob, mixed funargs) {
    
    if ((origin() != "simul") || (file_name(PO(0)) != CALLER)) {
	//Acceso denegado.    
	write("call: Acceso denegado.\n");
	return 0;
    }
    return efun::call_other(ob, funargs);
}
