
string pluralize(string str) {
    if (!str || str=="") return "";
    
    if (member_array(str[strlen(str)-1], ({ 'a', 'e', 'i', 'o', 'u' })) != -1)
	return str+"s";
    else 
	return str+"es";
}