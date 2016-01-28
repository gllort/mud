#include <mudlib.h>
#include <dirs.h>

#include <room.h>

string * razas;

void init() {
    add_action("ser", "ser");
    add_action("ver", "ver");
}

void habitacion() {
    int i, size, err;
    string * desc = ({ }), str;

    nombre("Eleccion de raza");
    
    
    err = catch(razas = get_dir(DIR_RAZAS+"/*.c"));
    if(err) size = 0;
    else size = sizeof(razas);

    for (i=0; i<size; i++) desc += ({ capitalize(lower_case(explode(razas[i],".")[0])) });
    size = sizeof(desc = sort_array(desc, 1));
    if (!size) str = "En este momento no hay razas disponibles.";
    else if (size == 1) str = "La unica raza disponible es: "+desc[0]+".";
    else {
	str = "Puedes escoger entre las siguientes razas: ";
	str += implode(desc[0..size-2], ", ") + " y "+desc[size-1]+".";
    }

    descripcion("Sala de eleccion de raza.\n\n"
	     	+str+"\n\n"
		"Escribe: ser <raza>.");
}

int ser(string raza) {
    if (!raza || raza=="") {
	notify_fail("Sintaxis: ser [raza]\n");
	return 0;
    }
    raza = lower_case(raza);
    if (member_array(raza+".c", razas) != -1) {
	TP -> encarnar(raza);
    }
    else {
	write("Esa raza no existe!\n");
    }
    return 1;
}

int ver(string raza) {
    object ob;

    if (!raza || raza=="") {
	notify_fail("Sintaxis: ver [raza]\n");
	return 0;
    }
    raza = lower_case(raza);
    if (member_array(raza+".c", razas) != -1) {
	ob = load_object(DIR_RAZAS+"/"+raza+".c");
	if (!ob) {
	    write("raza: "+capitalize(raza)+": Error cargando la raza.\n");
	}
        else {
	    write("ob->descripcion()\n");
	}
    }
    else {
	write("Esa raza no existe!\n");
    }
    return 1;
}
