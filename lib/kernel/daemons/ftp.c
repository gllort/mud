/*
 * o Do_list cutre
 * o Fichero destino no se tokeniza localmente en los GET (Bien*).
 */
 
#include <mudlib.h>
#include <ports.h>
#include <log.h>
#include <daemons.h>
#include <sockets.h>
#include <ftpd.h>
#include <dirs.h>

inherit DAEMON;

#define RESTRICT_PORTS
#define MIN_PORT 6000
#define MAX_PORT 6100
#define MAX_TRIES 10

#define LTYPE_LIST 0
#define LTYPE_NLST 1

/* Variables globales */
private nosave int lastport = MIN_PORT;
private nosave int idles;
private nosave string iphost;
private nosave object FTPserver;
private nosave mixed outfile = ([ ]);
private nosave mapping pasivos   = ([ ]);
private nosave mapping sesiones  = ([ ]);
private nosave mapping dataports = ([ ]);
private nosave mapping command_handlers = ([
    "user" : (: FTP_CMD_user :),
    "pass" : (: FTP_CMD_pass :),
    "retr" : (: FTP_CMD_retr :),
    "stor" : (: FTP_CMD_stor :),
    "nlst" : (: FTP_CMD_nlst :),
    "list" : (: FTP_CMD_list :),
    "pwd"  : (: FTP_CMD_pwd  :),
    "cdup" : (: FTP_CMD_cdup :),
    "cwd"  : (: FTP_CMD_cwd  :),
    "quit" : (: FTP_CMD_quit :),
    "type" : (: FTP_CMD_type :),
    "mkd"  : (: FTP_CMD_mkd  :),
    "port" : (: FTP_CMD_port :),
    "noop" : (: FTP_CMD_noop :),
    "dele" : (: FTP_CMD_dele :),
    "syst" : (: FTP_CMD_syst :),
    "pasv" : (: FTP_CMD_pasv :),
]);

/* Prototipos */
private void FTP_Read(object, string);
private void FTP_Write(object);
private void FTP_Close(object);
private void FTP_Crear_Sesion(object);
private string FTP_Welcome_Message();
private void FTP_Timeout_Sesiones();
int FTP_Escoger_Puerto();
private void do_list(class sesion_FTP, string, int);

void start() {
    FTPLOG("Demonio de FTP iniciado a las "+ctime(time())+".\n");
}

void stop() {
    FTPLOG("Demonio de FTP desactivado a las "+ctime(time())+".\n");
}

private void create() {
//    FTPserver = new(SOCKET, SOCKET_LISTEN, PORT_FTP, (: FTP_Read :), (: FTP_Close :));
    iphost = "172.26.0.8";
//    resolve("212.170.18.18",(: iphost=$2 :) );
}

/*************************************
 *     Callbacks de los Sockets      *
 *************************************/

private void FTP_Read(object socket, string data) {
    int i;
    string cmd, arg, err;
    function cmd_handler;
    class sesion_FTP ultimaSesion;

    /* Si no recibimos datos significa que es una conexion nueva */
    if (!data) {
	FTP_Crear_Sesion(socket);
        return;
    }
    ultimaSesion = sesiones[socket];
    ultimaSesion->idleTime = 0;
    if (!ultimaSesion->command) ultimaSesion->command = "";  
    data = replace_string(data, "\r", "");
    ultimaSesion->command += data;
    if ((i=strsrch(ultimaSesion->command,"\n")) == -1) return;
    data = ultimaSesion->command[0..i-1];
    ultimaSesion->command = ultimaSesion->command[i+1..];
    ultimaSesion->command = trim(ultimaSesion->command);

    /* Parseamos el comando y los argumentos */
    if (!sscanf(data, "%s %s", cmd, arg)) {
        cmd = data;
    }
    cmd = lower_case(cmd);

    /* Si no estamos conectados, los comandos validos son... */
    if (!ultimaSesion->connected) {
	switch(cmd) {
	    case "user":
		FTP_CMD_user(ultimaSesion, arg);
		return;
	    case "pass":
		FTP_CMD_pass(ultimaSesion, arg);
		return;
	    case "quit":
		FTP_CMD_quit(ultimaSesion, arg);
		return;
	    case "noop":
		FTP_CMD_noop(ultimaSesion, arg);
		return;
	    default:
		socket->send("503 Identificate con USER antes.\n");
		return;
	}
    }
    /* Estamos conectados */    
    cmd_handler = command_handlers[cmd];
    if (!cmd_handler) {
        /* Comando incorrecto */
	FTPLOG(capitalize(ultimaSesion->user)+" ejecuta un comando inexiste: "+cmd+".\n"); 
        socket->send(sprintf("502 Comando incorrecto %s.\n", cmd));
        return;
    }
    if (err = catch(evaluate(cmd_handler, ultimaSesion, arg))) {
	FTPLOG(sprintf("%s provoca un ERROR con el comando '%s': %s",
		capitalize(ultimaSesion->user), 
		cmd,
		err));
        socket->send("550 Error desconocido. Por favor, informa a un administrador de que estabas haciendo.\n");
    }
    return;
}

