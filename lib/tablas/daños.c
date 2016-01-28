#include <da�os.h>

int tipo_da�o_valido?(string tipo) {
    switch(tipo) {
	case CORTANTE:
	case APLASTANTE:
	case PERFORANTE:
	    return 1;
	default:
	    return 0;
    }
    return 0;
}

mixed * mensajes_combate(string tipo) {
    switch(tipo) {
	case CORTANTE: return ({
	    ({ PIFIA  , "Pierdes el equilibrio y te pinchas con $detarma$.", "$atacante$ pierde el equilibrio y se pincha con su $arma$.", "$atacante$ pierde el equilibrio y se pincha con su $arma$." }),
	    ({ LEVE   , "Cortas superficialmente a $victima$ en $detzona$.", "$atacante$ te produce un corte superficial en $detzona$.", "$atacante$ produce un corte superficial en $detzona$ de $victima$." }),
	    ({ MEDIO  , "Rajas $detzona$ de $victima$ de un certero tajo.", "$atacante$ te raja $detzona$ de un certero tajo.", "$atacante$ raja $detzona$ de $victima$ de un certero tajo. " }),
	    ({ CRITICO, "Acuchillas salvajemente $detzona$ de $victima$ seccionandole musculos y tendones.", "$atacante$ te achuchilla salvajemente $detzona$ seccionandote musculos y tendones.", "$atacante$ acuchilla salvajemente $detzona$ de $victima$ seccionandole musculos y tendones." }),
	    ({ MUERTE , "Hundes tu $arma$ en el corazon de $victima$ hasta la empunyadura!", "$atacante$ hunde su $arma$ en tu corazon hasta la empunyadura!", "$atacante$ hunde su $arma$ en el corazon de $victima$ hasta la empunyadura!" }),
	});
	    
    	case APLASTANTE: return ({ });
		
	case PERFORANTE: return ({ });

	default: return ({ });

    }
}

