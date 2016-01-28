#include <mudlib.h>
#include <dirs.h>
#include <room.h>

private nosave object gremio;

void create() {
    ::create();
}

void init() {
    add_action("alistarme", "alistarme");
    add_action("aprender", "aprender");
    
}

void habitacion() {
    nombre("Habitacion del Gremio");
}

void gremio(string guild) {
    string err;
    
    if (!find_file(guild)) {
	if (!find_file(DIR_GREMIOS+"/"+guild+".c")) {
	    write("Ese gremio no existe!\n");
	    destruct(TO);
	    return;
	}
	else {
	    guild = DIR_GREMIOS+"/"+guild+".c";
	}
    }
    err = catch(gremio = load_object(guild));
    if ((err) || (!gremio)) {
	write("Error cargando habitacion del gremio "+guild+"\n");
	if (err) write(err+"\n");
	destruct(TO);
	return;
    }
    if (!inherits(GREMIO, gremio)) {
	write("El objeto "+guild+" no es un gremio!\n");
	destruct(gremio);
	destruct(TO);
	return;
    }
}

int alistarme() {
    if (!gremio) {
	notify_fail("El gremio no esta inicializado!\n");
	return 0;
    }
    if (gremio->acceso_permitido?()) {
	/* Alistamos el jugador al gremio */

	TP -> gremio(file_name(gremio)+".c");
    }
    return 1;
}

int aprender(string args) {
    string * comandos;

    if (!gremio) {
	notify_fail("El gremio no esta inicializado!\n");
	return 0;
    }
    comandos = keys(gremio->comandos_nivel(TP -> nivel?() - TP -> nivel_alistamiento?()));

    if (!args) {
	if (!sizeof(comandos)) 
	    write("No estas preparado para aprender nada nuevo.\n");
	else	
	    write("Puedes aprender los siguientes comandos: \n"+implode(comandos,"\t")+"\n");
    }
    else {
	if (member_array(args, comandos) == -1)
	    write("No puedes aprender eso.\n");
	else
	    write("Vas a aprender el comando "+args+"... XP & Money checks!\n");
    }
    return 1;
}
