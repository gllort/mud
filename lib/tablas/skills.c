#include <skills.h>

private nosave mapping skills = ([ ]);

void create() {
    skills["identificar"] = INT;
    skills["equilibrio"] = DEX;
    skills["enga�ar"] = CHA;
    skills["escalar"] = STR;
    skills["concentracion"] = CON;
}

void debug() {
    printf("%O\n", skills);
}

int skill?(string skill) {
    return (member_array(skill, keys(skills)) == -1 ? 0 : 1);
}

int coste(int nivel) {
    int i, coste;
    for (i=1; i<=nivel; i++) {
	coste = coste + i*1000;
    }
    return coste;
}
