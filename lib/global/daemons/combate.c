#include <mudlib.h>
#include <daemons.h>
#include <daños.h>
#include <tablas.h>
#include <mensajes.h>

inherit DAEMON;

private nosave mapping iniciativas = ([ ]);
private nosave object * en_lucha = ({ });
private nosave object * pendientes = ({ });

void ejecutar_ataque(object);
void unir_pendientes_a_lucha();
void start_fight(object, object);
int calcular_iniciativa(object);

void start() { 
    set_heart_beat(1);
}

void stop() { ; }


void heart_beat() {
    int * lista_iniciativas;

    /* Unimos a la lucha a los combatientes pendientes */    
    if (sizeof(pendientes) != 0) {
	unir_pendientes_a_lucha();
    }
    lista_iniciativas = (int *)keys(iniciativas);
    lista_iniciativas = sort_array(lista_iniciativas, -1);
    foreach(int iniciativa in lista_iniciativas) {
	foreach(object atacante in iniciativas[iniciativa]) {
    	    ejecutar_ataque(atacante);
	}
    }    
    foreach(object luchador in en_lucha) {
	tell_object(luchador, "\n");
    }    
}

int calcular_iniciativa(object personaje) {
    /* Aplicar bonificadores del personaje */
    return DADO->lanzar(1,20);
}

void start_fight(object atacante, object victima) {
    /* Los nuevos combatientes entraran en lucha en el proximo heart beat,
     * para no interferir en el transcurso del turno actual.
     */
    if ((member_array(atacante, en_lucha) == -1) && (member_array(atacante, pendientes) == -1)) {
	pendientes += ({ atacante });
    }
    if (member_array(victima, en_lucha) == -1) {
	pendientes += ({ victima });
    }
}

void unir_pendientes_a_lucha() {
    int iniciativa;
    
    foreach(object atacante in pendientes) {
	en_lucha += ({ atacante });
	iniciativa = calcular_iniciativa(atacante);

	/* En casos de empate, deberiamos ordenar por quien tiene mas BONUS */
	if (!iniciativas[iniciativa]) {
	    iniciativas[iniciativa] = ({ atacante });
	}
	else {
	    iniciativas[iniciativa] += ({ atacante });
	}
	pendientes -= ({ atacante });    
    }
}

void ejecutar_ataque(object atacante) {
    int armado, ac, daño, * ataques;
    string * tipo_daño;
    object victima, * candidatos, * armas, arma;
    class mensaje msg;
    
    /* Escogemos la victima del ataque */
    candidatos = atacante->atacando?();
    victima = candidatos[random(sizeof(candidatos))];

    /* Recuperamos las armas que estamos empuñando */
    armado = sizeof(armas = atacante -> empuñando?());

    /* Resolvemos la lista de ataques y la AC de la victima */
    ataques = DADO -> tiradas_ataque(atacante); 
    ac = 10 + min(victima->mod_des?(), victima->max_mod_des?()) + victima->ac?();

    foreach(int ataque in ataques) {
	write("<att "+ataque+" vs ac "+ac+"\n");	

	if (ataque >= ac) {
	    /* El atacante impacta */
	    if (!armado) {
		daño = DADO -> lanzar(1,3);

		msg = atacante -> mensaje_combate_desarmado?(random(5)+1);		
	    }
	    else {
	    	arma = armas[random(armado)];
		daño = DADO -> lanzar(arma->tiradas?(), arma->dado?());
		tipo_daño = arma -> tipo_daño?();
	    }
	    
	    tell_object(atacante, replace_string(msg->texto_yo, "$Z", victima->zona_impacto?())+".\n");
	    tell_object(victima, msg->texto_el+".\n");
	    tell_room(ENV(atacante), msg->texto_otros+".\n", ({ atacante, victima }));
	}
	else {
	    /* La victima esquiva el ataque */
	
	}
    }    
}    
    
    
    /*
    
    
    if (!armado) {
	/* Ataque desarmado 
        tell_object(atacante, "Atacas a "+victimas[random(sizeof(victimas))]->nombre?()+"\n");
	tell_object(victima, atacante->nombre?()+" te ataca.\n");
    }
    else {
	/* Escogemos una de las armas para realizar el ataque 
    	arma = armas[random(armado)];





/*
	tipo_daño = arma -> tipo_daño?();
	causa = tipo_daño[0]; 

	msgs = TABLA_DAÑOS -> mensajes_combate(causa);
	rd = random(sizeof(msgs));
    
	msg_atacante = msgs[rd][1];
	msg_victima  = msgs[rd][2];

	msg_atacante = replace_string(msg_atacante, "$atacante$", capitalize(atacante->nombre?()));
	msg_atacante = replace_string(msg_atacante, "$victima$", capitalize(victima->nombre?()));
	msg_atacante = replace_string(msg_atacante, "$arma$", arma->nombre?());
	msg_atacante = replace_string(msg_atacante, "$detarma$", arma->determinante?()+" "+arma->nombre?());
	msg_atacante = replace_string(msg_atacante, "$detzona$", victima->impactar_parte(msgs[rd][0]));

	msg_victima = replace_string(msg_victima, "$atacante$", capitalize(atacante->nombre?()));
	msg_victima = replace_string(msg_victima, "$victima$", capitalize(victima->nombre?()));
	msg_victima = replace_string(msg_victima, "$arma$", arma->nombre?());
	msg_victima = replace_string(msg_victima, "$detarma$", arma->determinante?()+" "+arma->nombre?());
	msg_atacante = replace_string(msg_victima, "$detzona$", victima->impactar_parte(msgs[rd][0]));
	
	tell_object(atacante, msg_atacante+"\n");
	tell_object(victima, msg_victima+"\n");
*/


    