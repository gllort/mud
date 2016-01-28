#include <comando.h>
#include <daemons.h>

int comando(int argc, string * argv) {

    if (argc != 2) {
	notify_fail("Sintaxis: upload [origen] [destino]\n");
	return 0;
    }


    //DOMINIOS->domainlord?( **del dominio** ) != TP->nombre?()
    
    return 1;
}
