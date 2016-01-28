
private string verbo = relative_name(TO); /* El nombre relativo del fichero */

void create() { ; }

protected varargs void crear_regla(array reglas, array sinonimos) {
    parse_init();
    foreach(string regla in reglas) {
	parse_add_rule(verbo, regla);
	if (sizeof(sinonimos)) {
	    /* Esto de los sinonimos... */
	    foreach(string sinonimo in sinonimos) {
		parse_add_synonym(sinonimo, verbo, regla);
	    }
	}
    }
}

mixed can_verb_rule(string verbo, string regla) {
    write("can_verb_rule: "+verbo+" "+regla+"\n");
    return 1;
}

void do_verb_obj(string verbo, object ob) {
    write("do_verb_obj");
    printf("%O\n", ob);
    call_other(ob, "do_"+verbo);
}