private void FTP_Write(object socket) {
  class sesion_FTP sesion;

  sesion = sesiones[dataports[socket]];
  sesion->cmdPipe->send("226 Transferencia completada.\n");
  sesion->dataPipe->remove();
}

private void FTP_Close(object socket) {
    map_delete(sesiones, socket);
}

private void FTP_PASV_read(class sesion_FTP sesion, object socket, string text) { 

    sesion->dataPipe = socket;

    /* Reseteamos el write callback del socket. Ni idea de porque se hace,
     * pero no deberia hacer ningun mal a nadie. 
     */
    sesion->dataPipe->set_write_callback((:FTP_Write:));

    /* La conexion en modo pasivo ha funcionado, por tanto, podemos destruir
     * el socket que esta escuchando.
     */
    if (pasivos[sesion->cmdPipe]) {
	destruct(pasivos[sesion->cmdPipe]);
	map_delete(pasivos, sesion->cmdPipe); /* Para que no quede apuntando a 0 */
    }
    
    dataports[sesion->dataPipe] = sesion->cmdPipe;
    if (!text) return;

    switch(sesion->binary) {
	case 0:
    	    text = replace_string(text,"\r","");
	    /* ### SEGURIDAD AQUI ### */
    	    write_file(sesion->targetFile, text);
    	    return;
	case 1:
	    /* ### SEGURIDAD AQUI ### */
    	    write_buffer(sesion->targetFile, sesion->filepos, text);
    	    sesion->filepos += sizeof(text);
    	    return;
	default:
    	    return;
    }
}

private void FTP_DATA_read(object socket, mixed text) {
    class sesion_FTP sesion;

    sesion = sesiones[dataports[socket]];
    switch(sesion->binary) {
	case 0:
    	    text = replace_string(text,"\r","");
	    /* ### SEGURIDAD ? ### */
    	    write_file(sesion->targetFile, text);
    	    return;
	case 1:
	    /* ### SEGURIDAD ? ### */
    	    write_buffer(sesion->targetFile, sesion->filepos, text);
    	    sesion->filepos+=sizeof(text);
    	    return;
	default:
    	    return;
    }
}

private void FTP_DATA_close(object socket) {
    class sesion_FTP sesion;

    sesion = sesiones[dataports[socket]];
    if(sesion) {
        sesion->cmdPipe->send("226 Transferencia completada.\n");
    }
}

string FTP_CMD_retr_callback(object ob) {
    int start,length;
    mixed ret;

    if (!ob || undefinedp(outfile[ob])) return 0;
    start = outfile[ob][2];
    length = FTP_BLOCK_SIZE;
    outfile[ob][2] += length;
    if (start+length>file_size(outfile[ob][0])) length=file_size(outfile[ob][0])-start;
    ret=read_buffer(outfile[ob][0],start,length);
    if (start+length>=file_size(outfile[ob][0])) {
	map_delete(outfile,ob);
	ob->set_write_callback((: FTP_Write :));
    }
    return ret;
}


/*************************************
 *       Funciones auxiliares        *
 *************************************/

private void FTP_Crear_Sesion(object socket) {
    class sesion_FTP nueva_sesion;

    nueva_sesion = new(class sesion_FTP);
    nueva_sesion -> cmdPipe = socket;

    socket -> send(FTP_Welcome_Message());

    map_delete(sesiones, 0); /* Por si hay basura */
    if(!sizeof(sesiones)) {
	idles = call_out((: FTP_Timeout_Sesiones :), 60);
    }
    sesiones[socket] = nueva_sesion;
}

