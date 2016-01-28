#include <mudlib.h>
#include <daemons.h>
#include <errno.h>

#include <room.h>

void init() {
    add_action("ayuda", "ayuda");
    add_action("crear", "crear");
    add_action("listar", "listar");
    add_action("eliminar", "eliminar");
    add_action("miembros", "miembros");
}

void habitacion() {
    nombre("$NTorre del Destino: $4Control de Rangos de Inmortales$R");
    descripcion("Esta es la sala de Control de Rangos de Inmortales. Desde aqui,\n"+
		"un administrador puede manipular la jerarquia de rangos de inmortales:\n"+
		"crear y eliminar rangos, y modificar los niveles y responsabilidades\n"+
		"de cada uno.\n\n"+
		"Escribe 'ayuda' para mostrar los comandos disponibles de la sala.");

    salida("sur", "/dominios/cielo/comun.c");
}

int ayuda() {
    write("ayuda                 - Muestra este texto de ayuda.\n");
    write("crear [rango] [nivel] - Crea un nuevo rango de inmortales.\n");
    write("eliminar [rango]      - Elimina el rango de inmortales indicado.\n");
    write("listar                - Lista los rangos de inmortales definidos.\n");
    write("miembros [rango]      - Lista los inmortales asignados a ese rango.\n");
    write("\n");
    write("Para asignar o deasignar usuarios a un rango, usa los comandos\n"
          "promote y demote respectivamente.\n");    
    return 1;
}

int listar() {
    int * niveles; 
       
    niveles = RANGOS -> niveles?();
    if (niveles == ({ })) {
	notify_fail("No hay rangos definidos! Esto nunca debe ocurrir.\n");
	return 0;
    }
    niveles = sort_array(niveles, -1);
    foreach(int nivel in niveles) {
	write(capitalize(RANGOS->rango(nivel))+"\t"+nivel+"\n");
    }
    return 1;
}

int crear(string str) {
    string rango, err;
    int nivel, gid;
    
    if ((!str) || (str=="") || (sscanf(str, "%s %d", rango, nivel) != 2)) {
	notify_fail("Sintaxis: crear [rango] [nivel]\n");
	return 0;
    }
    if (err = catch(gid = RANGOS -> crear(rango, nivel))) {
	notify_fail(serror(err)+"\n");
	return 0;
    }
    write("Rango '"+capitalize(lower_case(rango))+"' creado con GID '"+gid+"' y nivel '"+nivel+"'.\n"); 
    return 1;
}

int eliminar(string str) {
    string err;
    
    if ((!str) || (str=="")) {
	notify_fail("Sintaxis: eliminar [rango]\n");
	return 0;
    }
    if (err = catch(RANGOS -> eliminar(str)))
	write(serror(err)+"\n");
    else 
	write("Rango '"+str+"' eliminado.\n");
    return 1;
}

int miembros(string str) {
    string * miembros, err;
    
    if (!str || str=="") {
	notify_fail("Sintaxis: miembros [rango]\n");
	return 0;
    }
    if (!RANGOS -> es_rango?(str)) {
	notify_fail("No existe el rango '"+str+"' de inmortales.\n");
	return 0;
    }
    if (err = catch(miembros = GRUPOS -> miembros(str))) {
	notify_fail(serror(err)+"\n");
	return 0;
    }    
    if (!sizeof(miembros)) write("Ningun inmortal esta asignado a ese rango.\n");
    else write(implode(miembros, "\n")+"\n");
    return 1;
}
