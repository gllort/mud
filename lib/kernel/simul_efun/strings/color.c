#include <config.h>

string codigo_color(int, int);

string color(string texto) {
    int c1, c2, fin;
    string coloreado, prev, post, str, c3;

    if (!texto || texto=="") return "";    
    fin = 0;
    str = texto+COLOR_MARK+"R";
    coloreado = "";
    while (!fin) {
	if (sscanf(str, "%s"+COLOR_MARK+"%d,%d%s", prev, c1, c2, post) == 4) {
    	    coloreado += prev;
	    c1 = c1 % 8;	
	    c2 = c2 % 8;
	    coloreado += codigo_color(c1, c2);
	    str = post;
	    continue;
	}
	if (sscanf(str, "%s"+COLOR_MARK+"%s", prev, post) == 2) {
	    c3 = post[0..0];
	    post = post[1..strlen(post)-1];
	    coloreado += prev;
	    if ((to_int(c3) == 0) && (c3 != "0")) {
		switch(c3) {
        	    case "N": coloreado += "[1m"; break;
	    	    case "P": coloreado += "[5m"; break; 
		    case "R": coloreado += "[0;37;40m"; break;
		    case "S": coloreado += "[4m"; break; 
		    default : coloreado += COLOR_MARK;
			      post = c3 + post; 
			      break;
		}
	    }
	    else coloreado += codigo_color(to_int(c3), -1);
	    str = post;
	    continue;
	}
	fin = 1;
        coloreado += str;
    }
    return coloreado;
}

string codigo_color(int front, int back) {
    string color = "";
    switch(front) {
        case 0 : color += "[30m"; break;
        case 1 : color += "[34m"; break; 
        case 2 : color += "[32m"; break;
        case 3 : color += "[36m"; break;
        case 4 : color += "[31m"; break;
        case 5 : color += "[35m"; break;
        case 6 : color += "[33m"; break;
        case 7 : color += "[37m"; break;
	default: color += ""; break;
    }
    switch(back) {
	case 0 : color += "[40m"; break;
	case 1 : color += "[44m"; break; 
        case 2 : color += "[42m"; break;
        case 3 : color += "[46m"; break;
        case 4 : color += "[41m"; break;
        case 5 : color += "[45m"; break;
        case 6 : color += "[43m"; break;
        case 7 : color += "[47m"; break;
	default: color += ""; break;
    }
    return color;
}