private void FTP_Timeout_Sesiones() {
    map_delete(sesiones, 0); /* Por si hay basura */
    if (!sizeof(sesiones)) return;
  
    /* Buscamos sesiones que esten inactivas para cerrarlas */
    foreach(object socket, class sesion_FTP sesion in sesiones) {
	if(sesion->dataPipe) /* Conexion de datos activa */ {
    	    sesion->idleTime = 0;
    	    continue;
	}
	sesion->idleTime += 60;
	if (sesion->idleTime >= MAX_IDLE_TIME+60) {
    	    FTPLOG(capitalize(sesion->user)+" desconectado por inactividad a las "+ctime(time())+".\n");
    	    socket->send(sprintf("421 Timeout (%i segundos inactivo). Cerrando conexion...\n", MAX_IDLE_TIME));
    	    map_delete(sesiones, socket);
    	    destruct(socket);
	}
    }
    if (sizeof(sesiones)) idles = call_out((: FTP_Timeout_Sesiones :), 60);
}

private string FTP_Welcome_Message() {
    string msg;
    msg = "220- "+MUD_NAME+": Servidor FTP activo.\n"+
	  "220 Identificate con el nombre de tu inmortal.\n"; 
    return msg;
}

int FTP_Escoger_Puerto() {
#ifdef RESTRICT_PORTS
    lastport ++;
    if (lastport>MAX_PORT+1)
	lastport = MIN_PORT+1;
    return lastport - 1;
#endif
    return 0;
}

void remove() {
    class sesion_FTP sesion;

    foreach(sesion in values(sesiones)) {
        if(objectp(sesion->cmdPipe)) {
	    destruct(sesion->cmdPipe);
	}
        if(objectp(sesion->dataPipe)) {
	    destruct(sesion->dataPipe);
	}
    }
    destruct(FTPserver);
    remove_call_out(idles);
}


/*************************************
 *         Comandos del FTP          *
 *************************************/

private void FTP_CMD_user(class sesion_FTP sesion, string arg) {
    NEEDS_ARG();

    arg = lower_case(arg);
    if(sesion->connected) {
	/* Comando USER cuando YA estamos conectados */
	sesion->cmdPipe->send("530 Prohibido cambiar de usuario.\n");
        return;
    }
    sesion->user = arg;
    sesion->cmdPipe->send(sprintf("331 Se requiere password para conectar como %s.\n", arg));
    return;
}

private void FTP_CMD_pass(class sesion_FTP sesion, string arg) {
    string err, password;
    string * userinfo;
    
    NEEDS_ARG();
    
    if (sesion->connected || !sesion->user) {
	/* Comando PASS cuando YA estamos conectados */
	sesion->cmdPipe->send("503 Identificate con USER antes.\n");
        return;
    }

    /* ### SEGURIDAD AQUI ### */
    err = catch(userinfo = USERS -> info(sesion->user));
    if (err) {
        FTPLOG(err+"\n");
        sesion->cmdPipe->send("530 Login incorrecto.\n");
        return;
    }
    password = userinfo[1];
  
    if(arg != password || !USERS->inmortal?(sesion->user)) {
        sesion->cmdPipe->send("530 Login incorrecto.\n");
        return;
    }
    sesion->cmdPipe->send(sprintf("230 Usuario %s conectado.\n", sesion->user));
    sesion->connected = 1;
    FTPLOG(sprintf("%s conecta desde %s a las %s.\n", 
		    capitalize(sesion->user), 
		    sesion->cmdPipe->address()[0],
		    ctime(time())));

    sesion->pwd = DIR_HOME + "/" + sesion->user;

    if (!find_dir(sesion->pwd)) {
	sesion->pwd = "/"; 
    }

    return;
}

private void FTP_CMD_pwd(class sesion_FTP sesion, string arg) {
    sesion->cmdPipe->send(sprintf("257 Directorio actual: \"%s\".\n", sesion->pwd));
}

private void FTP_CMD_syst(class sesion_FTP sesion, string arg) {
    sesion->cmdPipe->send("215 UNIX Nombre del Mud: "+MUD_NAME+"\n");
}

