#include <daemons.h>

varargs mixed call_other(mixed ob, mixed fun, mixed arg1, mixed arg2, mixed arg3, mixed arg4, mixed arg5, mixed arg6) {
    object doer;
    mixed funargs = ({ });
/*
    if (inmortalp()) doer = TP;
    else doer = PO(0);
*/
    doer = PO(0);
    
    /* Estandarizacion de los multiples formatos que acepta efun::call_other */
    if (arrayp(fun)) funargs = fun;
    else funargs += ({ (string)fun, arg1, arg2, arg3, arg4, arg5, arg6 });

    if (origin() == "local") return efun::call_other(ob, funargs); 
    else return efun::call_other(CALLER, "protected_call", doer, ob, funargs);
}






























/* 
varargs mixed call_other(mixed ob, mixed fun, mixed arg1, mixed arg2, mixed arg3, mixed arg4, mixed arg5) {
    object doer;
    mixed * obs = ({ }), * funargs = ({ });

    if (efun::call_other(TP, "es_inmortal")) doer = TP;
    else doer = PO(0);
    obs = (mixed *)( arrayp(ob) ? ob : ({ ob }) );
    funargs = (mixed *)( arrayp(fun) ? fun : ({ (string)fun, arg1, arg2, arg3, arg4, arg5 }) );

    return efun::call_other(ob, fun, arg1, arg2, arg3, arg4, arg5);

    if (file_name(PO(0)) == "/comandos/jugador/p") {    
	printf("%O\n%O\n", obs, funargs);
        return efun::call_other(obs, funargs);
    }

    return efun::call_other(ob, fun, arg1, arg2, arg3, arg4, arg5);


    return efun::call_other(obs, funargs);
/*
    if (file_name(doer)+".c" == PERMISOS) return efun::call_other(obs, funargs);
    size = sizeof(obs);
    for (i=0; i<size; i++) {
	object dest;
        file = 0;
	dest = ( objectp(obs[i]) ? obs[i] : load_object(obs[i]) );
	if (!dest) { result += ({ 0 }); continue; }
	obj = dest;
        while(shadow = shadow(obj, 0)) {
    	    obj = shadow;
	    file = function_exists(fun, obj);
	}
	if (!file) file = function_exists(fun, dest);
	if (file) {
	    if (efun::call_other(PERMISOS, "call_access", doer, explode(file_name(dest),"#")[0]+".c")) {
		result += ({ efun::call_other(dest, funargs) });
	    }
	    else result += ({ 0 });
	}
	else {
	    result += ({ 0 });
	}	
    }
    if (size == 1) return result[0];
    else return result;
*/


