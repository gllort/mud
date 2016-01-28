#include <sexo.h>
#include <dirs.h>

private mapping clases = ([ ]);
private nosave object * obs_clases = ({ });

int cargar_clase() {
    object ob;
    string err;

    foreach(string clase in keys(clases)) {
	err = catch(ob = load_object(clase));
	if ((err) || (!ob)) {
	    write("Error cargando la clase "+clase+"\n");
	    if (err) write(err+"\n");
	    return 0;
	}
	obs_clases += ({ ob });
    }
    return 1;
}

int clase(string nueva_clase) {
    object raza, clase;
    string file, err;

    nueva_clase = lower_case(nueva_clase);

    if (!find_file(nueva_clase)) {
	if (!find_file(DIR_CLASES+"/"+nueva_clase+".c")) {
	    write("Esa clase no existe!\n");
	    return 0;
	}
	else nueva_clase = DIR_CLASES+"/"+nueva_clase+".c";
    }
    if (!(file = TP -> fichero_raza?())) {
	write("La ficha debe tener raza antes que clase!\n");
	return 0;
    }
    if ((err = catch(raza = load_object(file))) || (!raza)) {
	write("Error cargando la raza "+file+"\n");
	if (err) write(err);
	return 0; 
    }
    if (!raza -> clase_permitida?(clase)) {
	write("Un "+raza->nombre_raza?(VARON)+" no puede pertenecer a esa clase.\n");
	return 0;
    }
    if ((err = catch(clase = load_object(nueva_clase)) || (!clase))) {
	write("Error cargando la clase "+nueva_clase+"\n");
	if (err) write(err);
	return 0;
    }
    clases[nueva_clase] = 0;
    obs_clases += ({ clase });
    
    return 1;
}

string * clase?() { 
    string * nombres = ({ }); 
    foreach(object clase in obs_clases) {
	nombres += ({ clase -> nombre_clase?() });	
    }
    return nombres;
}

string * fichero_clase?() {
    return keys(clases);
}

string * skills_favoritas?() {
    string * favoritas = ({ }), * cfav = ({ }); 
    foreach(object clase in obs_clases) {
	cfav = clase -> skills_favoritas?();	
	foreach(string skill in cfav) {
	    if (member_array(skill, favoritas) == -1)
		favoritas += ({ skill });	
	}
    }
    return favoritas;
}

int skill_favorita?(string skill) {
    return (member_array(skill, skills_favoritas?()) == -1 ? 0 : 1);
}

varargs int nivel?(string clase) {
    int found, nivel = 0;
    string nombre;
    
    if ((!clase) || (clase=="")) {
	foreach(string c in keys(clases)) {
	    nivel += clases[c];
	}
	return nivel;
    }
    else {
	found = 0;
	foreach(object ob_clase in obs_clases) {
	    nombre = ob_clase -> nombre_clase?();
    	    if (nombre == clase) {
		found = 1;
		nombre = file_name(ob_clase) + ".c";
		break;	    
	    }
	}	
	if (found) return clases[nombre];
	else return 0;
    }
}

protected void avanzar_clase(string avanzar_clase) {
    if (!clases[DIR_CLASES+"/"+avanzar_clase+".c"]) {
	clase(avanzar_clase);
    }
    clases[DIR_CLASES+"/"+avanzar_clase+".c"] += 1;
}

int ataque_base?() {
    int ab;
    foreach(object clase in obs_clases) {
	ab += clase -> ataque_base?(clases[file_name(clase)+".c"]);    
    }
    return ab;
}

string * clases?() {
    string * clases = ({ });

    foreach(string clase in get_dir(DIR_CLASES+"/*.c")) {
	clases += ({ explode(clase,".c")[0] });   
    }
    return clases;
}

/* Devuelve una lista de clases compatibles con las que el usuario 
 * podria multiclasear su ficha
 */
string * clases_compatibles?() {
    string * compatibles;

    compatibles = clases?();
    foreach(object ob in obs_clases) {
	compatibles -= ({ ob -> nombre_clase?() }); 
	foreach(string incompatible in ob -> clases_incompatibles?()) {
	    compatibles -= ({ incompatible });	
	}
    }
    return compatibles;
}
