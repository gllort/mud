#include <comando.h>

void wc(string);

int comando(int argc, string * argv) {

    wc(argv[0]);
    return 1;
}

void wc(string fichero) {
    int lineas, palabras, caracteres;
    string l;

    lineas = 1;    
    while((l = read_file(fichero, lineas, 1))) {
	lineas++;
	palabras += sizeof(explode(l, " "));
	caracteres += strlen(l);
    }
    printf("%s: %d lineas\t%d palabras\t%d caracteres.\n", fichero, lineas-1, palabras, caracteres);
}