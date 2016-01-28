
/*
 * Extraido de las librerias Lima 1.0b4
 * Devuelve una version del array pasado por parametro sin
 * items duplicados.
 * Ej: clean_array(({1,2,2})) => ({1,2})
 */
 
mixed * clean_array(mixed * r) {
    int i, n;

    r = r & r; // sort.  sort_array() can't hack it.  And no, &= doesn't work.

    n = sizeof(r) - 1;
    while (i < n) {
	if (r[i] == r[i+1]) {
	    int j = i+1;
	    
	    while (j < n && r[i] == r[j + 1])
		j++;

	    r[i..j-1] = ({});
	    n -= j - i;
	}
	i++;
    }
    return r;
}
