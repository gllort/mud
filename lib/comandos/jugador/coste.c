#include <comando.h>
#include <tablas.h>

int comando(int argc, string * argv) {
    
    write("Necesitas "+TABLA_COSTES->coste?(TP->nivel?()+1)+" puntos de experiencia para avanzar a nivel "+(TP->nivel?()+1)+".\n");
    return 1;
}
