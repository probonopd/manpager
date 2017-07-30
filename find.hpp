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

#include <QDialog>
#include <QLineEdit>

#include "compiler.hpp"

class Viewer;

class Find final : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(Find)

public:
    Find(Viewer *view, const QString& search);

    inline static void setSensitive(bool flag) {
        Sensitivity = flag;
    }

    inline static bool sensitive() {
        return Sensitivity;
    }

private:
    QLineEdit *editor;

    static bool Sensitivity;

signals:
    void updateSearch(const QString& text);

private slots:
    void onForward(void);
    void onBackward(void);
    void onEnter(void);
};


