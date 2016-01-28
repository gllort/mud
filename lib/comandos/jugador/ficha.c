#include <comando.h>

int comando(int argc, string * argv) {
    int size;
    string str, * clases;

    write("Ficha de "+capitalize(TP->nombre?())+",\n");
    write("Raza: "+capitalize(TP -> raza?())+"\n");
    size = sizeof(clases = TP -> clase?());
    for(int i=0; i<size; i++) {
    	clases[i] = capitalize(clases[i]);
    }
    if (size > 1) write("Multiclase: ");
    else write("Clase: ");
    write(capitalize(implode(clases,"/"))+"\n");
    write("Nivel: "+TP->nivel?()+" (");
    str = "";
    foreach(string clase in clases) {
	str += "/" + (string)TP -> nivel?(lower_case(clase));
    }
    write(str[1..]+")\n");
    write("Gremio: "+TP->gremio?()+"\n");
    write("Fuerza: "+TP->fue?()+"\n");
    write("Destreza: "+TP->des?()+"\n");
    write("Constitucion: "+TP->con?()+"\n");
    write("Inteligencia: "+TP->tel?()+"\n");
    write("Sabiduria: "+TP->sab?()+"\n");
    write("Carisma: "+TP->car?()+"\n");
    write("Experiencia: "+TP->xp?()+" de "+TP->xp_total?()+" puntos conseguidos.\n");
    return 1;
}
