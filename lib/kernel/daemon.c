#include <errno.h>

void start() { }

void stop() { }

private void create() { 
    string err;
    if (err = catch(start())) {
	throw("*"+serror(E1(ERR_LOADING_DAEMON,file_name(TO)))+"\n" + serror(err)+"\n");
    }
}

int clean_up(int inherited) {
    return 0;
}

