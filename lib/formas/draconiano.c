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
	({ "alas",1,F,P }),
	({ "cola",1,F,S }),
	({ "brazos",1,M,P,({ "brazo izquierdo", "brazo derecho" }) }),
	({ "mu�ecas",1,F,P }),
	({ "garras",1,F,P }),
	({ "dedos",3,M,P }),
	({ "cintura",1,F,S }),
	({ "piernas",1,F,P }),
	({ "pies",1,M,P }),
    }));
    items_sostenibles(2);
}
