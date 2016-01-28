#include <mudlib.h>
#include <daemons.h>
#include <errno.h>

#include <room.h>


void init() {
    add_action("ayuda", "ayuda");
    add_action("grupos", "grupos");
    add_action("crear", "crear");
    add_action("eliminar", "eliminar");
    add_action("asignar", "asignar");
    add_action("deasignar", "deasignar");
    add_action("miembros", "miembros");
}

void habitacion() {
    nombre("$NTorre del Destino: $4Control de Grupos de Trabajo$R");
    descripcion("Esta es la sala de Control de Grupos de Trabajo. Desde aqui,\n"+
		"un administrador puede crear y eliminar grupos de trabajo, y\n"+ 
		"asignar o deasignar miembros a cada grupo.\n\n"+
		"Escribe 'ayuda' para mostrar los comandos disponibles de la sala.");
    salida("este", "/dominios/cielo/comun.c");
}

int ayuda() {
    write("ayuda                 - Muestra este texto de ayuda.\n");
    write("grupos [usuario]      - Lista todos los grupos de trabajo, o los de un usuario.\n");
    write("crear [grupo]         - Crea un nuevo grupo de trabajo.\n");
    write("eliminar [grupo]      - Borra un grupo de trabajo.\n");
    write("miembros [grupo]      - Lista los miembros de un grupo de trabajo.\n");
    write("asignar [inmortal] [grupo]   - Asigna un inmortal al grupo de trabajo.\n");
    write("deasignar [inmortal] [grupo] - Borra al inmortal del grupo de trabajo.\n");
    return 1;

}

int grupos(string user) {
    string * grupos;
    grupos = GRUPOS -> grupos?(user);
    if (grupos == ({ })) {
	if (!user) write("No hay ningun grupo de trabajo definido.\n");
	else write("El usuario '"+capitalize(user)+"' no es miembro de ningun grupo.\n");
    }
    else {
	write(implode(sort_array(grupos, 1), "\n")+"\n");
    }
    return 1;
}

int crear(string str) {
    string rango, err;
    int nivel, gid;
    
    if ((!str) || (str=="")) {
	notify_fail("Sintaxis: crear [grupo]\n");
	return 0;
    }
    if (err = catch(gid = GRUPOS -> crear(str))) {
	notify_fail(serror(err)+"\n");
	return 0;
    }
    write("Grupo de trabajo '"+capitalize(lower_case(str))+"' creado con GID '"+gid+"'.\n"); 
    return 1;
}

int eliminar(string str) {
    string err;
    
    if ((!str) || (str=="")) {
	notify_fail("Sintaxis: eliminar [grupo]\n");
	return 0;
    }
    if (RANGOS -> es_rango?(str)) {
	notify_fail("Hazlo desde la Sala de Gestion de Rangos.\n");
	return 0;
    }
    if (err = catch(GRUPOS -> eliminar(str)))
	write(serror(err)+"\n");
    else 
	write("Grupo de trabajo '"+str+"' eliminado.\n");	
    return 1;
}

int miembros(string str) {
    string * miembros, err;
    
    if (!str || str=="") {
	notify_fail("Sintaxis: miembros [grupo]\n");
	return 0;
    }
    if (err = catch(miembros = GRUPOS -> miembros(str))) {
	notify_fail(serror(err)+"\n");
	return 0;
    }    
    if (!sizeof(miembros)) write("Ningun inmortal esta asignado a ese grupo de trabajo.\n");
    else write(implode(miembros, "\n")+"\n");
    return 1;
}

int asignar(string str) {
    string err, user, grupo;
    if ((!str) || (str=="") || (sscanf(str, "%s %s", user, grupo) != 2)) {
	notify_fail("Sintaxis: asignar [inmortal] [grupo]\n");
	return 0;
    }
    if (RANGOS -> es_rango?(grupo)) {
	notify_fail("Usa el comando promote.\n");
	return 0;
    }
    if (err = catch(GRUPOS -> asignar(user, grupo))) {
	notify_fail(serror(err)+"\n");
	return 0;
    }
    write("Inmortal '"+user+"' asignado al grupo de trabajo '"+grupo+"'.\n");
    return 1;
}

int deasignar(string str) {
    string err, user, grupo;
    if ((!str) || (str=="") || (sscanf(str, "%s %s", user, grupo) != 2)) {
	notify_fail("Sintaxis: deasignar [inmortal] [grupo]\n");
	return 0;
    }
    if (RANGOS -> es_rango?(grupo)) {
	notify_fail("Usa el comando demote.\n");
	return 0;
    }
    if (err = catch(GRUPOS -> deasignar(user, grupo))) {
	notify_fail(serror(err)+"\n");
	return 0;
    }
    write("Inmortal '"+user+"' borrado del grupo de trabajo '"+grupo+"'.\n");
    return 1;
}

