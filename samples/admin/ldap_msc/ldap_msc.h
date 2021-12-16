/****************************************************************************

    PROGRAM:      ldap_msc

    FILE:         ldap_msc.h

    DESCRIPTION:	Header file containing definitions for the API program
	               'ldap_msc'.

****************************************************************************/

#include "global.h"
#include "misc.h"
#include "osmisc.h"

#include "ldap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT         389
#define FILTER       "(cn=j*)"

/*#define MOD_DN       "CN=John Public, O=HCLPNP"*/
#define MOD_DN       "CN=testuser1"
/*#define MOD_NDN      "CN=John Q Public, O=HCLPNP"*/
#define MOD_NDN      "CN=testuser2"
/*#define MOD_NRDN     "CN=John Q Public"*/
#define MOD_NRDN     "CN=testuser2"
