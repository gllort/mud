#include <gremio.h>

void gremio() {
    nombre_gremio("Asesinos de Calimport");
    razas_permitidas( ({ "humano" }) );
    clases_permitidas( ({ "guerrero", "bribon", "bardo"  }) );
    comando(5, "apu�alar", "/gremios/asesinos/cmds/apu�alar.c");
    comando(10, "emboscar", "/gremios/asesinos/cmds/emboscar.c");
    comando(20, "mortal", "/gremios/asesinos/cmds/mortal.c");
}

