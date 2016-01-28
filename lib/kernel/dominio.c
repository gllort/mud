#include <daemons.h>
#include <errno.h>
#include <dirs.h>

private nosave string dominio;
private string domainlord;
private int domainlord_uid;

void save_me();

void create(string domain) { 
    dominio = domain;
}

void cargar(string domain) {
    /* Cuando hacemos restore_object de un dominio que ya existe */
    if (find_file(DIR_DOMINIOS+"/"+domain+"/config/"+domain+".o")) {
	restore_object(DIR_DOMINIOS+"/"+domain+"/config/"+domain+".o");
    }
    else throw(E1(ERR_NO_DOMAIN,domain));
}

void domainlord(string lord) {
    string err;
    
    err = catch(domainlord_uid = USERS -> uid?(lord));
    if (err) throw(err);
    domainlord = lower_case(lord);
    save_me();
}

string domainlord?() { return domainlord; }
int domainlord_uid?() { return domainlord_uid; }
string dname?() { return dominio; }

void save_me() {
    save_object(DIR_DOMINIOS+"/"+dominio+"/config/"+dominio+".o");    
}

void dest_me() {
    destruct(TO);
}
