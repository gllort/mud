#include <tablas.h>

private mapping skills = ([ ]);

int add_skill(string skill) {
    skill = lower_case(skill);
    if (!TABLA_SKILLS -> skill?(skill)) return 0;
    skills[skill] = 1;
    return 1;
}

string skill?(string skill) {
    return (skills[skill] ? 1 : 0);
}

int nivel_skill?(string skill) {
    return (skills[skill] ? skills[skill] : 0);
}

int nivel_maximo_skill?(string skill) {
    int nivel;
    
    nivel = TP -> nivel?();
    if (TP -> skill_favorita?(skill)) 
        nivel += 3;
    else 
	nivel = nivel / 2;
    return nivel;
}

int coste_skill?(string skill) {
    int coste;
    
    if (!skills[skill]) return 0;
    coste = TABLA_SKILLS -> coste(skills[skill] + 1);    
    if (!TP -> skill_favorita?(skill)) coste *= 2;
    return coste;
}

string * skills?() {
    return keys(skills);
}

void avanzar_skill(string skill) {
    if (!skills[skill]) return;
    skills[skill] += 1;
}

