#include <comando.h>
#include <socket.h>
#include <mudlib.h>

object s;

void SRV_read(object socket, string data) {
    write(sprintf("Server reading data: %s by %O\n", data, socket));

    socket -> send("cocida");
}

void SRV_close(object socket) {
    write(sprintf("Server close %O\n", socket));
}

int comando(int argc, string * argv) {

    s = new(SOCKET, SOCKET_LISTEN, 4666, (: SRV_read :), (: SRV_close :));
    s -> debug();        
    return 1;
    
}

