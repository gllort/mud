#include <mudlib.h>

inherit DAEMON;

void start() { ; }
void stop() { ; }

int lanzar(int tiradas, int dado) {
    int i, puntuacion = 0;
    for(i=tiradas; i>0; i--) {
	puntuacion += (random(dado)+1);    
    }
    return puntuacion;
}

int * tiradas_ataque(object player) {
    int ab, * tiradas = ({ });
    
    ab = player -> ataque_base?();
    while (ab > 0) {
	tiradas += ({ lanzar(1,20) + ab + player -> mod_fue?() });
	ab -= 5;
    }
    return tiradas;
}
