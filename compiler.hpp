/*
 * Copyright 2017 Tycho Softworks.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef	__COMPILER_HPP__
#define __COMPILER_HPP__

#ifdef __clang__
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#if __cplusplus <= 199711L && !defined(_MSC_VER)
#error C++11 compliant compiler required
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

/*!
 * Provides pragmas and macros for use in every compilation unit.
 * \file compiler.hpp
 */

/*!
 * \def FOR_DEBUG(block)
 * Defines a block of code only built for debug builds.
 */

/*!
 * \def FOR_RELEASE(block)
 * Defines a block of code only built in release builds.
 */

#endif
