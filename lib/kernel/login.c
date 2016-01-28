#include <config.h>
#include <mudlib.h>
#include <dirs.h>
#include <errno.h>
#include <sexo.h>

/* Variables globales */
private nosave object usuario;
private nosave string nick, dbpass, pass, email;
private nosave int inmortal, retry, ncall;

private nosave int sexo;
private nosave string raza, clase;

/* Prototipos */
void logon();
void intro_nick(string);
void confirm_nick(string);
void crear_password(string);
void check_password(string);
void confirm_password(string);
void intro_mail(string);
void crear_sexo(string);
void crear_raza();
void introducir_raza(string, string *);
void crear_clase();
void introducir_clase(string, string *);
void desconectar_copia(string);
void desconectar();
private nomask int find_admin?();
void load_user(int);

void create() { ; }

void logon() {
    cat(DIR_DOC+"/Bienvenida.txt");

    ncall = call_out("desconectar", LOGIN_TIME);

    write("Introduce tu apodo: ");
    input_to("intro_nick");
}

void intro_nick(string user) {
    string err, * info;
        
    if (strlen(user) < LOGIN_MIN_NAME) {
	write("La longitud de tu apodo debe ser superior a "+LOGIN_MIN_NAME+" caracteres.\n");
        write("Introduce tu apodo: ");
	input_to("intro_nick");
	return;
    }
    if (strlen(user) > LOGIN_MAX_NAME) {
	write("La longitud de tu apodo debe ser inferior a "+LOGIN_MAX_NAME+" caracteres.\n");
        write("Introduce tu apodo: ");
	input_to("intro_nick");
	return;
    }
    nick = lower_case(user);
    if (!find_file(DIR_FICHAS"/"+nick[0..0]+"/"+nick+".o")) {
	/* Por si hay impureza en la BD */
        err = catch(USERS -> deregistrar(nick));
	if ((err) && (errno(err) != ERR_NO_USER)) { throw(err); destruct(TO); }
	write("\nEres un nuevo jugador.\n");
#ifdef NO_NEW_PLAYERS
	write("Actualmente no se aceptan nuevos jugadores.\n");    	    	
	destruct(TO);
	return;
#else 
	write("Seguro que deseas ser conocido como "+capitalize(nick)+"? ");
	input_to("confirm_nick");
#endif
    }
    else {
	/* Existe ficha */
	err = catch(info = USERS -> info(nick));
	if ((err) && (errno(err) == ERR_NO_USER)) {
	    /* No hay entrada en la BD */
	    write("\nLost & found!\n");
	    destruct(TO);
	    return;
	}
	else if (err) throw(err);
	else {
	    /* Recuperamos los datos */
	    dbpass = info[1];
	    retry = 0;
	    inmortal = 0;
	    if (USERS -> inmortal?(nick)) {
		/* Patch secure login ***********
		write("Utiliza una conexion segura.\n");
		destruct(TO);
		return;
		*********************************/
		inmortal = 1;
	    }	
	    write("Password: ");
	    input_to("check_password");
	}
    }
}

void confirm_nick(string opcion) {
    opcion = lower_case(opcion);
    if ((opcion=="s") || (opcion=="y") || (opcion=="si") || (opcion=="yes")) {
	write("Elige tu password: ");
	input_to("crear_password");
    } 
    else {
        write("\nIntroduce tu apodo: ");
	input_to("intro_nick");
    }
}

void check_password(string passwd) {
    retry++;
    if (dbpass == passwd) {
	/* Existe copia previa del objeto jugador? */
	usuario = find_player(nick);
	if (usuario) {
	    write("El usuario '"+capitalize(nick)+"' ya esta en el juego. Desconectar la copia? ");
	    input_to("desconectar_copia");
	}
	else {
	    load_user(0);
	}
    }
    else {
	if (retry >= 3) {
	    write("\nDemasiados intentos incorrectos.\n");
	    destruct(TO);
	}
	write("\nPassword incorrecto. Intentalo de nuevo: ");
	input_to("check_password");
    }
}

void crear_password(string passwd) {
    if (strlen(passwd) < LOGIN_MIN_PASS) {
	write("\nLa longitud de tu password debe ser superior a "+LOGIN_MIN_PASS+" caracteres.\n");
	write("Elige tu password: ");
	input_to("crear_password");
	return;
    }
    if (strlen(passwd) > LOGIN_MAX_PASS) {
	write("\nLa longitud de tu password debe ser inferior a "+LOGIN_MAX_PASS+" caracteres.\n");
	write("Elige tu password: ");
	input_to("crear_password");
	return;
    }
    pass = passwd;
    write("Reescribe el password: ");
    input_to("confirm_password");    
}

void confirm_password(string passwd) { 
    if (pass != passwd) {
	write("\nLos passwords no coinciden.\n");
	write("Elige tu password: ");
	input_to("crear_password");
    }
    else {
	write("Introduce tu direccion de correo electronico: ");
	input_to("intro_mail");    
    }
}

void intro_mail(string mail) {
    string username, host, domain;
        
    if (sscanf(mail, "%s@%s.%s", username, host, domain) != 3) {
	write("Direccion de correo incorrecta.\n");
	write("Introduce tu direccion de correo electronico: ");
	input_to("intro_mail");    
	return;
    }
    else {
	email = mail;
        write("Vas a ser varon o hembra? (v/h) ");
	input_to("crear_sexo");
    }    
}

