
inherit BASEROOM;
#include "path.h"

void habitacion() {


nombre("$N$0Zorimeth:$R$3 Narbondel$R");
descripcion("Ante ti se alza el imponente Narbondel, un inmenso pilar de piedra decorado con extra�os s�mbolos "+
			"drow. Este es el punto central de la ciudad de Zorimeth, decenas de drows pasan por aqui diariamente."+
			"Cada d�a el maese de los hechizeros lanza un conjuro sobre el pilar que lo mantiene ligeramente "+
			"iluminado un d�a entero de la superficie.\n\n");
/*
add_item("narbondel","\nEste enorme pilar de piedra es el �nico modo que poseen los drows de tener constancia de paso "+
		"del tiempo. Gracias al hechizo que diariamente el maese de los hechizeros lanza sobre el pilar, ""una leve luminosidad recorre el pilar manteniendose durante un dia completo de la superficie. ""Gracias a esto, los drows son capaces de tener constancia del paso de los dias.\n\n");
*/
salida("oeste",ROOMS+"ciudad_84.c");
salida("norte",ROOMS+"ciudad_67.c");
salida("este",ROOMS+"ciudad_86.c");
salida("sur",ROOMS+"ciudad_104.c");
}

