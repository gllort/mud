#include <mudlib.h>
#include <daemons.h>

int inmortalp() {
    if (TP) {
	if ((file_name(TP) == INMORTAL) && (TP->nivel_inmortal?() > 0))
	    return 1;
	else
	    return 0;
    }
    else {
	write("INMORTALP no es player\n");
	return 0;
    }
}
