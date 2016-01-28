#include <mudlib.h>

mixed direct_mirar_a_obj(object ob, string obtext) {
    printf("DIRECT_MIRAR_A_OBJ %O %s\n", ob, obtext);
    return 1;
}
mixed direct_mirar_a_obj_en_obj(object ob1, object ob2, string ob1text, string ob2text) {
    write("DIRECT_MIRAR_A_OBJ_EN_OBJ\n");
    return 1;
}

mixed direct_dejar_obj(object ob, string obtext) {
    if (ENV(ob) == TP) return 1;
    else return 0;
}

mixed direct_coger_obj(object ob, string obtext) {
    if (ENV(ob) == ENV(TP)) return 1;
    else return 0;
}

mixed direct_empu�ar_obj(object ob, string obtext) {
    if ((ENV(ob) == TP) && (inherits(ARMA, ob))) {
	return 1;
    }
    else return "No puedes empu�ar eso.\n";
}

mixed direct_envainar_obj(object ob, string obtext) {
    if ((ENV(ob) == TP) && (inherits(ARMA, ob))) {
	return 1;
    }
    else return "No puedes envainar eso.\n";
}

mixed direct_matar_obj(object ob, string obtext) {
    if (ENV(ob) == ENV(TP)) return 1;
    else return 0;
}

mixed direct_vestir_obj(object ob, string obtext) {
    if ((ENV(ob) == TP) && (inherits(ARMADURA, ob))) {
	return 1;
    }
    else return "No puedes ponerte eso.\n";
}

mixed direct_quitar_obj(object ob, string obtext) {
    if ((ENV(ob) == TP) && (inherits(ARMADURA, ob))) {
	return 1;
    }
    else return "No puedes quitarte eso.\n";
}
