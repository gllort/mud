#include <comando.h>

int comando(int argc, string * argv) {
    write("Driver "+__VERSION__+"\n");
    write(LIB_VERSION+"\n");
    return 1;
}
