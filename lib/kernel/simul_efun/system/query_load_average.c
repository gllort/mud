
string query_load_average() {
    string str; 
    str = efun::query_load_average();
    str = replace_string(str, "cmds/s", "comandos/segundo");
    str = replace_string(str, "comp lines/s", "lineas compiladas/segundo");
    return str;
}
