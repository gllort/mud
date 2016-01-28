#include <mudlib.h>

inherit VERBO;

private nosave array salidas_comunes = ({ "norte", "sur", "este", "oeste", 
				  	  "noreste", "noroeste", "sureste", 
					  "suroeste", "arriba", "abajo", 
					  "dentro", "fuera" });

void create() {
    ::create();
    crear_regla( ({ "STR", "a STR", "hacia STR" }) );
}

void do_ir_str(string direccion) {
    ETP -> andar(direccion);
}

void do_ir_a_str(string direccion) {
    ETP -> andar(direccion);
}

void do_ir_hacia_str(string direccion) {
    ETP -> andar(direccion);
}

mixed check_exits(string direccion) {
    object env = ETP;
    if (env->existe_salida?(direccion) == 1) return 1;
    if (member_array(direccion, salidas_comunes) != -1) return "No hay salida en esa direccion.\n";
    return 0;
}

mixed can_ir_str(string direccion) { return check_exits(direccion); }
mixed can_ir_a_str(string prep, string direccion) { return check_exits(direccion); }
mixed can_ir_hacia_str(string prep, string direccion) { return check_exits(direccion); }

