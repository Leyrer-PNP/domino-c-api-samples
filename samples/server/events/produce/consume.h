/****************************************************************************
 *
 * Copyright HCL Technologies 1996, 2023.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
    PROGRAM:    consume

    FILE:       consume.h

****************************************************************************/
#include <globerr.h>

#define SERVER_ADDIN_NAME (PKG_ADDIN+0)  /* PKG_ADDIN+0 must be the name of the task */
#define ADDIN_VERSION (PKG_ADDIN+1)      /* PKG_ADDIN+1 must be the version number of the task */
#define ADDIN_MSG_FMT (PKG_ADDIN+2)      /* user-defined strings are PKG_ADDIN+2 or greater */

