
inherit BASEROOM;
#include "path.h"

void habitacion() {



nombre("$N$0Zorimeth:$R$1 Plaza del Narbondel$R");
descripcion("Esta es la plaza central de Zorimeth. Normalmente hay mucho movimiento en esta zona. "+
			"En el centro de la plaza, tal como indica su nombre, se encuentra el Narbondel. Ese enorme "+
			"pilar es la unica manera que tienen los drows de tener constancia del paso del tiempo en su "+
			"siempre oscuro mundo subterraneo.\n\n");
salida("oeste",ROOMS+"ciudad_83.c");
salida("noreste",ROOMS+"ciudad_67.c");
salida("este",ROOMS+"ciudad_85.c");
salida("sureste",ROOMS+"ciudad_104.c");
}

