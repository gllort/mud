#include <sexo.h>

private int sexo;

void create() { 
    sexo = ASEXUAL;
}

nomask void sexo(int s) { 
    if ((s != VARON) && (s != HEMBRA) && (s != ASEXUAL)) sexo = ASEXUAL;
    else sexo = s; 
}

nomask int sexo?() { return sexo; }

nomask int varon?() { return (sexo == VARON ? 1 : 0); }

nomask int hembra?() { return (sexo == HEMBRA ? 1 : 0); }

nomask int asexual?() { return (sexo == ASEXUAL ? 1 : 0); }
