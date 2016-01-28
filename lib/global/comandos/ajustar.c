int totales, restantes, maximo, minimo, maxatribs, maxatribsact, maxact, pciento;
int fue, des, con, tel, sab, car;

int maximo();
int confirmar(string);
int escoger_opcion(string);

int comando(int argc, string * argv) {
    
    totales = restantes = 83;
    maximo = 18;
    maxact = fue = des = con = tel = sab = car = 5;
    maxatribs = 2;
    maxatribsact = 0;
    pciento = 30;
    minimo = ceil(maxact * pciento / 100);
    restantes = totales - (fue+des+con+tel+sab+car);
    
    write("Tienes un total de "+totales+" puntos a repartir entre tus 6 atributos\n"
          "principales: fuerza, destreza, constitucion, inteligencia,\n"
	  "sabiduria y carisma.\n"
	  "Puedes asignar un maximo de "+maximo+" puntos para "+maxatribs+" de estos atributos,\n"
	  "y como minimo el "+pciento+"% (redondeado) del valor de tu atributo maximo.\n"
	  "Los comandos que puedes utilizar son:\n"
	  "suma  <atributo> <puntos>	    Asigna puntos a los atributos.\n"
	  "resta <atributo> <puntos>	    Asigna puntos a los atributos.\n"
	  "puntos           Muestra los puntos restantes a repartir.\n"
	  "atributos        Muestra los puntos asignados a cada atributo actualmente.\n"
	  "ayuda            Muestra este mensaje de ayuda.\n"
	  "guardar          Guardar y salir.\n"
	  "salir            Salir sin guardar.\n");

    write("Opcion: ");
    input_to("escoger_opcion");
    
    return 1;
}

int escoger_opcion(string opc) {
    int valor, vatrib, match;
    string opcion, atrib;
    
    if (!(match = sscanf(opc, "%s %s %d", opcion, atrib, valor))) {
	match = 1;
	opcion = opc;
    }
    if (match == 1) {
	switch(opcion) {
	    case "puntos":
		write("Te quedan "+restantes+" puntos para repartir.\n");		
		break;
	    case "atributos":
		write("Fue: "+fue+"\tDes: "+des+"\tCon: "+con+"\n"
		      "Int: "+tel+"\tSab: "+sab+"\tCar: "+car+"\n");
		break;
	    case "salir":
		write("No olvides ajustar tu ficha mas tarde!\n");
		return 1;
	    case "guardar":
		if (!restantes) {
		    write("No podras volver a modificar estos valores. Estas seguro? [s/n]: ");
		    input_to("confirmar");
		    return 1;
		}
		else {
		    write("Aun te quedan puntos por repartir!\n");	
		}
		break;
	    default:
    		write("Opcion incorrecta.\n");
		break;
	}
    }
    else if (match == 3) {
	switch(atrib) {
	    case "fue": vatrib = fue; break;
	    case "des": vatrib = des; break;
	    case "con": vatrib = con; break;
	    case "car": vatrib = car; break;
	    case "sab": vatrib = sab; break;
	    case "int": vatrib = tel; break;
	    default: write("Opcion incorrecta.\n"); 
		     write("Opcion: ");
		     input_to("escoger_opcion");
		     return 1;
    	}
	switch(opcion) {
	    case "suma":
		if (valor > restantes) write("No tienes suficientes puntos.\n");
		else if (vatrib + valor > maximo) write("No puedes aumentar un atributo por encima de "+maximo+" puntos.\n");
		else if ((vatrib + valor == maximo) && (maxatribsact == maxatribs)) write("Ya tienes "+maxatribs+" atributos con "+maximo+" puntos.\n");
		else {
		    restantes -= valor;
		    vatrib += valor;
		    if (vatrib > maxact) { 
			maxact = vatrib; 
		        minimo = ceil(maxact * pciento / 100);
		    }
		    if (vatrib == maximo) maxatribsact++;
		}
		break;

	    case "resta":
		if (vatrib < valor) valor = vatrib;
		else if (vatrib - valor < minimo) write("El "+pciento+"% de tu atributo maximo es de "+minimo+" puntos.\n");
		else {
		    restantes += valor;
		    if (vatrib == maxact) maxact = maximo();
		    if (vatrib == maximo) maxatribsact--;
		    vatrib -= valor;		
		}
		break;
	    default:
    		write("Opcion incorrecta.");
		break;
	}
	switch(atrib) {
	    case "fue": fue = vatrib; break;
	    case "des": des = vatrib; break;
	    case "con": con = vatrib; break;
	    case "car": car = vatrib; break;
	    case "sab": sab = vatrib; break;
	    case "int": tel = vatrib; break;
	}
    }
    else {        
	write("Opcion incorrecta.\n");
    }
    write("Opcion: ");
    input_to("escoger_opcion");
    return 1;
}

int confirmar(string opc) {
    opc = lower_case(opc);
    if ((opc == "s") || (opc == "si") || (opc == "y") || (opc == "yes")) {
	TP -> fue(fue);
	TP -> des(des);
	TP -> con(con);
	TP -> car(car);
	TP -> sab(sab);
	TP -> tel(tel);
	write("Atributos ajustados.\n");
	TP -> rem_command(relative_name(TO));
	TP -> guardar();
	destruct(TO);
    }
    else {
	write("Opcion: ");
        input_to("escoger_opcion");
    }
    return 1;
}

int maximo() {
    int max = 0;
    if (fue > max) max = fue;
    if (des > max) max = des;
    if (con > max) max = con;
    if (sab > max) max = sab;
    if (tel > max) max = tel;
    if (car > max) max = car;
    return max;    
}
