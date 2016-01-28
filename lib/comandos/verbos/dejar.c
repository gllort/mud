#include <mudlib.h>
#include <move.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_dejar_obj(object ob, string obtext) {
    mixed res;    

    res = ob -> move(ETP);
    if (res != MOVE_OK) {
	if (stringp(res)) {
	    write(res+"\n");
	}
    }
    else write("Dejas "+ob->nombre?()+" en "+ENV(ob)->nombre?()+".\n");
}

mixed can_dejar_obj(object ob, string obtext) {
    printf("can_dejar_obj %O %O\n", ob, obtext);
    return 1; 
}

