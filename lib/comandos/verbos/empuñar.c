#include <mudlib.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_empu�ar_obj(object ob, string obtext) {
    TP -> empu�ar(ob);
}

mixed can_empu�ar_obj(object ob, string obtext) {
    return 1; 
}
