#include <comando.h>
#include <daemons.h>

int comando(int argc, string * argv) {
    string canal, texto;

    if (argc < 2) {
	notify_fail("Sintaxis: canal [canal] [texto]\n");
	return 0;
    }    
    canal = argv[0];
    texto = implode(argv[1..], " ");

    switch(texto) {
	case "on" : TP -> open_channel(canal);
		    break;
	case "off": TP -> close_channel(canal);
		    break;
	default   : TP -> send_channel(canal, texto);
		    break;
    }
    return 1;
}
