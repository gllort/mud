#include <mudlib.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_matar_obj(object ob, string obtext) {
    write("Empiezas a atacar a "+capitalize(ob->nombre?())+".\n");
    tell_object(ob, capitalize(TP->nombre?())+" empieza a atacarte.\n");
    TP -> atacar(ob);
}

mixed can_matar_obj(object ob, string obtext) {
    return 1; 
}

