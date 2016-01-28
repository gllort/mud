#include <mudlib.h>
#include <macros.h>
#include <move.h>
#include <triggers.h>

inherit CONTENEDOR;

#define DEFAULT_RELATION "dentro de" /* En esta relacion entraran los living */

private nosave mapping salidas = ([ ]);

void habitacion() { ; }
void on_enter(object, string);
void on_leave(object, string);

void create() {
    ::create();
    nombre("HABITACION");
    descripcion("DESCRIPCION");

    /*
     * La masa de una habitacion debe ser  > que la capacidad de su relacion
     * por defecto. De este modo, una habitacion no puede clonarse dentro de
     * otra. Para un objeto como una lampara magica, que contenga dentro una 
     * habitacion, podemos reducir la masa de esta. 
     */
    volumen(100000001);

    crear_relacion(DEFAULT_RELATION, 100000000);

    add_trigger("on_enter", (: on_enter :));
    add_trigger("on_leave", (: on_leave :));

    habitacion();
}

void init() { ; }

void salida(string direccion, string fichero) {
    salidas[direccion] = fichero;
}

void borrar_salida(string direccion) {
    map_delete(salidas, direccion);
}

string * salidas?() { return keys(salidas); }

int existe_salida?(string direccion) { return (salidas[direccion] ? 1 : 0); }

void andar(string direccion) {
    mixed res;

    if ((res = TP -> move(salidas[direccion])) != MOVE_OK) {
	if (stringp(res)) write(res);	
    }
    TP->lugar_previo?() -> call_triggers("on_leave", TRIGGER_IGNORE, TP, direccion);
    ETP -> call_triggers("on_enter", TRIGGER_IGNORE, TP, direccion);
    ETP -> mirar();
}

string resolver_nombres(object living) { return capitalize(living->nombre?()); }

void mirar() {
    int size;
    array exits;
    object * living;
    
    if (TP->inmortal?()) write("["+file_name(TO)+"]\n");
    write(nombre?()+"$R\n\n");

    if (TP->detallado?())
	write(descripcion?()+"$R\n\n");

    exits = salidas?();
    if (exits == ({ })) {
	write("No parece haber salidas.\n");
    }
    else {
	if ((size = sizeof(exits)) > 1) {
	    exits = sort_array(exits, 1);
	    write("Puedes dirigirte a: "+implode(exits[0..size-2],", ")+" y "+exits[size-1]+".\n");
	}
	else {
	    write("Hay una unica salida: "+exits[0]+".\n");
	}
    }
    living = objetos(); /* Los living entran en la relacion por defecto */
    living -= ({ TP });
    size = sizeof(living);
    if (size == 1) write(capitalize(living[0]->nombre?()) + " esta aqui.\n");
    else if (size > 1) write(implode(map_array(living[0..size-2], "resolver_nombres", TO), ", ")+" y "+capitalize(living[size-1]->nombre?())+" estan aqui.\n");    



}

int room?() { return 1; }


void on_enter(object ob, string direccion) {
    string file;

    file = file_name(ob -> lugar_previo?());
    foreach(string salida in keys(salidas)) {
        if ((salidas[salida] == file) || (salidas[salida] == file+".c")) {
    	    tell_room(TO, capitalize(ob->nombre?())+" llega desde "+salida+".\n", ({ ob }));
	    return;
	}
    }
    tell_room(TO, capitalize(ob->nombre?())+" llega de alguna parte.\n", ({ ob }));
}

void on_leave(object ob, string direccion) {
    tell_room(TO, capitalize(ob->nombre?())+" se va hacia "+direccion+".\n");
}

