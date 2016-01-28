#include <mudlib.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_empuñar_obj(object ob, string obtext) {
    TP -> empuñar(ob);
}

mixed can_empuñar_obj(object ob, string obtext) {
    return 1; 
}
