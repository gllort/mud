
private mapping propiedades = ([ ]);

class propiedad {
    mixed valor;
    int tiempo;
}

void create() {
    set_heart_beat(1);
}

varargs int propiedad(string propiedad, mixed valor, int tiempo) {
    if ((!propiedad) || (propiedad=="") || (!stringp(propiedad))) return 0;
    if (!tiempo) tiempo = -1;
    propiedades[propiedad] = new (class propiedad);
    propiedades[propiedad] -> valor = valor;
    propiedades[propiedad] -> tiempo = tiempo;
    return 1;
}

int propiedad?(string propiedad) {
    return (member_array(propiedad, keys(propiedades)) == -1 ? 0 : 1);
}

int valor_propiedad?(string propiedad) {
    if (!propiedades[propiedad]) return 0;
    else return propiedades[propiedad] -> valor;
}

int tiempo_propiedad?(string propiedad) {
    if (!propiedades[propiedad]) return 0;
    else return propiedades[propiedad] -> tiempo;
}

string * propiedades?() {
    return keys(propiedades);
}

void heart_beat() {
    int hb;
    hb = __HEARTBEAT_INTERVAL__ / 1000000; /* segundos entre HBs */
    foreach(string propiedad in keys(propiedades)) {
	if (propiedades[propiedad] -> tiempo == -1) continue;
	if ((propiedades[propiedad] -> tiempo - hb) <= 0) {
	    map_delete(propiedades, propiedad);
	}
	else {
	    propiedades[propiedad] -> tiempo = propiedades[propiedad] -> tiempo - hb;
	}
    }
}
