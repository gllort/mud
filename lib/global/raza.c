#include <dirs.h>
#include <sexo.h>

private nosave int fue_bon, des_bon, con_bon, car_bon, tel_bon, sab_bon, sue_bon;
private nosave string forma, nombre_varon, nombre_hembra, nombre_asexual, descripcion;
private nosave string * clases_permitidas = ({ }), * clases_favoritas = ({ }); 

void raza() { ; }

void create() {
    fue_bon = des_bon = con_bon = car_bon = tel_bon = sab_bon = sue_bon = 0;
    forma = "humanoide";
    raza();
}

void bonus_fue(int bonus) { fue_bon = bonus; }
void bonus_des(int bonus) { des_bon = bonus; }
void bonus_con(int bonus) { con_bon = bonus; }
void bonus_tel(int bonus) { tel_bon = bonus; }
void bonus_sab(int bonus) { sab_bon = bonus; }
void bonus_car(int bonus) { car_bon = bonus; }
void bonus_sue(int bonus) { sue_bon = bonus; }
int bonus_fue?() { return fue_bon; }
int bonus_des?() { return des_bon; }
int bonus_con?() { return con_bon; }
int bonus_tel?() { return tel_bon; }
int bonus_sab?() { return sab_bon; }
int bonus_car?() { return car_bon; }
int bonus_sue?() { return sue_bon; }

void forma(string forma_cuerpo) { forma = forma_cuerpo; }
string forma?() { return forma; }

string nombre_raza(string v, string h, string a) { 
    nombre_varon = v;
    nombre_hembra = h;
    nombre_asexual = a;
}

string nombre_raza?(int genero) {
    switch(genero) {
	case VARON: return nombre_varon;
	case HEMBRA: return nombre_hembra;
	case ASEXUAL: return nombre_asexual;
	default: return nombre_asexual;
    }
}

string descripcion(string desc) { descripcion = desc; }
string descripcion?() { return descripcion; }

void clases_permitidas(string * clases) {
    foreach(string clase in clases) {
	clase = lower_case(clase);
	if (find_file(DIR_CLASES + "/" + clase + ".c")) {
	    clases_permitidas += ({ clase });
	}
    }
}

int clase_permitida?(string clase) {
    if (!sizeof(clases_permitidas)) return 1;
    return (member_array(lower_case(clase), clases_permitidas) == -1 ? 0 : 1);
}

string * clases_permitidas?() { return clases_permitidas; }

void clases_favoritas(string * clases) {
    foreach(string clase in clases) {
	clase = lower_case(clase);
	if (find_file(DIR_CLASES + "/" + clase + ".c")) {
	    clases_favoritas += ({ clase });
	}
    }
}

string * clases_favoritas?() { return clases_favoritas; }

int clase_favorita?(string clase) {
    if (!sizeof(clases_favoritas)) return 1;
    return (member_array(lower_case(clase), clases_favoritas) == -1 ? 0 : 1);
}
