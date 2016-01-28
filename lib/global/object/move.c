#include <move.h>
#include <macros.h>

private nosave object lugar_previo;
private string ultimo_lugar;

void create() { ; }

/* FUNCION: move(destino, relacion)
 * Mueve el objecto actual dentro del objeto contenedor destino con
 * la relacion especificada.
 * Si hay errores, retorna un mensaje de error (string) o un numero 
 * distinto de MOVE_OK.
 */
varargs mixed move(mixed destino, string relacion) {
    mixed res;
    string antigua_rel;

    if (stringp(destino)) {
	catch(destino = load_object(destino));
    }
    if (!objectp(destino)) {
	return MOVE_NO_DEST;
    }
    lugar_previo = ETO;
    if (lugar_previo) {
	antigua_rel = lugar_previo -> relacion(TO);
    	if (!lugar_previo -> leave_object(TO)) {
	    return MOVE_CANT_LEAVE;
	}
    }
    if ((res = destino -> enter_object(TO, relacion)) != MOVE_OK) {
        /* Reinsertar el objeto en su antigua relacion */
	if ((lugar_previo) && (lugar_previo->enter_object(TO, antigua_rel) != MOVE_OK)) {
	    error("Error insertando objeto en el contenedor.");
	}
	return res;
    }
    move_object(destino);

    ultimo_lugar = file_name(destino);
    return MOVE_OK;
}

object lugar_previo?() { return lugar_previo; }
string ultimo_lugar?() { return ultimo_lugar; }
