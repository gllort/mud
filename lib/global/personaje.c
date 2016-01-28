#include <dirs.h>
#include <tablas.h>
#include <mudlib.h>
#include <daemons.h>

inherit CUERPO;
inherit CONTENEDOR;
inherit __DIR__ "/personaje/atributos.c";
inherit __DIR__ "/personaje/clase.c";
inherit __DIR__ "/personaje/feats.c";
inherit __DIR__ "/personaje/skills.c";
inherit __DIR__ "/personaje/gremio.c";
inherit __DIR__ "/personaje/comandos.c";
inherit __DIR__ "/personaje/raza.c";
inherit __DIR__ "/personaje/sexo.c";
inherit __DIR__ "/personaje/propiedades.c";

nosave object * atacando = ({ });

void avanzar();
void avanzar2(string, string *, string *);
void avanzar3(string, string, string *, string *);

void create() {
    cuerpo::create();
    contenedor::create();
    atributos::create();
    comandos::create();
    raza::create();
    sexo::create();

    volumen(100);
    crear_relacion("encima", 3500);
}

/********************************************
 *                 AVANCE                   *
 ********************************************/
 
void avanzar() {
    int coste;
    string * clases, * compatibles;
        
    coste = TABLA_COSTES -> coste?(nivel?()+1);
    if (xp_total?() < coste) {
        write("No has conseguido suficientes puntos de experiencia para avanzar al nivel "+(nivel?()+1)+".\n");
        return;
    }
    write("$N$2¡Enhorabuena! $7Puedes avanzar al siguiente nivel.\n\n");
    clases = clase?();
    compatibles = clases_compatibles?();

    if ((sizeof(clases) == 1) && (!sizeof(compatibles))) {
	avanzar2(clases[0], clases, compatibles);
	return;
    }
    else if (sizeof(clases) == 1) 
        write("Puedes avanzar tu clase: \n");
    else 
        write("Puedes avanzar cualquiera de tus clases: \n");
    foreach(string clase in clases) write(capitalize(clase)+"  ");
    write("\n\n");
    
    if (sizeof(compatibles)) {
	write("Tambien puedes multiclasear tu ficha con una de las siguientes clases: \n");
	foreach(string clase in compatibles) write(capitalize(clase)+"  ");
	write("\n\n");
    } 
    write("Que clase quieres avanzar? ");
    input_to("avanzar2", clases, compatibles);
}

void avanzar2(string clase, string * clases, string * compatibles) {
    if ((member_array(clase, clases) == -1) && (member_array(clase, compatibles) == -1)) {
        write("Que clase quieres avanzar? ");
	input_to("avanzar2", clases, compatibles);
	return;
    }    
    if (member_array(clase, compatibles) != -1) {
	write("$N$4¡Atencion!$R Esto agrega una nueva clase a tu ficha y te hace multiclase.\n");
    }
    write("Estas seguro? ");
    input_to("avanzar3", clase, clases, compatibles);
}

void avanzar3(string opcion, string clase, string * clases, string * compatibles) {
    opcion = lower_case(opcion);
    if ((opcion == "s") || (opcion == "si")) {
	avanzar_clase(clase);
	write("Avanzas tus habilidades de "+capitalize(clase)+" al nivel "+nivel?(clase)+".\n");
	write("Has alcanzado el nivel "+nivel?()+".\n");
	/* Avanzar feats */
    }
    else {
        write("Que clase quieres avanzar? ");
	input_to("avanzar2", clases, compatibles);
	return;
    }
}

/********************************************
 *             PARSER APPLIES               *
 ********************************************/

int is_living() { return 1; }


/********************************************
 *                 VARIOS                   *
 ********************************************/

void mirar() {
    write("Ves a "+capitalize(nombre?())+"\n");
}

void atacar(object ob) {
    atacando += ({ ob });
    ob -> atacado_por(TO);
    COMBATE -> start_fight(TO, ob);
}
void atacado_por(object ob) {
    atacando += ({ ob });
}
object * atacando?() { return atacando; }
