
string relative_name(mixed ob) {
    string * path;
    if (objectp(ob)) 
	path = explode(file_name(ob),"/");
    else if (stringp(ob)) 
	path = explode(ob,"/");
    else 
	error("Object or array expected.\n"); 
    return path[sizeof(path)-1];
}