private void FTP_CMD_pasv(class sesion_FTP sesion, string arg) {
    int port_dec, tries;
    int * port;
    mixed listen_socket;

    if (arg) {
	sesion->cmdPipe->send("500 Sintaxis incorrecta.\n");
	return;
    }	
    if(sesion->dataPipe) {
	destruct(sesion->dataPipe);
    }

    tries = 0;
    switch(sesion->binary) {
	case 0:
    	    while(!listen_socket && tries < MAX_TRIES) {
    		listen_socket = evaluate((: new(SOCKET, SOCKET_LISTEN, 
	        	FTP_Escoger_Puerto(),
            	     (: FTP_PASV_read, $(sesion) :),
            	     (: FTP_DATA_close :)) :));
		tries++;
	    }
    	    break;
	case 1:
    	    while(!listen_socket && tries < MAX_TRIES) {
    		listen_socket = evaluate((: new(SOCKET, SOCKET_LISTEN_B, 
	        	FTP_Escoger_Puerto(),
            	     (: FTP_PASV_read, $(sesion) :),
            	     (: FTP_DATA_close :)) :));
		tries++;
	    }
    	    break;
	default:
    	    return;
    }

    if(member_array(sesion->cmdPipe, keys(pasivos)) > -1) {
	if (pasivos[sesion->cmdPipe])
    	    destruct(pasivos[sesion->cmdPipe]);
	else 
	    map_delete(pasivos, sesion->cmdPipe); /* Por si hay basura */
    }
    pasivos[sesion->cmdPipe] = listen_socket;
    port_dec = listen_socket->local_port();
    port = ({ port_dec>>8, port_dec%256 });
    sesion->cmdPipe->send(sprintf("227 Entrando en modo Pasivo. (%s,%i,%i)\n",
			      replace_string(iphost,".",","),
			      port[0],
			      port[1]) );
}


void FTP_CMD_noop(class sesion_FTP sesion, string arg) {
    sesion->cmdPipe->send("221 Comando NOOP ejecutado correctamente.\n"); 
}

private void FTP_CMD_list(class sesion_FTP sesion, string arg) {

    /* Verificamos que el Puerto de Datos esta conectado */
    if(!sesion->dataPipe) {
	call_out( (: FTP_CMD_list :), 2, sesion, arg);
        return;
    }
    do_list(sesion, arg, LTYPE_LIST);
}

private void FTP_CMD_nlst(class sesion_FTP sesion,string arg) {

    /* Verificamos que el Puerto de Datos esta conectado */
    if(!sesion->dataPipe) {
      call_out( (: FTP_CMD_nlst :), 2, sesion, arg);
      return;
    }
    do_list(sesion, arg, LTYPE_NLST);
}

private void do_list(class sesion_FTP sesion, string arg, int ltype) {
    string * files, path, err;

    path = (sesion->pwd == "/" ? sesion->pwd + "*" : sesion->pwd + "/*");
    err = catch(files = get_dir(path));
    if (err) {
	sesion->cmdPipe->send("550 Acceso denegado.\n");
    }
    else {
        sesion->cmdPipe->send("150 Abriendo conexion ASCII de datos para listar ficheros.\n");
	sesion->dataPipe->send(implode(files, "\r\n")+"\r\n");
    }
}

