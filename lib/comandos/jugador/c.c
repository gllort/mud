#include <comando.h>

#define ENSURE(x)	(x ? 1 : 0)

int comando(string argc, string * argv) {
    function f;
    string str;
    
    if (evaluate( (: TP->nombre?() != "cain" ? 1 : 0  :) )) 
	write("ENSURADO!!!!!!!!!\n");
    else
	write("KAKITAS\n");
	
/*    
    f += ({ (: write("hola") :) }) + ({ (: write("feo") :) });

    foreach(function fun in f) 
	printf("eval: "+evaluate(fun)+"\n");
*/
    return 1;
}

