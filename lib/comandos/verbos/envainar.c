#include <mudlib.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_envainar_obj(object ob, string obtext) {
    TP -> envainar(ob);
}

mixed can_envainar_obj(object ob, string obtext) {
    return 1; 
}
