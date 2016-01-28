/* Inspirado en la idea de relaciones de la mudlib Lima */

#include <mudlib.h>
#include <move.h>
#include <contenedor.h>

inherit BASEOB;

class relacion {
    object * contenido;
    int capacidad;
    int oculta;
    mapping crear_en_reset;
}

private mapping relaciones = ([ ]);
private mapping alias_relaciones = ([ ]);
private string relacion_por_defecto;
void relacion_por_defecto(string);

void create() {
    ::create();
}

/* Devuelve la relacion que contiene un objeto determinado. */
string relacion(object ob) {
    foreach(string relation, class relacion values in relaciones) {
	if (member_array(ob, values->contenido)!=-1) return relation;
    }
}

/* Crea una nueva relacion de posicion en el contenedor. */
varargs void crear_relacion(string rel, int max_capacity, int hidden) {
    class relacion nueva_relacion = new (class relacion);
    nueva_relacion -> contenido = ({ });
    nueva_relacion -> capacidad = max_capacity;
    nueva_relacion -> oculta = hidden;
    nueva_relacion -> crear_en_reset = ([ ]);
    relaciones[rel] = nueva_relacion;

    /* Si es la primera, la seteamos como relacion por defecto del objeto */
    if (sizeof(relaciones) == 1) relacion_por_defecto(rel);
}

/* Elimina una relacion del contenedor solo si NO contiene ningun objeto. */
void borrar_relaciones(string * relations) {
    foreach(string relation in relations) {
        if (sizeof(relaciones[relation]->contenido)) continue;
	map_delete(relaciones, relation);
    }
}

/* Devuelve la lista de relaciones validas del contenedor */
string * relaciones?() { return keys(relaciones); }

int relacion_valida?(string relation) {
    return (member_array(relation, keys(relaciones)) == -1 ? 0 : 1);    
}

void relacion_por_defecto(string relation) { 
    if (!relacion_valida?(relation)) {
	error("Imposible definir la relacion inexistente '"+relation+"' como relacion por defecto.");
    }
    relacion_por_defecto = relation; 
}

string relacion_por_defecto?() { return relacion_por_defecto; }

varargs void capacidad_maxima(int capacity, string relation) {
    if ((!relation) || (relation=="")) relation = relacion_por_defecto?();
    if (!relacion_valida?(relation)) 
	error("Relacion inexistente.");
    relaciones[relation] -> capacidad = capacity;
}

/* Devuelve la capacidad maxima de una relacion */
varargs int capacidad_maxima?(string relation) {
    if ((!relation) || (relation=="")) relation = relacion_por_defecto?();
    if (!relacion_valida?(relation)) return 0;
    return relaciones[relation] -> capacidad;
}

/* Devuelve la capacidad actual de una relacion */
varargs int capacidad?(string relation) {
    int capacity = 0;
    if ((!relation) || (relation=="")) relation = relacion_por_defecto?();
    if (!relacion_valida?(relation)) return 0;
    foreach(object ob in relaciones[relation]->contenido) {
	if (!ob) {
	    relaciones[relation]->contenido -= ({ ob });
	    continue;
	}
	capacity += ob -> volumen?(); 	
    }
    return capacity;
}

/* Se ejecuta cuando un objeto se mueve fuera del contenedor */
int leave_object(object target) {
    string relation;

    if (!target) return 1;    
    relation = relacion(target);

    if (!relation) return 0;
    relaciones[relation] -> contenido -= ({ target });
    return 1;
}

/* Se ejecuta cuando un objeto se mueve dentro del contenedor */
varargs mixed enter_object(object target, string relation) {
    int volumen;

    if ((!relation) || (relation=="")) relation = relacion_por_defecto?();

    if (target == TO) return "No puedes poner un objeto dentro de si mismo.\n";    
    if (!relacion_valida?(relation)) return "No puedes poner cosas "+relation+" de "+TO->nombre?()+".\n";

    volumen = target -> volumen?();
    if ((capacidad?(relation)+volumen) > capacidad_maxima?(relation)) {
	return capitalize(target->nombre?())+" es demasiado grande y no cabe "+relation+" de "+TO->nombre?()+".\n";
    }
    relaciones[relation] -> contenido += ({ target });
    return 1;	 
}

varargs object * objetos(string relation) {
    if ((!relation) || (relation=="")) relation = relacion_por_defecto?();
    if (!relacion_valida?(relation)) return ({ });
    foreach(object ob in relaciones[relation]->contenido) {
	if (!ob) {
	    relaciones[relation]->contenido -= ({ ob });
	    continue;
	}
    }
    return relaciones[relation] -> contenido;
}

varargs string look_in(string relation) {
    string str = "";
    
    if ((!relation) || (relation=="")) relation = relacion_por_defecto?();
    if (!relacion_valida?(relation)) return "No hay nada.\n";
    foreach(object ob in relaciones[relation]->contenido) {
	str += 	ob -> nombre?() + "\n";
    }
    return str;
}

int contenedor?() { return 1; }


void dump() {
    string str = "";
    object * obs;

    foreach(string relation in keys(relaciones)) {
	str += capitalize(relation)+" de "+TO->nombre?()+"$R ves: \n";
	str += look_in(relation);
	str += "\n";
    } 
    write(str);
}

/* Parser applies */
int inventory_visible() { return 1; }

int inventory_accessible() { return 1; }
