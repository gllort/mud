
inherit BASEROOM;
#include "path.h"

void habitacion() {



nombre("$N$0Zorimeth:$R$6 Avenida de la Antipoda Oscura$R");
descripcion("Junto con la avenida de Lloth, la avenida de la Antipoda Oscura es una de las principales "
			"calles de Zorimeth. Por aqui se desarrollan las principales actividades de la ciudad. "+
			"No es muy dificil ver pasar a gente por este lugar. Tanto drows como orcos y goblins "+
			"esclavizados caminan a menudo por la avenida.\n\n");
salida("oeste",ROOMS+"ciudad_82.c");
salida("este",ROOMS+"ciudad_84.c");
}

