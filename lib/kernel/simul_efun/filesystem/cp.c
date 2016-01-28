#include <errno.h>

int cp(string origen, string destino) {
    int res;
    string err;
    
    err = catch(res = efun::cp(origen, destino));
    if (err) write("cp: "+serror(err)+"\n");

    err = catch(PERMISOS -> crear(destino, TP->nombre?()));
    if (err) write("cp: "+destino+": "+serror(err)+"\n");
    
    return res;
}
