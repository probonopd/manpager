/**
 ** Copyright (C) 2017 Tycho Softworks.
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

// Pragmas and options to include in every compilation unit

#ifdef __clang__
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#ifdef _MSC_VER
#if _MSC_VER < 1800
#error "VS >= 2013 or later required"
#endif

#if _MSC_VER < 1900
#include <xkeycheck.h>
#define noexcept
#endif

#if !defined(_MT) && !defined(__MT__)
#error "multi-threaded build required"
#endif
#endif

#ifdef QT_NO_DEBUG
#define FOR_DEBUG(x)
#define FOR_RELEASE(x)    {x;}
#else
#define FOR_DEBUG(x)      {x;}
#define FOR_RELEASE(x)
#endif

