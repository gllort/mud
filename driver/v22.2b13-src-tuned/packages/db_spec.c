#include "spec.h"

int db_close(int);
int db_commit(int);
int db_connect(string, string, string|void, int|void);
int db_exec(int, string);
mixed *db_fetch(int, int);
int db_rollback(int);
string db_status(void);
int db_affected_rows(int);
int db_errno(int);
string db_error(int);
int db_insert_id(int);
