#include <mudlib.h>

private string fichero_gremio;
private nosave object gremio;
private int nivel_alistamiento;

int cargar_gremio() {
    string err;
    if (fichero_gremio) {
	err = catch(gremio = load_object(fichero_gremio));
	if ((err) || (!gremio)) {
	    write("Error cargando gremio "+fichero_gremio+"\n");
	    if (err) write(err+"\n");
	    return 0;
	}
    }	
    return 1;
}

void gremio(string file) {
    if ((!inherits(GUILDROOM, PO(0))) && (!adminp())) return;
    fichero_gremio = file;
    gremio = load_object(file);
    nivel_alistamiento = TP->nivel?();
}

string gremio?() {
    if (!gremio) return "Ninguno";
    return gremio -> nombre_gremio?();
}

string fichero_gremio?() { return fichero_gremio; }

int nivel_alistamiento?() { return nivel_alistamiento; }

