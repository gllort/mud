#include <mudlib.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_vestir_obj(object ob, string obtext) {
    TP -> vestir(ob);
}

mixed can_vestir_obj(object ob, string obtext) {
    return 1; 
}
