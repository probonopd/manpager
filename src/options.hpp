/*
 * Copyright (C) 2011-2019 David Sugar <tychosoft@gmail.com>.
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

#ifndef OPTIONS_HPP_
#define OPTIONS_HPP_

#include <QDialog>
#include <QTabWidget>

#include "compiler.hpp"

class Options final : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(Options)

public:
    Options(QTabWidget *tabs, QAction *sections[]);

    ~Options() final;

    inline int tabIndex() const {
        return tab;
    }

private slots:
    void selected(int row);
    void cancel();
    void accept() override;
    void movePathUp();
    void movePathDown();
    void removePath();
    void appendPath();

private:
    int tab, current;
};

/*!
 * This supports the options ui dialog, for setting things like search paths
 * for manpage files, case sensitivity, and enabling or disabling manual
 * sections.
 * \file options.hpp
 */

#endif
