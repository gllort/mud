#include <config.h>

string strip(string str) {
    string str1, str2;
    int num1, num2;

    str = replace_string(str, "$N", "");
    str = replace_string(str, "$R", "");
    str = replace_string(str, "$S", "");
    str = replace_string(str, "$P", "");
    str = replace_string(str, "$n", "");
    str = replace_string(str, "$r", "");
    str = replace_string(str, "$s", "");
    str = replace_string(str, "$p", "");
    while(sscanf(str, "%s"+COLOR_MARK+"%d,%d%s", str1, num1, num2, str2)==4) {
	str = str1+str2;
    }
    while(sscanf(str, "%s"+COLOR_MARK+"%d%s", str1, num1, str2)==3) {
	str = str1+str2;
    }
    return str;
}
