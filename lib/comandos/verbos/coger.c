#include <mudlib.h>
#include <move.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "OBJ" }) );
}

void do_coger_obj(object ob, string obtext) {
    mixed res;    
    string prev_env;

    prev_env = ENV(ob) -> nombre?();
    res = ob -> move(TP);
    if (res != MOVE_OK) {
	if (stringp(res)) {
	    write(res+"\n");
	}
    }
    else write("Coges "+ob->nombre?()+" de "+prev_env+".\n");
}

mixed can_coger_obj(object ob, string obtext) {
    printf("can_coger_obj %O %O\n", ob, obtext);
    return 1; 
}

