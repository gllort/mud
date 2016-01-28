#include <comando.h>
#include <errno.h>

int comando(int argc, string * argv) {
    object me;
    string err;

    if (argc != 2) {
	notify_fail("Sintaxis: demote [usuario] [motivo]\n");
	return 0;
    }
    me = TP;
    err = catch(USERS -> demote(argv[0], argv[1]));    
    if (err) {
	notify_fail("demote: "+serror(err)+"\n");
	return 0;
    }
    tell_object(me, capitalize(argv[0])+" ya no es un inmortal.\n");
    return 1;
}
