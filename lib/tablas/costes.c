
int coste?(int nivel) {
    int i, coste;

    if (nivel <= 1) return 0;
    for(i=2; i<=nivel; i++) {
	coste += (i-1) * 1000;    
    }
    return coste;
}
