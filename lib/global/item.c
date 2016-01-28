#include <mudlib.h>

inherit BASEOB;

private nosave string * razas_permitidas = ({ });

void create() {
    ::create();
}

void razas_permitidas(string * razas) {
    razas_permitidas = ({ })+razas;
}

int raza_permitida?(string raza) { 
    if ((!sizeof(razas_permitidas)) || (member_array(raza, razas_permitidas) != -1)) return 1;
    else return 0;
}

string * razas_permitidas?() { 
    return razas_permitidas;
}

