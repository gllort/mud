
inherit BASEROOM;
#include "path.h"

void habitacion() {


nombre("$N$0Zorimeth:$R$4 Avenida de Lloth$R");
descripcion("A ambos lados de la calle puedes ver edificaciones hechas aprovechando "+
			"las estalagmitas que se alzan hasta 30 o 40 metros del "+
			"suelo. Aqu� se encuentran la mayor�a de las casas menores de Zorimeth, "+
			"la mayoria creadas por hijas de madres matronas de grandes casas. Por "+
			"la calle puedes ver muchos drows as� como algunas hileras de esclavos "+
			"dirigi�ndose a sus casas despu�s de haber hecho alguna misi�n superflua.\n\n");
salida("oeste",ROOMS+"ciudad_18.c");
salida("sureste",ROOMS+"ciudad_27.c");
salida("sur",ROOMS+"ciudad_26.c");
}

