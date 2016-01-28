#include <mudlib.h>
#include <daemons.h>
#include <errno.h>
#include <room.h>

void init() {
    add_action("listar", "listar");
    
}

void habitacion() {
    nombre("$NTorre del Destino: $4Control de Canales$R");

    salida("oeste", "/dominios/cielo/comun.c");
}


int listar() {
    write(implode(CANALES -> canales?(), "\n")+"\n");
}
