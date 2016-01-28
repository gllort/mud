#include <mudlib.h>
#include <daemons.h>
#include <dirs.h>
#include <errno.h>
#include <config.h>
#include <permisos.h>

inherit DAEMON;

private nosave mapping dominios = ([ ]);

class dominio {
    string owner;
    int owner_id;
}

void start() { 
    int uid;
    object dom;
    string * doms, * path, err, domname;
    
    doms = explore_path(DIR_DOMINIOS+"/*",1);
    foreach(string dominio in doms) {
	path = explode(dominio,"/");
	domname = path[sizeof(path)-1];

	dom = clone_object(DOMINIO);
	if (!dom) throw("Error cargando objeto dominio "+DOMINIO+".\n");
	err = catch(dom -> cargar(path[sizeof(path)-1]));
	if (err) throw(err);

	dominios[domname] = new (class dominio);
	dominios[domname] -> owner = dom -> domainlord?();
	dominios[domname] -> owner_id = dom -> domainlord_uid?();
    }
} 

void stop()  { ; }

int es_dominio?(string dominio) { 
    if (member_array(dominio, keys(dominios)) == -1) return 0;
    else return 1;
}

void crear(string domain) {
    string err;
    object dominio;

    if (!adminp()) return;
    
    domain = lower_case(domain);
    if (es_dominio?(domain)) throw(E1(ERR_DUP_DOMAIN, domain));

    dominio = new (DOMINIO, domain);
    if (!dominio) err = "Error creando dominio.\n";

    err = catch(GRUPOS -> crear(domain));
    if (err) {
	destruct(dominio);
    	throw(err);
    }

    if (mkdir(DIR_DOMINIOS+"/"+domain) && 
	mkdir(DIR_DOMINIOS+"/"+domain+"/open")   &&
        mkdir(DIR_DOMINIOS+"/"+domain+"/config") &&
	mkdir(DIR_DOMINIOS+"/"+domain+"/items")  &&
	mkdir(DIR_DOMINIOS+"/"+domain+"/rooms")  &&
	mkdir(DIR_DOMINIOS+"/"+domain+"/npcs")) 
    {
	err = catch(PERMISOS -> grant(DIR_DOMINIOS+"/"+domain, domain, R));
	if (!err) 
	    err = catch(PERMISOS -> grant(DIR_DOMINIOS+"/"+domain+"/open", domain, RWC));
	if (!err)	
    	    err = catch(dominio -> domainlord(TP->nombre?()));
    }
    else err = "Error creando directorios de dominio.";
    if (err) {
	destruct(dominio);
	throw(err);
    }
    dominios[dominio] = new(class dominio);
    dominios[dominio] -> owner = TP -> nombre?();
    dominios[dominio] -> owner_id = USERS -> uid?(dominios[dominio] -> owner);
}

string * dominios?() { return keys(dominios); }

string domainlord?(string domain) {
    return dominios[domain] -> owner;
}
