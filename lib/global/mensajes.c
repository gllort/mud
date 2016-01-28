#include <mensajes.h>
#include <da�os.h>

private nosave class mensaje * mensajes = ({ });

void mensajes() { ; }

void create() {
    mensajes();
}

void mensaje(int gravedad, int tipo_da�o, string texto_yo, string texto_el, string texto_otros) {
    class mensaje msg = new(class mensaje);
    msg -> tipo_da�o = tipo_da�o;
    msg -> gravedad = gravedad;
    msg -> texto_yo = texto_yo;
    msg -> texto_el = texto_el;
    msg -> texto_otros = texto_otros;
    mensajes += ({ msg });    
}

varargs class mensaje random_msg?(int gravedad, int tipo_da�o) {
    int size;
    class mensaje def, * msgs = ({ });

    write("Grav: "+gravedad+"\n");

    if ((gravedad < PIFIA) || (gravedad > MORTAL)) gravedad = LEVE;

    /* Filtramos mensajes de esa gravedad */
    foreach(class mensaje msg in mensajes) {
        if (msg -> gravedad == gravedad) {
	    if ((!tipo_da�o) || (tipo_da�o == msg -> tipo_da�o)) { 
		msgs += ({ msg });
	    }
	}    
    }
    size = sizeof(msgs);    
    if (!size) {
	def = new(class mensaje);
	def -> tipo_da�o = APLASTANTE;
	def -> gravedad = gravedad; 
	def -> texto_yo = "No hay ningun mensaje definido de gravedad "+gravedad+".\n";
	def -> texto_el = "No hay ningun mensaje definido de gravedad "+gravedad+".\n";
	def -> texto_otros = "No hay ningun mensaje definido de gravedad "+gravedad+".\n";
	return def;
    }
    else return msgs[random(size)];
}

void debug_mensaje() {
    printf("%O\n", mensajes);
}
