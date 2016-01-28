
private nosave string * active_flags = ({ }), * valid_flags = ({ });

int comando(int, string *) { ; }

void create() { ; }

private nomask void enable_flag(string flag) {
    if (!flag || flag=="") return;
    if (member_array(flag, active_flags) == -1)
	active_flags += ({ flag });
}

int flag?(string flag) {
    return (member_array(flag, active_flags) == -1 ? 0 : 1);
}

void valid_flags(string flags) {
    int i, len;
    len = strlen(flags);
    if (!flags || flags=="") return;
    for(i=0; i<len; i++) {
	if (member_array(flags[i..i], valid_flags) == -1)
	    valid_flags += ({ flags[i..i] });
    }
}

int valid_flag?(string flag) {
    return (member_array(flag, valid_flags) == -1 ? 0 : 1);
}

int exec_command(int argc, string * argv) {
    int i, len;
    
    if ((argc) && (argv[0][0]=='-')) {
	len = strlen(argv[0]);
	for(i=1; i<len; i++) {
	    if (valid_flag?(argv[0][i..i]))	
		enable_flag(argv[0][i..i]);
	    else {
		notify_fail(relative_name(TO)+": "+argv[0][i..i]+": Parametro incorrecto.\n");
		return 0;
    	    }
	}
	return comando((argc - 1), (argv - ({ argv[0] })));		
    }
    else return comando(argc, argv);
}
