#include <comando.h>

int comando(int argc, string * argv) {
    int size;
    string * forma, * str_equipo;
    object * equipo;
    object * llevando;
    
    llevando = TP -> objetos(); /* Todos los objetos que lleva el jugador */
    if (!sizeof(llevando)) {
	write("No llevas nada encima.\n");
	return 1;
    }

    equipo = TP -> empu�ando?();
    if (sizeof(equipo) > 0) {
	write("Empu�ando: ");
	str_equipo = ({ });
	foreach(object item in equipo) {
	    str_equipo += ({ item -> nombre?() });
	    if (member_array(item, llevando) != -1) llevando -= ({ item });
	}
	size = sizeof(str_equipo);
	if (size == 1) write(str_equipo[0]+".\n");
	else write(implode(str_equipo[0..size-2],", ")+" y "+str_equipo[size-1]+".\n");     
    }    

    forma = TP -> forma?();
    foreach(string parte in forma) {
	equipo = TP -> vistiendo?(parte);
	if (!sizeof(equipo)) continue;
	write("* "+capitalize(parte)+": ");
	str_equipo = ({ });
	foreach(object item in equipo) {
	    str_equipo += ({ item->nombre?() });
	    if (member_array(item, llevando) != -1) llevando -= ({ item });
	}
	size = sizeof(str_equipo);
	if (size == 1) write(str_equipo[0]+".\n");
	else write(implode(str_equipo[0..size-2],", ")+" y "+str_equipo[size-1]+".\n");     
    }

    if (sizeof(llevando) > 0) {
        write("Cargando: ");
	str_equipo = ({ });
	foreach(object item in llevando) {
	    str_equipo += ({ item -> nombre?() }); 	    
	}
	size = sizeof(str_equipo);
	if (size == 1) write(str_equipo[0]+".\n");
	else write(implode(str_equipo[0..size-2],", ")+" y "+str_equipo[size-1]+".\n");     
    }
    write("\n");
    return 1;
}
