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

#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include <QTextEdit>
#include <QKeyEvent>
#include <QTableWidget>
#include <QIODevice>

#include "compiler.hpp"
#include "find.hpp"

class Viewer final : public QTextEdit
{
    Q_DISABLE_COPY(Viewer)

public:
    Viewer(QTabWidget *tabs, QIODevice &input, const QString &title);

    inline const QString currentSearch() const {
        return findText;
    }

    inline void release() {
        if(find)
            find->close();
    }

    void setDirection(bool forward, bool sensitivity);

private:
    QTextDocument::FindFlags findBy;
    QString findText;
    Find *find;
    int tabPosition;

    void searchText(void);

public slots:
    void activateFind();
    void findNextText();

private slots:
    void onUpdateSearch(const QString& text);
    void closeFind(QObject *object);
};

/*!
 * A searchable text view of the currently selected manual page.  This is a
 * QTextView composed by parsing the man page nroff into rich text html view.
 * \file viewer.hpp
 */

#endif

