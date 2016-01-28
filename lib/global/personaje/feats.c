#include <tablas.h>

string * feats = ({ });

int add_feat(string feat) {
    feat = lower_case(feat);
    if (!TABLA_FEATS -> requisitos?(feat)) return 0;
    feats += ({ feat });
    return 1;
}

string feat?(string feat) {
    return (member_array(feat, feats) == -1 ? 0 : 1);
}

string * feats?() {
    return feats;
}

string * feats_accesibles?() {
    string * all_feats;    

    all_feats = TABLA_FEATS -> feats?();
    foreach(string feat in all_feats) {
	if (!TABLA_FEATS -> requisitos?(feat)) {
	    all_feats -= ({ feat });
	}    
    }
    return all_feats;
}
