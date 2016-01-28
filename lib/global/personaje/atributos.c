#include <bonus.h>
#include <mudlib.h>

private int fuerza, destreza, constitucion, inteligencia, sabiduria, carisma;
private nosave int bonus_fuerza, bonus_destreza, bonus_constitucion, bonus_inteligencia, bonus_sabiduria, bonus_carisma;
private nosave int suerte, bonus_suerte;

private int xp, xp_total;

private int pv, pv_max, pe, pe_max;

void create() {
    fuerza = random(8)+1;
    destreza = random(8)+1;
    constitucion = random(8)+1;
    inteligencia = random(8)+1;
    sabiduria = random(8)+1;
    carisma = random(8)+1;
    suerte = random(18)+1;
    xp = xp_total = 0;
    pv = pv_max = pe = pe_max = 1;
}

nomask void fue(int x) { fuerza = x; }
nomask void des(int x) { destreza = x; }
nomask void con(int x) { constitucion = x; }
nomask void tel(int x) { inteligencia = x; }
nomask void sab(int x) { sabiduria = x; }
nomask void car(int x) { carisma = x; }
nomask void sue(int x) { suerte = x; }

nomask int fue?() { return fuerza + bonus_fuerza; }
nomask int des?() { return destreza + bonus_destreza; }
nomask int con?() { return constitucion + bonus_constitucion; }
nomask int tel?() { return inteligencia + bonus_inteligencia; }
nomask int sab?() { return sabiduria + bonus_sabiduria; }
nomask int car?() { return carisma + bonus_carisma; }
nomask int sue?() { return suerte + bonus_suerte; }

nomask int base_fue?() { return fuerza; }
nomask int base_des?() { return destreza; }
nomask int base_con?() { return constitucion; }
nomask int base_tel?() { return inteligencia; }
nomask int base_sab?() { return sabiduria; }
nomask int base_car?() { return carisma; }
nomask int base_sue?() { return suerte; }

nomask void bonus_fue(int x) { bonus_fuerza += x; }
nomask void bonus_des(int x) { bonus_destreza += x; }
nomask void bonus_con(int x) { bonus_constitucion += x; }
nomask void bonus_tel(int x) { bonus_inteligencia += x; }
nomask void bonus_sab(int x) { bonus_sabiduria += x; }
nomask void bonus_car(int x) { bonus_carisma += x; }
nomask void bonus_sue(int x) { bonus_suerte += x; }

nomask int bonus_fue?() { return bonus_fuerza; }
nomask int bonus_des?() { return bonus_destreza; }
nomask int bonus_con?() { return bonus_constitucion; }
nomask int bonus_tel?() { return bonus_inteligencia; }
nomask int bonus_sab?() { return bonus_sabiduria; }
nomask int bonus_car?() { return bonus_carisma; }
nomask int bonus_sue?() { return bonus_suerte; }

private nomask int modificador_habilidad?(int valor) {
    return -5 + (valor / 2); 
}

nomask int mod_fue?() { return modificador_habilidad?(fue?()); }
nomask int mod_des?() { return modificador_habilidad?(des?()); }
nomask int mod_con?() { return modificador_habilidad?(con?()); }
nomask int mod_tel?() { return modificador_habilidad?(tel?()); }
nomask int mod_sab?() { return modificador_habilidad?(sab?()); }
nomask int mod_car?() { return modificador_habilidad?(car?()); }
nomask int mod_sue?() { return modificador_habilidad?(sue?()); }

nomask int max_mod_des?() {
    int mmd = MAXIMO_MODIFICADOR_DESTREZA;
    object * equipo;

    equipo = TO -> vistiendo?();
    foreach(object item in equipo) {
	if (inherits(ARMADURA, item)) {
	    if (mmd > item -> max_mod_des?()) {
		mmd = item -> max_mod_des?();
	    }
	}
    }
    return mmd;
}


int xp?() { return xp; }
int xp_total?() { return xp_total; }
void suma_xp(int puntos) {
    xp += puntos;
    xp_total += puntos;
}
void resta_xp(int puntos) {
    xp -= puntos;
    xp_total -= puntos;
}
void gasta_xp(int puntos) {
    xp -= puntos;
}

int pv?() { return pv; }
int pv_max?() { return pv_max; }
int pe?() { return pe; }
int pe_max?() { return pe_max; }
void suma_pv(int puntos) {
    pv += puntos;
    if (pv < 0) pv = 0;
    else if (pv > pv_max) pv = pv_max;
}
void suma_pe(int puntos) {
    pe += puntos;
    if (pe < 0) pe = 0;
    else if (pe > pv_max) pe = pe_max;
}
void suma_pv_max(int puntos) {
    pv_max += puntos;
    pv += puntos;
    if (pv_max < 0) pv = pv_max = 1;
}
void suma_pe_max(int puntos) {
    pe_max += puntos;
    pe += puntos;
    if (pe_max < 0) pe = pe_max = 1;
}
