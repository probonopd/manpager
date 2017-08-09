/*
 * This file is part of the Manpager project.
 * Copyright (C) 2011-2017 Tycho Softworks.
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

#include <QObject>
#include "compiler.hpp"

// TODO: Eliminate this horrible idea, replace loaders with dispatch threads.
class BusyCursor final : public QObject
{
	Q_OBJECT
    Q_DISABLE_COPY(BusyCursor)

public:
    BusyCursor();
    ~BusyCursor();

private:
    bool eventFilter(QObject *obj, QEvent *event) final;
};

/*!
 * Blocking application with busy cursor.
 * \file busycursor.hpp
 */

/*!
 * \class BusyCursor
 * \brief Block application with a busy cursor.
 * This was a cheap and probably really bad way to disable ui response while
 * the application loads indexes or views from files.  You basically create an
 * instance of BusyCursor when you want to block ui, and often this is done
 * in a scoped block along with the function that does the long term or
 * blocking operation.  In reality, I want to replace these load functions
 * with dispatch threads that do loading and parsing without blocking Qt ui
 * functionality, and then have them signal updates to main when they complete.
 * That will happen in a future release making this obsolete.
 * \author David Sugar <tychosoft@gmail.com>
 */
