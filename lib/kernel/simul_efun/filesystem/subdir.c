
int subdir(string p1, string p2) {
    int i, size;
    string * subdir, * global;
    
    subdir = explode(p1, "/");
    global = explode(p2, "/");
    
    size = sizeof(global);
    for (i=0; i<size; i++) {
	if (global[i] != subdir[i]) return 0;
    }
    return 1;    
}
