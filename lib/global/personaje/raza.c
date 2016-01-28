#include <dirs.h>

private string fichero_raza;
private nosave string nombre_raza;
private nomask int encarnar(string);

protected void create() {
    nombre_raza = "Raza desconocida";
}

protected int cargar_raza() {
    if (fichero_raza) {
	return encarnar(fichero_raza);
    }
}

private nomask int encarnar(string racefile) { 
    object raza;
    
    if (!find_file(racefile)) {
	write("Esa raza no existe!\n");
	return 0;
    }
    if (catch(raza = load_object(racefile))) {
	write("Error cargando la raza "+racefile+".\n");
	return 0;
    }
    nombre_raza = raza -> nombre_raza?(TP -> sexo?());

    TO -> tmp_fue( raza -> bonus_fue?() );
    TO -> tmp_des( raza -> bonus_des?() );
    TO -> tmp_con( raza -> bonus_con?() );
    TO -> tmp_tel( raza -> bonus_tel?() );
    TO -> tmp_sab( raza -> bonus_sab?() );
    TO -> tmp_car( raza -> bonus_car?() );
    TO -> tmp_sue( raza -> bonus_sue?() );

    return (TO -> cambiar_forma(raza -> forma?()));
}

nomask int raza(string race) {
    if (!find_file(race)) {
	race = DIR_RAZAS+"/"+race+".c";
	if (!find_file(race)) {
	    write("Esa raza no existe!\n");
	    return 0;
	}
    }
    if (encarnar(race)) {
	fichero_raza = race;
	return 1;
    }
    return 0;
}

string fichero_raza?() { return fichero_raza; }

string raza?() { return nombre_raza; }

