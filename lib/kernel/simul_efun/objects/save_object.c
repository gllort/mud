
varargs int save_object(string name, int flag) {
    int res;
    string err;
    
    err = catch(res = efun::save_object(name, flag));
    if (err) {
	write("save_object: "+name+": Acceso denegado.\n");
    }    
    return res;    
}
