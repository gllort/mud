#include <mudlib.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_quitar_obj(object ob, string obtext) {
    TP -> quitar(ob);
}

mixed can_quitar_obj(object ob, string obtext) {
    return 1; 
}
