#include <mudlib.h>
#include <gennum.h>
#include <bonus.h>

inherit ITEM;

private nosave string * partes_que_ocupa = ({ });
private nosave int ac, max_mod_des = MAXIMO_MODIFICADOR_DESTREZA;

void armadura() { ; }

void create() {
    ::create();
    genero(FEMENINO);
    armadura();
}

void partes_que_ocupa(string * partes) {
    partes_que_ocupa = ({ })+partes;
}

string * partes_que_ocupa?() { return partes_que_ocupa; }

void ac(int x) { ac = x; }
int ac?() { return ac; }

void max_mod_des(int x) { 
    if (x > MAXIMO_MODIFICADOR_DESTREZA) max_mod_des = MAXIMO_MODIFICADOR_DESTREZA;
    else max_mod_des = x; 
}
int max_mod_des?() { return max_mod_des; }
