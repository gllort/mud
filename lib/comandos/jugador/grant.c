#include <comando.h>
#include <daemons.h>

int comando(int argc, string * argv) {


    PERMISOS -> grant(argv[0], argv[1], to_int(argv[2]));
    return 1;
}
