#include <comando.h>

int comando(int argc, string * argv) {
    write("Funcionando desde hace "+uptime()+" segundos.\n");
    write("Carga media: "+query_load_average()+".\n");
    return 1;
}
