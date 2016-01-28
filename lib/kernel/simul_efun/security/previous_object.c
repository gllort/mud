
varargs mixed previous_object(int previous) {
    object * obs = APO;

    if (origin() == "local") {
	if (previous == -1) return (object *)obs;
	else if ((previous < -1) || (previous >= sizeof(obs))) return (object)0;
	else return (object)obs[previous];
    }
    else {
	int size;
	//Eliminamos el primer elemento del array.
	size = sizeof(obs);
	if (size > 1) obs = obs[1..size-1];
	else obs = ({ });
	size = sizeof(obs);
	if (previous == -1) return (object *)obs;
	else if ((previous < -1) || (previous >= size)) return (object)0;
	else return ((obs == ({ })) ? (object)0 : (object)obs[previous]);
    }
}
