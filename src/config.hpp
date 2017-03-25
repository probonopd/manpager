/**
 ** Copyright (C) 2011-2017 Tycho Softworks.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QDebug>

#if defined(Q_OS_MAC)
#define CONFIG_FROM "tychosoft.com", "manpager"
#define CSS_STYLE ":/styles/macosx.css"
#elif defined(Q_OS_WIN)
#define CONFIG_FROM "Tycho Softworks", "Manpager"
#define CSS_STYLE ":/styles/windows.css"
#else
#define CONFIG_FROM "tychosoft.com", "manpager"
#define CSS_STYLE ":/styles/default.css"
#endif

#if defined(Q_OS_WIN)
#define PATHLIST_GROUPING ";"
#else
#define PATHLIST_GROUPING ":"
#endif

#define CONFIG_STR1(x) #x
#define CONFIG_STR(x) CONFIG_STR1(x)
#define	VERSION CONFIG_STR(PROJECT_VERSION)
#define COPYRIGHT CONFIG_STR(PROJECT_COPYRIGHT)

#ifdef	PROJECT_TESTDATA
#define	TESTDATA_PATH CONFIG_STR(PROJECT_TESTDATA)
#endif
