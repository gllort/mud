#include <comando.h>
#include <mudlib.h>

int comando(int argc, string * argv) {
    int logins = 0;
    string * names = ({ });

    foreach(object user in users()) {
	if (file_name(user) == LOGIN) logins++;
	else names += ({ capitalize(user->nombre?()) });
    }
    names = sort_array(names, 1);
    write(implode(names, "\n")+"\n");
    if (logins > 0) {
	if (logins > 1) write(logins+" usuarios en login.\n");
	else write(logins+" usuario en login.\n");
    }
    return 1; 
}
