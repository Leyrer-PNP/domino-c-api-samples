/****************************************************************************

    PROGRAM:    consume

    FILE:       consume.h

****************************************************************************/
#include <globerr.h>

#define SERVER_ADDIN_NAME (PKG_ADDIN+0)  /* PKG_ADDIN+0 must be the name of the task */
#define ADDIN_VERSION (PKG_ADDIN+1) /* PKG_ADDIN+1 must be the version number of the task */
#define ADDIN_MSG_FMT (PKG_ADDIN+2) /* user-defined strings are PKG_ADDIN+2 or greater	*/

#define EVENT_BUF_SIZE  128