/* funcion original 

private void do_list(class sesion_FTP info, string arg, int ltype) {
  string array 	files;
  string flags;
  string output;
  int isfile;

  flags=find_flags(arg);
  arg=strip_flags(arg);
  if(!arg || arg == "")
      arg = ".";
  
  // Check to make sure we aren't really lookin gat a file.  
  // canonical_file() mucks this up a bit later on so the check
  // is necessary 
  if(arg[<2..]=="/.") 
    if(is_file(arg[0..<3])) 
      isfile=1;
  
  arg = evaluate_path(arg, info->pwd);
  ANON_CHECK(arg);
  if(unguarded(1, (:is_directory($(arg)):)))
      arg = join_path(arg, "*");
  if(unguarded(info->priv, (:file_size(base_path($(arg))):)) == -1)
    {
      info->cmdPipe->send(sprintf("550 %s: No such file OR directory.\n", arg));
      destruct(info->dataPipe);
      return;
    }
  if(isfile)
    files=({});
  else
    files = unguarded(info->priv, (:get_dir($(arg),-1):));
  if(!files&&!isfile)
    {
	  info->cmdPipe->send(sprintf("550 %s: Permission denied.\n",arg));
	  destruct(info->dataPipe);
	  return;
    }
  if(flags)
    if(strsrch(flags,'a')==-1)
      files = filter(files, (: member_array($1[0], ({".",".."})) == -1 :));
  if(!sizeof(files)&&!isfile)
    {
      info->cmdPipe->send("550 No files found.\n");
      destruct(info->dataPipe);
      return;
    }

  // in case of LIST imply -l 
  // in case of NLST imply -1 
  if (ltype == LTYPE_LIST)
    {
      if (flags)
	{
	  if ( (strsrch(flags, 'l') == -1) &&
	       (strsrch(flags, 'C') == -1) &&
	       (strsrch(flags, '1') == -1) )
	    flags += "l";
	}
      else
	flags = "l";
    }
  else
    {
      if (flags)
	{
	  if ( (strsrch(flags, 'l') == -1) &&
	       (strsrch(flags, 'C') == -1) &&
	       (strsrch(flags, '1') == -1) )
	    flags += "1";
	}
      else
	flags = "1";
    }

  // Check the flags for output now

  // Check the F flag 
  if(strsrch(flags,'F')>-1)
    {
      foreach(mixed array file in files)
	if(file[1]==-2)
	  file[0]=sprintf("%s/",file[0]);
    }
  // The C flag
  if(strsrch(flags,'C')>-1)
    {
      int lines;
      int size;
      int i;
      if((strsrch(flags,'l')>-1) || (strsrch(flags,'1')>-1))
	{
	  info->cmdPipe->send("550: LIST -C flag is incompatible with -1 or -l.\n");
	  destruct(info->dataPipe);
	  return;
	}
      lines=((size=sizeof(files))/3)+1;
      output="";
      for(i=0;i<lines;i++)
	{
	  mixed array these_files;
	  if((i*3+2)<size)
	      these_files=files[(i*3)..(i*3+2)];
	  else if(i*3<size)
	    {
	      these_files=files[(i*3)..];
	      while(sizeof(these_files)<3)
		these_files+=({ ({"",0,0}) });
	    }
	  else 
	    break;
	  output=sprintf("%s%-=25s %-=25s %-=25s\n",
			 output,
			 these_files[0][0],
			 these_files[1][0],
			 these_files[2][0]
			 );
	}
    }
  if(strsrch(flags,'l')>-1)
    {
      if(strsrch(flags,'1')>-1)
	{
	  info->cmdPipe->send("550: LIST -l and -1 flags incompatible.\n");
	  destruct(info->dataPipe);
	  return;
	}
      
      output = implode(map(files, 
			   (:sprintf("%s %3i %=9s %=8s %=7s %s%5s %s",
				     $1[1]==-2?"drwxrwxr-x":"-rw-rw-r--",
				     1,
				     lower_case(replace_string(mud_name(), " ", "_"))[0..7],
				     lower_case(replace_string(mud_name(), " ", "_"))[0..7],
				     $1[1]==-2?"0":sprintf("%d",$1[1]),
				     ctime($1[2])[4..10],
				     (time()-$1[2])>31536000?ctime($1[2])[20..]:ctime($1[2])[11..15],
				     $1[0])
			    :)),"\n");
    }


  if(strsrch(flags,'1')>-1)
    output=implode(map(files,(:sprintf("%s",$1[0]) :)),"\n");

  info->cmdPipe->send("150 Opening ascii mode data connection for file list\n");
  info->dataPipe->send(implode(explode(output,"\n"), "\r\n")+"\r\n");
  return;
}
*/

private void FTP_CMD_cdup(class sesion_FTP sesion, string arg) {
  FTP_CMD_cwd(sesion, "..");
}

private void FTP_CMD_cwd(class sesion_FTP sesion, string arg) {
    string * paths;

    NEEDS_ARG();

    if (arg[0] != '/') arg = (sesion->pwd=="/" ? "/"+arg : sesion->pwd+"/"+arg);
    paths = explore_path(arg, 1);
    
    /* ### SEGURIDAD AQUI ### */
    if(!sizeof(paths)) {
	sesion->cmdPipe->send(sprintf("550 %s: No existe el fichero o directorio.\n", arg));
        return;
    }
    sesion->pwd = paths[0];
    sesion->cmdPipe->send("250 Comando CWD ejecutado correctamente.\n");
}

