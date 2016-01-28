#include <forma.h>

void forma() {
    forma_cuerpo(({    
	({ "cabeza",1,F,S }), 
	({ "orejas",2,F,P }),
    	({ "ojos",1,M,P }),
	({ "cara",1,F,S }),
	({ "cuello",3,M,S }),
	({ "pecho",1,M,S }),
	({ "espalda",1,F,S }),
	({ "brazos",1,M,P,({ "brazo izquierdo", "brazo derecho" }) }),
	({ "mu�ecas",1,F,P }),
	({ "manos",1,F,P }),
	({ "dedos",3,M,P }),
	({ "cintura",1,F,S }),
	({ "piernas",1,F,P }),
	({ "pies",1,M,P }),
    }));
    items_sostenibles(2);
}

void mensajes() {
    MENSAJE(LEVE,APLASTANTE,"Aranyas debilmente $Z de $V","$A te aranya debilmente $Z", "$A aranya debilmente $Z de $V");
    MENSAJE(LEVE,APLASTANTE,"Sacudes un codazo a $V", "$A te sacude un codazo", "$A sacude un codazo a $V");
    MENSAJE(MEDIO,APLASTANTE,"Asestas un punyetazo en $Z de $V","$A te asesta un punyetazo en $Z", "$A asesta un punyetazo en $Z de $V");
    MENSAJE(MEDIO,APLASTANTE,"Lanzas un potente rodillazo contra $Z de $A", "$A te propina un potente rodillazo en $Z", "$A propina un potente rodillazo en $Z de $V");
    MENSAJE(CRITICO,APLASTANTE, "Pateas con fuerza $Z de $V", "$A te patea $Z con fuerza", "$A patea con fuerza $Z de $V");
    MENSAJE(CRITICO,APLASTANTE, "Embistes salvajemente a $V", "$A te embiste salvajemente", "$A embiste salvajemente a $V");
    MENSAJE(MORTAL,APLASTANTE,"Agarras la cabeza de $V con las manos y le rompes el cuello en una sacudida brutal","$A te agarra la cabeza con las manos y te sacude brutalmente hasta romperte el cuello", "$A agarra la cabeza de $V con las manos y le rompe el cuello de una brutal sacudida");
}
