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

#include <QApplication>
#include "validate.hpp"
#include "main.hpp"

ValidateIndex::ValidateIndex(QObject *parent) :
QValidator(parent)
{
}

QValidator::State ValidateIndex::validate(QString& input, int& pos) const
{
    if(input.length() < 1)
        return Intermediate;

    QChar last = input[pos - 1];

    if(last != QChar(':') && last != QChar('.') && last != QChar('_') && last != QChar(' ') && !last.isLetterOrNumber())
        goto invalid;

    if(input.lastIndexOf('.') == -1)
        return Intermediate;

    if(input[0] == '.')
        goto invalid;

    if(input.lastIndexOf('.') != input.indexOf('.'))
        goto invalid;

    if(input[input.length() - 1] == '.')
        return Intermediate;

    return Acceptable;

invalid:
    Main *main = Main::instance();
    main->error(tr("Invalid input ") + QChar('\"') + input + QChar('\"'));
    return Invalid;
}


