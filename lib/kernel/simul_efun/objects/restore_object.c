
varargs int restore_object(string name, int flag) {
    int res;
    string err;
    
    err = catch(res = efun::restore_object(name, flag));
    if (err) {
	write("restore_object: "+name+": Acceso denegado.\n");
    }    
    return res;    
}
