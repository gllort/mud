#include <daemons.h>

int nocalls(object ob, object * apo, int ref i, int size) {
    if (((ob == TO) && (i+2<size) && (file_name(apo[i+1]) == CALLER) && (apo[i+2] == TO)) || 
        ((ob == TO) && (i-2 >= 0) && (file_name(apo[i-1]) == CALLER) && (apo[i-2] == TO)) || 
        ((file_name(ob) == CALLER) && (i-1 >= 0) && (i+1<size) && (apo[i-1] == TO) && (apo[i+1] == TO))) {
	    i++;
	    return 0; 
    }
    i++;
    return 1;
}

object * all_previous_objects() {
    int i = 0, size;
    object * apo, * obs;
    size = sizeof(apo = efun::all_previous_objects());
    obs = filter_array(apo, "nocalls", TO, apo, ref i, size);
    if (origin() == "local") return obs;
    else {
        size = sizeof(obs);
        if (size > 0) return obs[1..size-1];
	else return ({ });
    }
}
