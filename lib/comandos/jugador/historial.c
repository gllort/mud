#include <comando.h>

int comando(int argc, string * argv) {
    int index = 1;
    string * history;
        
    history = TP -> historial?();
    foreach(string cmd in history) {
        write(index+": "+cmd+"\n");
        index++;
    }
    return 1; 
}
