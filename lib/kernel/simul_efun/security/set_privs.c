#include <mudlib.h>

void set_privs(object ob, string privs) {
    if (inherits(INMORTAL, PO(0))>0) efun::set_privs(ob, privs);
    else return;
}
