#include <triggers.h>

private nosave mapping triggers = ([ ]);

void create() { ; }

void add_trigger(string event, function action) {
    if (triggers[event]) {
	triggers[event] -= ({ action });
	triggers[event] += ({ action });
    }
    else {
	triggers[event] = ({ action });
    }
}

void del_trigger(string event, function action) {
    if (triggers[event])
	triggers[event] -= ({ action });
    if (!sizeof(triggers[event]))
	map_delete(triggers, event);
}

varargs mixed call_triggers(string event, int response, array args...) {
    mixed res;
    array actions;
    function action;
    
    actions = triggers[event];
    
    if (!sizeof(actions)) return 0;

    response = to_int(response);
    switch(response) {    
	case TRIGGER_IGNORE:
	    map(actions, (: evaluate($1, $(args)...) :));
	    return 0;
	case TRIGGER_SUM:
	    foreach(action in actions)
    		res += evaluate(action, args...);
	    return res;
	case TRIGGER_AND:
	    foreach(action in actions)
    		if (!evaluate(action, args...)) return 0;
	    return 1;
	case TRIGGER_OR:
	    foreach(action in actions)
    		if (evaluate(action, args...)) return 1;
	    return 0;
	default: error("Tipo de respuesta desconocido en call_triggers().");
    }
    	    
}			
