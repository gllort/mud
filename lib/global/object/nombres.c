#include <gennum.h>

private string nombre = "", descripcion = "";
private nosave string * ids       = ({ });
private nosave string * plurales  = ({ });
private nosave string * adjetivos = ({ });
private nosave int genero = MASCULINO;
private nosave int numero = SINGULAR;

void create() {
    parse_init();
}

/******************************************
 *          NOMBRES Y ADJETIVOS           *
 ******************************************/

void nombre(string str) {
    string strip;
    if (!str || str=="") return;
    
    if ((origin() != "local") && (nombre != "")) {
	error("Prohibido cambiar externamente de nombre a un objeto.\n");
    }
    nombre = str;
    strip = strip(lower_case(str));
    ids += ({ strip });
    plurales += ({ pluralize(strip) });
    parse_refresh();
}

string nombre?() { return nombre; }

void descripcion(string str) {
    descripcion = str;
    parse_refresh();
}
string descripcion?() { return descripcion; }

void ids(string * id) { ids += id; }

string * ids?() { return ids; }

void adjetivos(string * adj) { adjetivos += adj; }

string * adjetivos?() { return adjetivos; }

void plurales(string * plu) { plurales += plu; }

string * plurales?() { return plurales; }

/******************************************
 *            GENERO Y NUMERO             *
 ******************************************/

void genero(int gen) { genero = gen; }

int genero?() { return genero; }

void numero(int num) { numero = num; }

int numero?() { return numero; }

varargs string determinante?(string gg, string nn) {
    int g, n;
    
    g = ( !gg ? genero : gg );
    n = ( !nn ? numero : nn );

    switch(g) {
	case FEMENINO:
	    return (n == SINGULAR ? "la" : "las");
	case MASCULINO:
	    return (n == SINGULAR ? "el" : "los");
	default:
	    return (n == SINGULAR ? "lo" : "los");
    }
}

/******************************************
 *             PARSER APPLIES             *
 ******************************************/

string array parse_command_id_list() {
    string * nombres_mas_adjetivos = ({ });
    foreach(string identificador in ids) {
	foreach(string adjetivo in adjetivos) {
	    nombres_mas_adjetivos += ({ identificador + " " + adjetivo });
	}
    }
    return ({ }) + ids + nombres_mas_adjetivos;
}

nomask string array parse_command_adjectiv_id_list() {
    return adjetivos;
}

nomask string array parse_command_plural_id_list() {
    return plurales;
}