void crear_sexo(string opcion) {
    opcion = lower_case(opcion);
    
    if ((opcion == "v") || (opcion == "varon") || (opcion == "hombre")) sexo = VARON;
    else if ((opcion == "h") || (opcion == "hembra") || (opcion == "mujer")) sexo = HEMBRA;
    else {
	write("Varon o hembra? (v/h) ");
	input_to("crear_sexo");
	return;
    }
    crear_raza();
}

void crear_raza() {
    int i, size;
    string err, str, * razas_disponibles, * desc = ({ });

    write("Eleccion de raza.\n");
    
    err = catch(razas_disponibles = get_dir(DIR_RAZAS+"/*.c"));
    if(err) size = 0;
    else size = sizeof(razas_disponibles);

    for (i=0; i<size; i++) { 
    	razas_disponibles[i] = lower_case(explode(razas_disponibles[i], ".")[0]);
	desc += ({ capitalize(razas_disponibles[i]) });
    }
    size = sizeof(desc = sort_array(desc, 1));
    if (!size) {
	write("En este momento no hay razas disponibles.\n");
	destruct(TO);
    }
    else if (size == 1) str = "La unica raza disponible es: "+desc[0]+".";
    else {
	str = "Puedes escoger entre las siguientes razas: ";
	str += implode(desc[0..size-2], ", ") + " y "+desc[size-1]+".";
    }
    write(str+"\n"+"Tu opcion: ");

    input_to("introducir_raza", razas_disponibles);
}

void introducir_raza(string raza_seleccionada, string * razas_disponibles) {
    if ((!raza_seleccionada) || (raza_seleccionada=="") || (member_array(raza_seleccionada, razas_disponibles) == -1)) {
	write("Esa raza no existe.\n"+
	      "Tu opcion: ");
	input_to("introducir_raza", razas_disponibles);
	return;
    }
    else {
        raza = raza_seleccionada;

	crear_clase();
    }
}

void crear_clase() {
    int i, size;
    string err, * clases, * tmp;
    object obrace;
    
    err = catch(obrace = load_object(DIR_RAZAS+"/"+raza+".c"));
    if (err) {
	write(err);
	destruct(TO);
    }
    clases = obrace -> clases_permitidas?();

    printf("%O %O\n", obrace, clases);

    if (!sizeof(clases)) {
	err = catch(clases = get_dir(DIR_CLASES+"/*"));
	if (err) {
	    write(err);
	    destruct(TO);
	}
	size = sizeof(clases);
	for(i=0; i<size; i++) {
	    tmp = explode(clases[i], "/");	
	    clases[i] = explode(tmp[sizeof(tmp)-1], ".")[0];
	}
    }
    size = sizeof(clases);
    if (!size) {
	write("En este momento no hay clases disponibles.\n");
	destruct(TO);
    }
    else if (size == 1) {
	write("La unica clase permitida para un "+raza+" es: "+clases[0]+"\n");
    }
    else {
	write("Las clases disponibles para un "+raza+" son: "+implode(clases[0..size-2],", ")+" y "+clases[size-1]+"\n");
    }
    write("Tu opcion: ");
    input_to("introducir_clase", clases);
}

void introducir_clase(string opcion, string * clases_permitidas) {
    string err;

    if ((!opcion) || (opcion == "") || (member_array(opcion, clases_permitidas) == -1)) {
	write("Esa clase no existe.\n"+
	      "Tu opcion: ");
	input_to("introducir_clase", clases_permitidas);
	return;
    }
    else {
        clase = opcion;

	err = catch(USERS -> registrar(nick, pass, email));
	if (err) {
	    write(err);
	    destruct(TO);
	}
	load_user(1);
    }
}

void desconectar_copia(string opcion) {
    opcion = lower_case(opcion);
    if ((opcion=="s") || (opcion=="y") || (opcion=="si") || (opcion=="yes")) {
	destruct(usuario);
	load_user(0);
    } 
    else {
	destruct(TO);
    }
}

void desconectar() {
    write("\nHas excedido el tiempo de login.\n");
    destruct(TO);
}

private nomask int find_admin?() {
    if (sizeof(USERS -> inmortales?()) <= sizeof(SYSTEM_USERS)) return 0;
    else return 1;
}

void load_user(int nuevo_jugador) {
    string file, err;

    if (inmortal) file = INMORTAL;
    else file = JUGADOR;
    
    if (catch(usuario = clone_object(file)) || !usuario) {
	write("login: Error cargando el objeto jugador!\n");
    }
    else {
        usuario -> nombre(nick);
	usuario -> inicializar();
	exec(usuario, TO);
	remove_call_out(ncall);
	if (nuevo_jugador) {
	    if (!usuario -> crear_ficha(sexo, raza, clase)) destruct(usuario);
        }
	if ((!nuevo_jugador) && (!usuario -> cargar_ficha())) destruct(usuario);

        /* Si no existe administrador humano del mud significa que el juego 
	 * esta siendo inicializado ahora por PRIMERA VEZ, y esta conexion, 
         * es la del admin arranca el mud. Promoteamos automaticamente.
         */
	if (!find_admin?()) {
	    err = catch(USERS -> promote(nick, RANGOS->rango_maximo?()));
	    if (err) error(err);
	}
    }
    destruct(TO);
}
