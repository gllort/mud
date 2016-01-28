
mapping comandos_especificos = ([ ]);

void create() { 

}

nomask void add_command(string verbo, string cmdfn) {
    comandos_especificos[verbo] = cmdfn;
}

nomask void rem_command(string verbo) {
    map_delete(comandos_especificos, verbo);
}

nomask string * commands?() { 
    return keys(comandos_especificos);
}

int ejecutar_comando(string comando) {
    int argc;
    object cmd;
    string err, verbo, argumentos, file, * argv;
    
    if ((!comando)||(comando=="")) return 0;
    if (sscanf(comando, "%s %s", verbo, argumentos) < 2) {
	verbo = comando;
	argumentos = "";
    }
    if (file = comandos_especificos[verbo]) {
	if (find_file(file)) {
	    err = catch(cmd = load_object(file));
	    if ((err) || (!cmd)) {
		write(err+"\n");
		return 1;
	    }
	    else {
		argv = explode(argumentos, " ");
		argc = sizeof(argv);
		return cmd->comando((int)argc, (string *)argv);
	    }
	}
	else {
	    rem_command(verbo);
    	    write("Has olvidado como hacer eso!\n");
	    return 1;
	}
    }
    else return 0;
}
