
inherit __DIR__ "object/nombres";
inherit __DIR__ "object/move";
inherit __DIR__ "object/triggers";
inherit __DIR__ "object/direct_verbs";
inherit __DIR__ "object/indirect_verbs";

private int volumen;

void create() {
    nombres::create();
    move::create();
    triggers::create();
}

void volumen(int masa) {
    volumen = masa;
}

int volumen?() { return volumen; }

