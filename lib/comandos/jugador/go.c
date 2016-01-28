#include <comando.h>
#include <move.h>

int comando(int argc, string * argv) {
    object ob;
    mixed dest;
    string err;

    if (argc != 1) {
	notify_fail("go: Parametros incorrectos: go [jugador | habitacion]\n");
	return 0;
    }
    argv[0] = lower_case(argv[0]);
    ob = find_player(argv[0]);
    if (ob == TP) return 1;
    if (ob) {
	dest = environment(ob);
	if (dest == ETP) return 1;
    }
    else {
	string * paths = explore_path(argv[0], 2);
	if (sizeof(paths)) dest = paths[0];
	else {
    	    notify_fail("go: No existe el destino '"+argv[0]+"'.\n");
	    return 0;
	}
    }
    if (TP->move(dest) != MOVE_OK) {
	write("go: Error moviendote al destino.\n");
    }
    return 1;
}
