#include <dirs.h>
#include <gennum.h>
#include <mudlib.h>
#include <mensajes.h>

#define NOMBRE(x) x[0]
#define ITEMS(x) x[1]
#define GENERO(x) x[2]
#define NUMERO(x) x[3]
#define SUBPARTES(x) (sizeof(x) >= 5 ? x[4] : ({ }))

class parte {
    int items;
    int genero;
    int numero;
    string * subpartes;
    object * equipo;
}

/* Variables globales */
private nosave object forma;			         /* Relacion con un objeto forma */
private nosave mapping partes_cuerpo = ([ ]);		 /* Formas del cuerpo mapeadas */
private nosave string * partes_cuerpo_ordenadas = ({ }); /* Lista ordenada de partes */

private nosave object * empuñando = ({ }); /* Armas que estamos empunyando */

/* Prototipos */
void definir_partes(mixed *);
object * empuñando?();
varargs object * vistiendo?(string);

void create() { ; }

/***********************************************
 *		     FORMA 		       *
 ***********************************************/

nomask int cambiar_forma(string form) {
    if (!find_file(DIR_FORMAS+"/"+form+".c")) {
	write("Esa forma no existe!\n");
	return 0;
    }
    if (catch(forma = load_object(DIR_FORMAS+"/"+form+".c"))) {
	write("Error cargando la forma "+form+".\n");
	return 0;
    }
    definir_partes( forma -> forma_cuerpo?() );     
    return 1;
}

void definir_partes(mixed * partes) {
    partes_cuerpo = ([ ]);
    partes_cuerpo_ordenadas = ({ });
    foreach(mixed * parte in partes) {
	partes_cuerpo_ordenadas += ({ NOMBRE(parte) }); 
	partes_cuerpo[NOMBRE(parte)] = new (class parte);
	partes_cuerpo[NOMBRE(parte)] -> items = ITEMS(parte);
	partes_cuerpo[NOMBRE(parte)] -> genero = GENERO(parte);
	partes_cuerpo[NOMBRE(parte)] -> numero = NUMERO(parte);
	partes_cuerpo[NOMBRE(parte)] -> subpartes = SUBPARTES(parte);
	partes_cuerpo[NOMBRE(parte)] -> equipo = ({ });
    }
}

string * forma?() { return partes_cuerpo_ordenadas; }

string nombre_forma?() { return forma -> nombre_forma?(); }

int items_sostenibles?() { return forma -> items_sostenibles?(); }

/***********************************************
 *		     CUERPO		       *
 ***********************************************/

/* Empunya un arma */
void empuñar(object ob) {
    if (!ob->raza_permitida?(TP->nombre_raza?())) {
	write("Esta arma no esta disenyada para alguien de tu raza.\n");
	return;
    }
    if (member_array(ob, empuñando) != -1) {
	write("Ya lo estas empuñando.\n");
	return;
    }
    if ((sizeof(empuñando) + ob->espacio_que_ocupa?()) <= forma -> items_sostenibles?()) {
	empuñando += ({ ob });
        write("Empuñas tu "+ob->nombre?()+".\n");
    }
    else {
	write("Te faltan manos para empuñar la "+ob->nombre?()+".\n");
    }
}

void envainar(object ob) {
    if (member_array(ob, empuñando) == -1) {
	write("No lo estas empuñando.\n");
	return;
    }
    empuñando -= ({ ob });
    write("Envainas tu "+ob->nombre?()+".\n");
}

object * empuñando?() { return empuñando; }

/* Viste un objeto */
void vestir(object ob) {
    string det;
    string * ocupa;

    if (member_array(ob, vistiendo?()) != -1) {
	write("Ya lo llevas puesto.\n");
	return;
    } 
    ocupa = ob -> partes_que_ocupa?();
    foreach(string parte in ocupa) {
	if (!((partes_cuerpo[parte]) && ((sizeof(partes_cuerpo[parte]->equipo)+1) <= partes_cuerpo[parte]->items))) {
	    write("Ya llevas algo puesto en "+DETERMINANTE(partes_cuerpo[parte]->genero, partes_cuerpo[parte]->numero)+" "+parte+".\n");
	    return;    
	}	
    }
    foreach(string parte in ocupa) partes_cuerpo[parte] -> equipo += ({ ob });
    write("Te pones tu "+ob->nombre?()+".\n");
}

void quitar(object ob) {
    string * ocupa;
    object * equipo = ({ });
    
    equipo = vistiendo?();
    if (member_array(ob, equipo) == -1) {
	write("No lo llevas puesto.\n");
	return;
    }
    ocupa = ob -> partes_que_ocupa?();
    foreach(string parte in ocupa) partes_cuerpo[parte] -> equipo -= ({ ob });    
    write("Te quitas tu "+ob->nombre?()+".\n");
}

varargs object * vistiendo?(string zona) {
    object * equipo = ({ });

    if ((!zona) || (zona == "")) 
	foreach(class parte parte in values(partes_cuerpo)) equipo += parte -> equipo;
    else 
	if (partes_cuerpo[zona]) return equipo = partes_cuerpo[zona] -> equipo;
    return equipo;
}

int ac?() {
    int ac;
    object * equipo;
    
    equipo = vistiendo?();
    foreach(object item in equipo) {
	if (inherits(ARMADURA, item)) {
	    ac += item -> ac?();
	}
    }
    return ac;
}

string zona_impacto?() {
    string * partes, parte; 
    
    partes = keys(partes_cuerpo);
    parte = partes[random(sizeof(partes))];
    return TO -> determinante(partes_cuerpo[parte]->genero, partes_cuerpo[parte]->numero) + " " + parte;
    
}


class mensaje mensaje_combate_desarmado?(int gravedad) {
    return forma -> random_msg?(gravedad);
}

