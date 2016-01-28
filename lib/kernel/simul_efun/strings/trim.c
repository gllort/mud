
string trim( string s ){
  int i, j;

  j = strlen(s) - 1;
  if ( j < 0 ) return "";

  while( i <= j && member_array(s[i], " \t\n\r") != -1 ) i++;
  while( i <= j && member_array(s[j], " \t\n\r") != -1 ) j--;
  return s[i..j];
}

