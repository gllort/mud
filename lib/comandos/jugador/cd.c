#include <comando.h>
#include <dirs.h>

int comando(int argc, string * argv) {
    string nwd;

    nwd = (!argc ? DIR_HOME+"/"+TP->nombre?() : argv[0]);    
    return TP->cwd(nwd);
}
