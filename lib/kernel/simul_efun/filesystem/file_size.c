
int file_size(string path) {
    int res;
    string err;

    err = catch(res = efun::file_size(path));
    if (err) write(serror(err)+"\n");  
    return res;
}
