#include <mudlib.h>

inherit VERBO;

void create() {
    ::create();
    crear_regla( ({ "", "a OBJ", "a OBS en OBJ" }) );
}

void do_mirar() {
    ETP -> mirar();
}

void do_mirar_a_obj(object ob, string obtext) {
    ob -> mirar();    
}

void do_mirar_a_obs_en_obj(object * obs, object ob, string obstext, string obtext) {
    write("do_mirar_a_obs_en_obj\n");    
}

void do_mirar_a_obj_en_obj(object * obs, object ob, string obstext, string obtext) {
    write("do_mirar_a_obj_en_obj\n");    
}

mixed can_mirar() {
    printf("can_mirar\n");
    return 1; 
}

mixed can_mirar_a_obj(object ob, string obtext) { 
    printf("can_mirar_obj %O %s\n", ob, obtext);
    return 1; 
}

mixed can_mirar_a_obj_en_obj(object * obs, object ob, string obstext, string obtext) { 
    printf("can_mirar_a_obj_en_obj %O %O %s %s\n", obs, ob, obstext, obtext);
    return 1; 
}
