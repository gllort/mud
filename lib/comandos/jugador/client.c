#include <comando.h>
#include <socket.h>
#include <mudlib.h>

object c;

void CLI_read(object socket, string data) {
    write(sprintf("Client reading data: %s by %O\n", data, socket));
}

void CLI_close(object socket) {
    write(sprintf("Client close %O\n", socket));
}

void CLI_write(object socket) {
    write(sprintf("Client writing %O\n", socket));
}


int comando(int argc, string * argv) {

    c = new(SOCKET, SOCKET_CONNECT, sprintf("%s %d", "127.0.0.1", 4666), (: CLI_read :), (: CLI_close :));
    c -> debug();        

    c -> set_write_callback((: CLI_write :));

    write("----------\n");
    c -> send("patata\n");

    return 1;
    
}

