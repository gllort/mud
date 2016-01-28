#include <mudlib.h>
#include <config.h>
#include <errno.h>

inherit DAEMON;

void start() { /* Skip */ } 

void stop()  { /* Skip */ }

int uid?(string user) {
    int res, bd, rows;
    string err;

    user = lower_case(user);
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	rows = db_exec(bd, "SELECT uid FROM inmortales WHERE nombre='"+user+"';");
	if (!rows) { err = E1(ERR_NO_INMORTAL,user); }
	else if (rows == -1) err = db_error(bd);
	else res = db_fetch(bd, 1)[0];
    }
    db_close(bd);
    if (err) throw(err);
    return res;
}

int gid?(string group) {
    int res, bd, rows;
    string err;

    group = lower_case(group);
    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	rows = db_exec(bd, "SELECT gid FROM grupos WHERE grupo='"+group+"';");
	if (!rows) { err = E1(ERR_NO_GROUP,group); }
	else if (rows == -1) err = db_error(bd);
	else res = db_fetch(bd, 1)[0];
    }
    db_close(bd);
    if (err) throw(err);
    return res;
}

string inmortal?(int uid) {
    int bd, rows;
    string res, err;

    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	rows = db_exec(bd, "SELECT nombre FROM inmortales WHERE uid='"+uid+"';");
	if (!rows) { err = E1(ERR_NO_INMORTAL,uid); }
	else if (rows == -1) err = db_error(bd);
	else res = db_fetch(bd, 1)[0];
    }
    db_close(bd);
    if (err) throw(err);
    return res;
}

string grupo?(int gid) {
    int bd, rows;
    string res, err;

    if ((bd = db_connect(BD_HOST, BD_NAME, BD_USER)) < 1) err = ERR_DB_CONNECT;
    else {
	rows = db_exec(bd, "SELECT grupo FROM grupos WHERE gid='"+gid+"';");
	if (!rows) { err = E1(ERR_NO_GROUP,gid); }
	else if (rows == -1) err = db_error(bd);
	else res = db_fetch(bd, 1)[0];
    }
    db_close(bd);
    if (err) throw(err);
    return res;
}


