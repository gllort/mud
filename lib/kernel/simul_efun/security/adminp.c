#include <mudlib.h>
#include <daemons.h>

int adminp() {
    if (TP) {
	if (((file_name(TP) == INMORTAL) || (inherits(INMORTAL, TP)>0)) && (TP->nivel_inmortal?() == RANGOS->nivel_maximo?()))
	    return 1;
	else
	    return 0;
    }
    else {
	/* Esta ejecucion no nace de ningun jugador. Se origina desde
	 * algun objeto de sistema (creo) 
	 */
	return 1;
    }
}