private void FTP_CMD_quit(class sesion_FTP sesion, string arg) {
    sesion->cmdPipe->send("221 Adios!\n");
    if(sesion->dataPipe)
	catch(destruct(sesion->dataPipe));
    if(member_array(sesion->cmdPipe, keys(pasivos))>-1) {
	catch(destruct(pasivos[sesion->cmdPipe]));
	map_delete(pasivos, sesion->cmdPipe);
    }
    FTPLOG(sprintf("%s desconecta a las %s.\n", capitalize(sesion->user), ctime(time())));
    map_delete(sesiones, sesion->cmdPipe);
    catch(destruct(sesion->cmdPipe));
}


private void FTP_CMD_retr(class sesion_FTP sesion, string arg) {
    string target_file, * paths;
    int i;

    /* Verificamos que el Puerto de Datos esta conectado */
    if(!sesion->dataPipe) {
        call_out( (: FTP_CMD_retr :), 2, sesion, arg);
        return;
    }
    NEEDS_ARG();
  
    if (arg[0] != '/') arg = (sesion->pwd == "/" ? "/"+arg : sesion->pwd+"/"+arg);
    paths = explore_path(arg, 2);
    
    if (!sizeof(paths)) {
	sesion->cmdPipe->send(sprintf("550 %s: No existe el fichero.\n", arg));
        destruct(sesion->dataPipe);
	return;
    }
    target_file = arg = paths[0];
    /* ### SEGURIDAD POR AQUI ### */       
    if(!file_size(target_file)) {
	sesion->cmdPipe->send(sprintf("550 %s: El fichero esta vacio.\n", target_file));
        destruct(sesion->dataPipe);
        return;
    }
    switch(sesion->binary) {
	case 0:	
    	    i = file_size(target_file);
    	    FTPLOG(sprintf("%s RECIBE %s (ascii).\n", capitalize(sesion->user), target_file));
    	    outfile[sesion->dataPipe]=({target_file,0,0,sesion->cmdPipe});
    	    sesion->dataPipe->set_write_callback((: FTP_CMD_retr_callback :));
	    sesion->cmdPipe->send(sprintf("150 Abriendo conexion de datos ASCII para recibir %s (%d bytes).\n", target_file, i));
    	    sesion->dataPipe->send(FTP_CMD_retr_callback(sesion->dataPipe));
    	    break;
	case 1:
    	    i = file_size(target_file);
    	    FTPLOG(sprintf("%s RECIBE %s (binario).\n", capitalize(sesion->user), target_file));
    	    outfile[sesion->dataPipe]=({target_file,1,0,sesion->cmdPipe});
    	    sesion->dataPipe->set_write_callback((: FTP_CMD_retr_callback :));
    	    sesion->cmdPipe->send(sprintf("150 Abriendo conexion de datos BINARIA para recibir %s (%d bytes).\n", target_file, i));
    	    sesion->dataPipe->send(FTP_CMD_retr_callback(sesion->dataPipe));
    	    break;
	default:
    	    return;
    }
}

private void FTP_CMD_stor(class sesion_FTP sesion, string arg) {
    string base, * path;

    /* Verificamos que el Puerto de Datos esta conectado */
    if(!sesion->dataPipe) {
        call_out( (: FTP_CMD_stor :), 2, sesion, arg);
        return;
    }
    NEEDS_ARG();

    if (arg[0] != '/') arg = (sesion->pwd=="/" ? "/"+arg : sesion->pwd+"/"+arg);
    path = explode(arg,"/");
    if (sizeof(path)==1) base = "/";
    else base = "/"+implode(path[0..sizeof(path)-2],"/")+"/";

    if (!find_dir(base)) {
	sesion->cmdPipe->send(sprintf("553 %s: No existe el directorio.\n",base));
        destruct(sesion->dataPipe);
        return;
    }

    if ((strsrch(path[sizeof(path)-1],"*") != -1) || (strsrch(path[sizeof(path)-1],"?") != -1)) {
	sesion->cmdPipe->send(sprintf("553 %s: Uno o mas caracteres invalidos.\n",path[sizeof(path)-1]));
        destruct(sesion->dataPipe);
        return;
    }

    sesion->targetFile = base+path[sizeof(path)-1];

    if (find_file(sesion->targetFile)) {
	if (!rm(sesion->targetFile)) {
	    sesion->cmdPipe->send(sprintf("550 %s: Permiso denegado.\n", sesion->targetFile));
	    destruct(sesion->dataPipe);
	    return;
	}
    }
    else if (!write_file(sesion->targetFile, "")) {
        sesion->cmdPipe->send(sprintf("550 %s: Permiso denegado.\n", sesion->targetFile));
        destruct(sesion->dataPipe);
        return;
    }
    FTPLOG(sprintf("%s ENVIA %s (%s).\n", capitalize(sesion->user), sesion->targetFile, sesion->binary ? "binario" : "ascii"));

    // Reset the file position flag. 
    sesion->filepos=0;
    sesion->cmdPipe->send(sprintf("150 Abriendo conexion %s de datos para enviar %s.\n",
			      sesion->binary ? "BINARIA" : "ASCII",
			      path[sizeof(path)-1]));
}

