#include <comando.h>

int comando(int argc, string * argv) {
    int i, size, listados = 0;
    object * obs;
    string pattern, fn, str;

    if (argc) {
	pattern = implode(argv,"|");
	str = " segun el patron '"+pattern+"'.\n";
    }
    else {
	pattern = ".";
	str = ".\n";
    }
    size = sizeof(obs = objects());
    for (i=0; i<size; i++) {
	fn = file_name_oid(obs[i]);
        if (regexp(fn, pattern)) {
	    listados++;
	    write(fn+" "+query_privs(obs[i])+"\n");
	}
    }
    if (!listados) 
	write("Ningun objeto en memoria"+str);
    else if (listados == 1)
	write(listados+" objeto listado"+str);
    else 
	write(listados+" objetos listados"+str);
    return 1;
}
