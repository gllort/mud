#include <comando.h>


int comando(int argc, string * argv) {
    object ob;
    
    ob = clone_object("/comandos/jugador/b.c");
    ob -> pruebaguay();
    return 1;
}

