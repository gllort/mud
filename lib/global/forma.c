#include <mudlib.h>

inherit MENSAJES;

private string nombre_forma = relative_name(TO); /* El nombre relativo del fichero */
private nosave int items_sostenibles; 
private nosave mixed * forma_cuerpo = ({ });
private nosave string * mensajes_desarmado = ({ });

void forma() { ; }

void mensajes() { ; }

void create() {
    forma();
    mensajes();
}

void forma_cuerpo(mixed * forma) { forma_cuerpo = forma; }
mixed * forma_cuerpo?() { return forma_cuerpo; }

void items_sostenibles(int items) { items_sostenibles = items; }
int items_sostenibles?() { return items_sostenibles; }

string nombre_forma?() { return nombre_forma; }

void mensajes_desarmado(string * msgs) { mensajes_desarmado = msgs; }
string * mensajes_desarmado?() { return mensajes_desarmado; }
