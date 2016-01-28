
int mv(string origen, string destino) {
    int err, res;    

    if ((!find_file(origen)) || (find_dir(destino))) return 0;

    err = catch(res = efun::cp(origen, destino));
    if (err) write("mv: "+serror(err)+"\n");
    if ((err) || (!res)) return 0;

    err = catch(res = efun::rm(origen));
    if (err) write("mv: "+serror(err)+"\n");
    if ((err) || (!res)) return 0;

    return 1;
}
