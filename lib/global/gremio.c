
private nosave string gremio;
private nosave mixed * comandos = ({ });
private nosave string * clases_permitidas, * razas_permitidas;

void gremio() { ; }

void create() {
    razas_permitidas = ({ });
    clases_permitidas = ({ });
    gremio = relative_name(TO);
    gremio();
}

void nombre_gremio(string guild) { gremio = guild; }
string nombre_gremio?() { return gremio; }

void clases_permitidas(string * clases) {
    clases_permitidas = clases;
}

string * clases_permitidas?() { return clases_permitidas; }

void razas_permitidas(string * razas) {
    razas_permitidas = razas;
}

string * razas_permitidas?() { return razas_permitidas; }

int acceso_permitido?() {
    int clase_valida;
    string * clases;
    /* Comprueba si TP puede alistarse en el gremio */

    if ((sizeof(razas_permitidas)) && (member_array(TP -> raza?(), razas_permitidas) == -1)) {
	write("Este gremio no acepta gente de tu raza!\n");
	return 0;
    }

    clases = TP -> clase?();
    clase_valida = 0;
    foreach(string clase in clases) {
	if (member_array(clase, clases_permitidas) != -1) {
	    clase_valida = 1;
	    break;
	}    
    }
    if ((!clase_valida) && (sizeof(clases_permitidas))) {
        write("Este gremio no acepta gente de tu clase!\n");
        return 0;
    }
    return 1;
}

void comando(int nivel, string alias, string fichero) {
    if (find_file(fichero))
	comandos += ({ ({ nivel, alias, fichero }) });
}

mapping comandos_nivel(int nivel) {
    mapping res = ([ ]);
    foreach(mixed * comando in comandos) {
	if (comando[0] <= nivel) {
	    res[comando[1]] = comando[2];
	}
    }
    return res;
}

mixed * comandos?() { return comandos; }
