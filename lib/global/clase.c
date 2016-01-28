
/* El nombre de la clase es el nombre relativo del fichero.
 * NO se debe cambiar!
 */
private nosave string nombre_clase = relative_name(TO);
private nosave string * skills_favoritas = ({ });
private nosave string * incompatibles = ({ });

void clase() { ; }

void create() {
    clase();
}

string nombre_clase?() { return nombre_clase; }

void skills_favoritas(string * skills) { skills_favoritas = skills; }

string * skills_favoritas?() { return skills_favoritas; }

int ataque_base?(int nivel) { return 1; }

void clases_incompatibles(string * clases) { incompatibles = clases; }

string * clases_incompatibles?() { return incompatibles; }
