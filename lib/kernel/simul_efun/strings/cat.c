#include <errno.h>

void cat(string fichero) {
    string err, texto;
    
    err = catch(texto = read_file(fichero));
    if (err) write("cat: "+serror(err)+"\n");
    else write(texto);
}
