#include <comando.h>



int comando(int argc, string * argv) {
    int i, niv, max, pc, pintado;
    
    string * skills;

    skills = TP -> skills?();
    skills = sort_array(skills, 1);
    foreach(string skill in skills) {
	niv = TP->nivel_skill?(skill);
	max = TP->nivel_maximo_skill?(skill);
	
	write("$N"+capitalize(skill)+"$R Nv. "+niv+" (Max "+max+") ");

	pc = (niv*100 / max);

	pintado = (niv*30 / max);
	

	write("[$3,3");
	for(i=0;i<pintado;i++) {
	    write(".");	    
	}
	write("$R");
	for(i=pintado;i<30;i++) {
	    write(" ");
	}
	write("] ");
	write(pc+"%\n");
    }
    return 1;
}
