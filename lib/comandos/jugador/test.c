#include <comando.h>

varargs void test(int a, int b) {

    write(a+"\n");
    write(b+"\n");
    
}

int comando(int argc, string * argv) {

    test(argv...);
}

