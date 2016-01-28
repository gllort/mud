/* 
    local_efuns.c: Local efunctions specific to MundoOscuro driver.
    This file must be included at the end of efuns_main.c.
    Their prototypes must be defined in local_efuns.h.
    Cain@MundoOscuro.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "lpc_incl.h"

/* mkpasswd() 
 *
 * Prototype that must be added to local_efuns.h: string mkpasswd(string); 
 *
 * Simple password generator by Nelson Minar (minar@reed.edu)
 * Modified by Cain@MundoOscuro.
 * May 2002.  
 */
   
void f_mkpasswd PROT((void)) {
    static char saltChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
    char salt[3];
    char * plaintext;

    srandom(time(0));		/* may not be the BEST salt, but its close */
    salt[0] = saltChars[random() % 64];
    salt[1] = saltChars[random() % 64];
    salt[2] = 0;
    plaintext = sp->u.string;
    push_constant_string(crypt(plaintext, salt));
}

