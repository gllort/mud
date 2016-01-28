#include <dirs.h>
#include <mudlib.h>
#include <move.h>

inherit PERSONAJE;
inherit __DIR__ "/jugador/shell.c";
inherit __DIR__ "/jugador/canales.c";
inherit __DIR__ "/jugador/history.c";

void create() {
    ::create();
}

void inicializar() {
    set_heart_beat(1);
    set_living_name(nombre?());
    enable_commands();
    master() -> refresh_parser();
}

int jugador?() { return 1; }
int nivel_inmortal?() { return 0; }

int crear_ficha(int sexo, string raza, string clase) { 
    mixed res;

    if (!find_dir(DIR_FICHAS+"/"+nombre?()[0..0])) {
	if (!mkdir(DIR_FICHAS+"/"+nombre?()[0..0])) {
	    write("Error creando directorio de fichas.\n");
	    return 0;
	}
    }

    if (!save_object(DIR_FICHAS+"/"+nombre?()[0..0]+"/"+nombre?()+".o")) {
	write("Error creando ficha.\n");
	return 0;
    }

    if ((res=move("/global/room/entrada.c")) != MOVE_OK) {
	if (stringp(res)) write(res);
        write("Error cargando el entorno del jugador.\n");
        return 0;
    }
    
    add_command("ajustar", "/global/comandos/ajustar.c");
    
    sexo(sexo);
    raza(raza);
    clase(clase);
    
    return 1;
}

int cargar_ficha() { 
    mixed res;
    string * clases;
    
    if (!restore_object(DIR_FICHAS+"/"+nombre?()[0..0]+"/"+nombre?()+".o", 1)) {
	write("Error restaurando ficha.\n");
	return 0;
    }
    /* 
     * Movemos al jugador a su ultima habitacion.
     */
    if ((res=move(ultimo_lugar?())) != MOVE_OK) {
	if (stringp(res)) write(res);
	if (move(VOID) != MOVE_OK) {
	    write("Error cargando el entorno del jugador.\n");
	    return 0;
	}
    }
    join_all(); /* Abrimos todos los canales OPEN */

    if (!cargar_raza()) return 0;

    if (!cargar_clase()) return 0;

    if (!cargar_gremio()) return 0;

    return 1;
}

varargs int guardar(int silencio) {
    int ok = save_object(DIR_FICHAS + "/" + nombre?()[0..0] + "/" + nombre?() + ".c");
    if ((ok)&&(!silencio)) write("Ficha grabada.\n");
    else if ((!ok)&&(!silencio)) write("Error grabando ficha.\n");
    return ok;
}

void salir() {
    write("Gracias por jugar!\n");
    destruct(TO);
}

	