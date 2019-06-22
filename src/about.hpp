/*
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

#ifndef ABOUT_HPP_
#define ABOUT_HPP_

#include <QDialog>
#include "compiler.hpp"

class About final : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(About)

public:
    About(QWidget *parent = nullptr);
};

/*!
 * Ui about dialog.
 * \file about.hpp
 */

/*!
 * \class About
 * \brief impliments about box ui.
 * This is used to activate the toolbox buttons which connect to the browser
 * to bring you to the link for the project on gitlab, and the tribal logo.
 * \author David Sugar <tychosoft@gmail.com>
 */

#endif
