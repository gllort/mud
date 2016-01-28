#include <daemons.h>
#include <mudlib.h>
#include <dirs.h>

inherit DAEMON;

varargs mixed protected_call(object doer, mixed ob, mixed fun, mixed arg1, mixed arg2, mixed arg3, mixed arg4, mixed arg5) {
    string destfile;
    int valid = 1, call_manual = 0;
    object * obs;
    
    if ((adminp()) || (file_name(doer) == PERMISOS) || (file_name(doer) == OB_PERMISOS))
        return unguarded_call(ob, fun, arg1, arg2, arg3, arg4, arg5);
    
    obs = APO;
    foreach(object o in obs) {
	if ((TP) && (subdir(file_name(o), DIR_CMD)) && (relative_name(o) == "call")) {
//	    printf("[call-spy] %O %O %O\n", TP, fun[0], ob);	
	    call_manual = 1;
	    break;
	}
    }
    if (objectp(ob)) destfile = file_name(ob);
    else destfile = ob;

    if (call_manual) {
//	valid = unguarded_call(PERMISOS, ({ "call_access?", TP, destfile }) );
	valid = 1;
    }
    if (valid)
        return unguarded_call(ob, fun, arg1, arg2, arg3, arg4, arg5);
    else {
//	write("call: "+destfile+": Acceso denegado.\n");
	return 0;
    }
}

