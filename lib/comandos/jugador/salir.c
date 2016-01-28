#include <comando.h>

void confirmar_salida(string);

int comando(int argc, string * argv) {
    if (!TP -> guardar()) {
	write("Salir de todos modos? (s/n): ");
	input_to("confirmar_salida");
    }
    else TP -> salir();        
    return 1;
}

void confirmar_salida(string opcion) {
    opcion = lower_case(opcion);
    if ((opcion == "s") || (opcion == "si") || (opcion == "y") || (opcion == "yes")) {
	TP -> salir();
    }
}
