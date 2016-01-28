
private nosave mapping feats = ([ ]);

void create() {
    feats["alertness"] = 1;
    feats["animal affinity"] = 1;
    feats["light armor proficiency"] = 1; 
    feats["medium armor proficiency"] = (: TP->feat?("light armor proficiency") ? 1 : 0 :);
    feats["heavy armor proficiency"] = (: TP->feat?("medium armor proficiency") ? 1 : 0 :);
    feats["augment summoning"] = (: TP->feat?("spell focus") ? 1 : 0 :);    
    feats["blind fight"] = 1;
    feats["combat casting"] = 1;
    feats["combat expertise"] = (: TP->tel?()>=13 ? 1 : 0 :);
    feats["improved disarm"] = (: ((TP->tel?()>=13) && (TP->feat?("combat expertise"))) ? 1 : 0 :);
    feats["improved feint"] = (: ((TP->tel?()>=13) && (TP->feat?("combat expertise"))) ? 1 : 0 :);
    feats["improved trip"] = (: ((TP->tel?()>=13) && (TP->feat?("combat expertise"))) ? 1 : 0 :);

    /* Falta prereq attack bonus = +4 */
    /* Quitamos -> spring attack y mobility */
    feats["whirlwind attack"] = (: ((TP->dex?()>=13) && (TP->tel?()>=13) && (TP->feat?("combat expertise")) && (TP->feat?("dodge"))) ? 1 : 0 :);

    feats["deceitful"] = 1;
    feats["deft hands"] = 1;
    feats["diligent"] = 1;
    feats["dodge"] = (: TP->des?()>=13 ? 1 : 0 :);
    feats["diehard"] = 1; /* pre: endurance */
    feats["eschew materials"] = 1;

    /* pre: Base attack bonus = +1 */
    feats["exotic weapon proficiency"] = (: TP->tel?()>=13 ? 1 : 0 :);

    /* ... */
}

void debug() {
    printf("%O\n", feats);
}

int requisitos?(string feat) {
    if (!feats[feat]) return 0;
    else return evaluate(feats[feat]);
}

string * feats?() {
    return keys(feats);
}
