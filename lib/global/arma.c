#include <mudlib.h>
#include <tablas.h>

inherit ITEM;

private nosave int * tipo_daño = ({ });
private nosave int espacio_que_ocupa = 1;
private nosave int multiplicador = 1;
private nosave int critico = 20;
private nosave int dado = 1, tiradas = 1;
private nosave mapping mensajes_combate = ([ ]);

void arma() { ; }

void create() {
    mixed * msgs_basicos;

    ::create();
    arma();

}

void espacio_que_ocupa(int espacio) { espacio_que_ocupa = espacio; }

int espacio_que_ocupa?() { return espacio_que_ocupa; }

void daño(int rolls, int dice) {
    tiradas = rolls;
    dado = dice; 
}

string daño?() { return tiradas+"d"+dado; } 
int tiradas?() { return tiradas; }
int dado?() { return dado; }

void multiplicador(int factor) { multiplicador = factor; }
int multiplicador?() { return multiplicador; }

void critico(int valor) { critico = valor; }
int critico?() { return critico; }

void tipo_daño(int * tipos) { 
    foreach(int tipo in tipos) {
        if ((member_array(tipo, tipo_daño) == -1) && (TABLA_DAÑOS -> tipo_daño_valido?(tipo))) {
	    tipo_daño += ({ tipo });
	}
    }
}
string * tipo_daño?() { return tipo_daño; }

void debug() {
    printf("%O\n", mensajes_combate);
}
