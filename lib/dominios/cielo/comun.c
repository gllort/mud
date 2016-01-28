#include <mudlib.h>

#include <room.h>

void create() {
    ::create();
    printf("%O\n", APO);
}
 
void habitacion() {
    nombre("$NTorre del destino: $6Gran Sal�n$R");
    descripcion("Te encuentras en la $NSala de la Creaci�n$R de los Reinos.\n"
             "Un amplio sal�n de marmol jaspeado se extiende ante ti.\n"
	     "A ambos lados de la sala unas columnas plateadas sustentan\n" 
	     "un firmamento cambiante, rodeado por miles de estrellas, en\n" 
	     "el que observas como la tierra se moldea bajo el capricho de\n" 
	     "sus Creadores. En el centro de la sala ves un tabl�n de\n"
	     "anuncios, en el que se reflejan las opiniones y cambios que\n"
	     "han tenido lugar en los Reinos de $0$NMundo Oscuro$R.");
    
    salida("norte", "/dominios/cielo/rangos.c");
    salida("este", "/dominios/cielo/canales.c");
    salida("oeste", "/dominios/cielo/grupos.c");

}
 
