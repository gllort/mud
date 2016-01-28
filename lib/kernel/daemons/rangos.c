#include <mudlib.h>
#include <config.h>
#include <errno.h>

inherit DAEMON;

private nosave mapping jerarquia = ([ ]);

int crear(string, int);

/* Servicios start & stop */

private void start() {
    string err; 
    int i, bd, rows;

    jerarquia = ([ ]);
    /* Cargamos la jerarquia de inmortales en memoria */
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	rows = db_exec(bd, "SELECT * FROM rangos;");
	if (rows >= 0) {
	    for (i=1; i<=rows; i++) jerarquia[db_fetch(bd,i)[0]] = db_fetch(bd,i)[1];
	    if (!sizeof(keys(jerarquia))) {
		/* Primer arranque del mud */
		if (err = catch(crear(ADMIN_RANK, ADMIN_LEVEL))) {
		    jerarquia = ([ ]);
		}
	    }
	}
	else err = db_error(bd);
    }
    db_close(bd);
    if (err) throw(err);
    jerarquia[PLAYER_RANK] = 0;
}

private void stop() { /* Skip */ } 

int es_rango?(string rank) { 
    return (member_array(lower_case(rank), keys(jerarquia)) == -1 ? 0 : 1); 
}

int es_nivel?(int level) { 
    return (member_array(level, values(jerarquia)) == -1 ? 0 : 1); 
}

string rango(int nivel) {
    int idx;
    nivel = to_int(nivel);
    idx = member_array(nivel, values(jerarquia));
    return (idx == -1 ? PLAYER_RANK : keys(jerarquia)[idx]);
}

int nivel(string rango) {
    int idx;
    idx = member_array(rango, keys(jerarquia));    
    return (idx == -1 ? 0 : values(jerarquia)[idx]);
}

int nivel_maximo?() { 
    return sort_array(values(jerarquia), -1)[0]; 
}

int nivel_minimo?() { 
    return sort_array(values(jerarquia), 1)[0]; 
}

string rango_maximo?() { 
    return rango(nivel_maximo?()); 
}

string rango_minimo?() { 
    return rango(nivel_minimo?()); 
}

string * rangos?() {
    return keys(jerarquia);
}

int * niveles?() {
    return values(jerarquia);
}

protected nomask int f_jerarquia(string key, int value, int limit, int superior) {
    if (superior) {
	if (value > limit) return 1;
	else return 0;
    }
    else {
	if (value < limit) return 1;
	else return 0;
    }
}

private mixed ordenar(mixed base, int o1, int o2) {
    mapping map = ([ ]);
    int * niveles, nivel, i, result_str = 0;
    
    nivel = to_int(base);
    if ((!nivel) && (stringp(base))) {
	result_str = 1;
	base = lower_case(base);
	nivel = nivel((string)base);
    }
    map = filter_mapping(jerarquia, "f_jerarquia", TO, nivel, o1);
    if ((!map) || (map == ([ ])) || (!sizeof(keys(map)))) return base;
    else {
	nivel = sort_array(values(map), o2)[0];
	if (result_str) return rango(nivel);
	else return nivel;
    }
}

mixed inferior(mixed base) {
    return ordenar(base, 0, -1);
}

mixed superior(mixed base) {
    return ordenar(base, 1, 1);
}

int crear(string rank, int level) {
    int gid;
    string err;
    
    rank = lower_case(rank);
    if (es_rango?(rank)) throw(E1(ERR_DUP_RANK,rank));
    if (es_nivel?(level)) throw(E1(ERR_DUP_LEVEL,level));
    if (err = catch(gid = GRUPOS -> crear(rank, level))) throw(err);
    jerarquia[rank] = level;
    return gid;
}

void eliminar(string rank) {
    int bd, rows, lvl;
    string err;
        
    rank = lower_case(rank);
    if (!es_rango?(rank)) throw(E1(ERR_NO_RANK, rank));
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;            
    else {
	if (!(err = catch(GRUPOS -> eliminar(rank)))) {
	    /* Se han borrado entradas de grupos, rangos y canales de la BD
	     * mediante los ON DELETE CASCADE que referencian a grupos.
	     * Los inmortales de ese rango tambien han sido eliminados
	     * (Referencias ON DELETE de rangos). Si alguno de ellos 
	     * estaba conectado, nos lo cargamos */

	    lvl = nivel(rank);
	    foreach(object user in users()) {
		if (user -> nivel_inmortal?() == lvl) {
		    tell_object(user, "Has sido degradado.\n");
		    destruct(user);		    
		} 
	    }	  
	    map_delete(jerarquia, rank);  	    
	}
    }    
    db_close(bd);
    if (err) throw(err);
}

string rango_inmortal(string user) {
    int bd, rows;
    string err, res;
    
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	user = lower_case(user);
	rows = db_exec(bd, "SELECT * FROM inmortales WHERE nombre='"+user+"';");
	if (!rows) err = E1(ERR_NO_INMORTAL,user);
	else if (rows == -1) err = db_error(bd);
	else res = db_fetch(bd,1)[2];
    }
    if (err) throw(err);
    return res;
}

int nivel_inmortal(string user) {
    string err, rank;
    
    if (err = catch(rank = rango_inmortal(user))) throw(err);
    return nivel(rank);
}
