#include <config.h>

private nosave string * history = ({ });

protected nomask void add_history(string cmd) {
    if (sizeof(history) < HISTORY_SIZE) 
	history += ({ cmd });
    else {
	history -= ({ history[0] });
	history += ({ cmd });
    }
}

string * historial?() {
    return history;
}

