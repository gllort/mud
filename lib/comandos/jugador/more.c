#include <comando.h>
#include <errno.h>

#define NUM_OF_LINES 20

void more(string, string, int);

int comando(int argc, string * argv) {
    string err, file, * files;

    if (!argc) {
	notify_fail("Sintaxis: more [archivo].\n");
	return 0;
    }    
    files = explore_path(argv[0], 2);
    if (!sizeof(files)) {
	notify_fail("more: "+argv[0]+": No existe el archivo.\n");
	return 0;
    }
    file = files[0];

    /* Intentamos leer del archivo para comprobar que tenemos acceso
     * de lectura. Si todo va bien, todas las lecturas posteriores 
     * seran correctas.
     */
    err = catch(read_file(file, 1, 1));
    if (err) {
	notify_fail("more: "+serror(err)+"\n");
	return 0;
    }
    
    if (sizeof(files) > 1) write("Archivo ambiguo: ");
    write("Mostrando "+file+".\n");

    more("", file, 1);
    return 1; 
}

void more(string opc, string file, int start_line) {
    string line;

    switch(lower_case(opc)) {
	case "q": return;
	case "b": start_line -= NUM_OF_LINES * 2;
		  if (start_line<1) start_line=1;
	          break;
	default : break;
    }	
    for (int i=start_line; i<start_line+NUM_OF_LINES; i++) {

	/* Antes hemos verificado que teniamos acceso de lectura, 
	 * de modo que podemos hacer las lecturas "a saco".
	 */
        line = read_file(file, i, 1);
        if (!line) { write("\n"); return; }	
        write(i+": "+line);	
    }
    write("$N$7,1[ Fichero $3"+file+"$R$N$7,1 - q o Q para salir ]$R ");
    input_to("more", 0, file, NUM_OF_LINES+start_line);  
}
