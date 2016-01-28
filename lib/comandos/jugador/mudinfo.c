#include <comando.h>

#define WIDTH 14
#define PAD(x,y) sprintf("%-*s: %s\n",WIDTH,x,"" + y)

int comando(int argc, string * argv) {
    mapping r;
    float cpu;
    string mach, vers, name, up, cc, loadav, tmp;
    int obs, memory, callouts, nusers;
   
    r = rusage();
    obs = sizeof( objects() );
    mach = __ARCH__;
#ifndef MSDOS
    cc = __COMPILER__;
#else
    cc = "Probablemente GCC";
#endif
#ifndef MUD_NAME
    name = "Desconocido";
#else
    name = MUD_NAME + "  ("+query_host_name()+" "+__PORT__+")";
#endif
    vers = __VERSION__;
    cpu = ((r["utime"] + r["stime"]) / uptime() / 10.0);
    memory = memory_info();
    up = ""+uptime()+" segundos";
    loadav = query_load_average();
    callouts = sizeof(call_out_info());
    nusers = sizeof(users());
    
    tmp = PAD("Nombre MUD", name) +
    	  PAD("Driver", vers) +
          PAD("Arquitectura", mach) +
          PAD("Compilador", cc) +
          PAD("Uptime", up) +
          PAD("Uso medio CPU", sprintf("%4.2f%%", cpu)) +
          PAD("Carga media", loadav) +
          PAD("Usuarios", nusers+" "+(nusers == 1 ? "conectado" : "conectados")) +
          PAD("Objetos", obs + " cargados en memoria") +
          PAD("Callouts", callouts + " pendientes") +
          PAD("Uso memoria", memory);

    write(sprintf("%-*#s\n", 80, tmp));
    return 1;
}
