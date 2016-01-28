#define ONLY_DIRS	1
#define ONLY_FILES	2

private void do_explore_path(string *, int, string, string ref *, int);
string real_path(string, string);

varargs string * explore_path(string str, int flag) {
    string finalpath, basedir, * paths = ({ });

    if (TP) basedir = (TP->cwd?() == "/" ? "/" : TP->cwd?()+"/");
    else {
	string * tmp;
	tmp = explode(file_name(PO(0)),"/");
	if (sizeof(tmp) <= 1) basedir = "/";
	else basedir = "/"+implode(tmp[0..sizeof(tmp)-2],"/")+"/";
    }
    finalpath = real_path(str, basedir); 
    do_explore_path(explode(finalpath, "/"), 0, "/", ref paths, flag);
    return paths;
}

int f_dots(string file, string path, int flag, int final) { 
    if (file == "." || file == "..") return 0; 
    if (flag == ONLY_DIRS)  return find_dir(path+file); 
    if (flag == ONLY_FILES) return find_file(path+file);
    return 1;
}

private void do_explore_path(string * path, int index, string parsed, string ref * paths, int finalflag) {
    int i, size, flag;
    string * match, * files, err;
    
    if (index == sizeof(path)) {
	paths += ({ "/"+implode(explode(parsed,"/"),"/") });
	return;
    }
    flag = (index == sizeof(path)-1 ? finalflag : ONLY_DIRS);

    err = catch(files = get_dir(parsed+path[index]));
    if (err) {
	write(serror(err)+"\n");
	return;
    }    
    size = sizeof(match = filter_array(files, "f_dots", TO, parsed, flag));
    for (i=0; i<size; i++) {
    	do_explore_path(path, index+1, parsed+match[i]+"/", ref paths, finalflag);
    }
}

string real_path(string str, string basedir) {
    int i, size;
    string destino, dir;
    string * path, * aux;
    
    if (!str || str=="" || str==".") return basedir;
    if (str=="/") return str;
    if (str[0..0] == "/") destino = str;
    else destino = basedir+str;
    dir = "/";
    size = sizeof(path=explode(destino, "/"));
    for(i=0; i<size; i++) {
	if (path[i]=="")  continue;
	if (path[i]==".") continue;
	else if (path[i] == "..") {
	    aux = explode(dir, "/");
	    if ((sizeof(aux)-1) <= 0) dir = "/";
	    else dir = "/"+implode(aux[0..sizeof(aux)-2], "/") + "/";
	}	
	else {
	    dir += path[i];
	    if ((i<(size-1))&&(dir != "/")) dir += "/";
	    else if (str[strlen(str)-1..strlen(str)-1] == "/") dir += "/";
	}
    }
    return dir;
}
