#include <comando.h>

int comando(int argc, string * argv) {
    string * canales;

    canales = TP -> canales?();
    if (!sizeof(canales)) {
	write("No puedes hablar por ningun canal.\n");
    }    
    else {
        write("Puedes hablar en los siguientes canales:\n");
	write(implode(canales, "\n")+"\n");
    }
    return 1;
}
