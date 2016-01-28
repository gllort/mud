
int mkdir(string newdir) {
    int i, size;
    string previo, * path;

    if ((strsrch(newdir,"*")!=-1)||(strsrch(newdir,"?")!=-1)||(strsrch(newdir,".")!=-1)) {
	write("mkdir efun: "+newdir+": Uno o mas caracteres invalidos.\n");    
	return 0;
    }
    previo = "/";
    size = sizeof(path = explode(newdir,"/"));
    for (i=0; i<size; i++) {
        if (!find_dir(previo+path[i])) 
    	    if (!efun::mkdir(previo+path[i])) 
		return 0;
	previo += path[i]+"/";
    }
    return 1;
}
