/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Portions copyright Copyright (C) 2010 Stellar Science                       *
 *                                                                             *
 * This file is part of xylose                                                 *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/

/** \file
 * Windows compatibility layer for <unistd.h>.
 */

#ifndef xylose_compat_unistd_hpp
#define xylose_compat_unistd_hpp

#ifndef WIN32
#  include <unistd.h>
#else

#include <xylose/compat/config.hpp>

#if defined(USE_WINSOCK)
// you will also have to link to ws2_32.lib
#  include <winsock2.h> // for gethostname
#  pragma comment(lib, "ws2_32.lib")
#endif

#include <windows.h>
#include <direct.h> // for _getcwd, chdir
#include <io.h> // for _unlink
#include <stdio.h> // for _unlink

#include <ctime>

#if defined(_MSC_VER)
  #undef min
  #undef max
#endif

namespace xylose {
  namespace compat {

    inline void sleep(int t) {
    	Sleep(t*1000); // Sleep uses milliseconds
    }

    #ifndef USE_WINSOCK
      inline void gethostname(char*name, const int len) {
      	strncat(name,"WindozeBox",std::min(10,len-1));
      }
    #endif

    inline unsigned long getpid() {
      return static_cast<unsigned long>( GetCurrentProcessId() );
    }


    static const int _SC_CLK_TCK = 1;

    inline long sysconf(int name) {
      switch (name) {
        case _SC_CLK_TCK:
          return CLOCKS_PER_SEC;
          break;

        default:
          return -1;
      }
    }


    #if WIN32_POSIX_REMOVED_COMPLETELY
    //MSDN docs say that these won't exist(?), but they seem to...

    inline char * getcwd(char * buf, const size_t & size) {
      return _getcwd(buf, size);
    }


    inline int chdir(const char * path) {
      return _chdir(path);
    }


    inline int unlink( const char *path ) {
      return _unlink(path);
    }
    #endif

  }/* namespace xylose::compat */
}/* namespace xylose::compat */


/* import xylose::compat into :: */
using namespace xylose::compat;

#endif // WIN32

#endif // xylose_compat_unistd_hpp