private void FTP_CMD_type(class sesion_FTP sesion, string arg) {
    NEEDS_ARG();
    switch(arg) {
	case "a":
	case "A":
    	    sesion->binary = 0;
    	    sesion->cmdPipe->send("200 Tipo de fichero: A.\n");
    	    return;
	case "i":
	case "I":
    	    sesion->binary = 1;
    	    sesion->cmdPipe->send("200 Tipo de fichero: I.\n");
    	    return;
	default:
    	    sesion->cmdPipe->send("550 Tipo de fichero desconocido.\n");
    	    return;
    }
}

private void FTP_CMD_dele(class sesion_FTP sesion, string arg) {
    string * paths;
    
    NEEDS_ARG();
    
    if (arg[0] != '/') arg = (sesion->pwd=="/" ? "/"+arg : sesion->pwd+"/"+arg); 
    paths = explore_path(arg);

    if (!sizeof(paths)) {
        sesion->cmdPipe->send(sprintf("550 %s: No existe el fichero o directorio.\n", arg));
        return;
    }
    if (!rm(paths[0])) {
        sesion->cmdPipe->send(sprintf("550 %s: Permiso denegado.\n",paths[0]));
        return;
    }
    FTPLOG(sprintf("%s BORRA %s.\n", capitalize(sesion->user), paths[0]));
    sesion->cmdPipe->send("250 Comando DELE ejecutado correctamente.\n");
}


private void FTP_CMD_mkd(class sesion_FTP sesion, string arg) {
    NEEDS_ARG();

    if (arg[0] != '/') arg = (sesion->pwd == "/" ? "/"+arg : sesion->pwd+"/"+arg);
    
    if (find_dir(arg)) {
	sesion->cmdPipe->send(sprintf("550 %s: El directorio ya existe.\n", arg));
        return;
    }
    if (find_file(arg)) {
	sesion->cmdPipe->send(sprintf("550 %s: Ya existe un fichero con ese nombre.\n", arg));
        return;
    }
    /* ### SEGURIDAD ### */
    if (!mkdir(arg)) {
        sesion->cmdPipe->send(sprintf("550 %s: Permiso denegado.\n", arg));
        return;
    }
    sesion->cmdPipe->send("257 Comando MKD ejecutado correctamente.\n");
    return;
}


private void FTP_CMD_port(class sesion_FTP sesion, string arg) {
    string ip, * parts;
    int	port;
    
    NEEDS_ARG();

    parts = explode(arg, ",");
    if(sizeof(parts)!=6) {
	sesion->cmdPipe->send("550 Comando PORT fallido.\n");
	destruct(sesion->dataPipe);
	return;
    }
    ip = implode(parts[0..3],".");
    /* Make a 16 bit port # out of 2 8 bit values.  */
    port = (to_int(parts[4]) << 8) + to_int(parts[5]);
  
    if(sesion->dataPipe) destruct(sesion->dataPipe);

    switch(sesion->binary) {
	case 0:
    	    sesion->dataPipe = new(SOCKET, SOCKET_CONNECT, 
				 sprintf("%s %d", ip, port),
				 (: FTP_DATA_read :),
				 (: FTP_DATA_close :));
    	    break;
	case 1:
    	    sesion->dataPipe = new(SOCKET, SOCKET_CONNECT_B, 
				 sprintf("%s %d", ip, port),
				 (: FTP_DATA_read :),
				 (: FTP_DATA_close :));
    	    break;
	default:
    	    return;
    }
    /* Mapeamos el puerto de datos al puerto de control para que podamos 
     * encontrar el puerto de control a partir del de datos 
     */
    dataports[sesion->dataPipe] = sesion->cmdPipe;
    sesion->dataPipe->set_write_callback((: FTP_Write :));
    sesion->cmdPipe->send("200 Comando PORT ejecutado correctamente.\n");
    return;
}
