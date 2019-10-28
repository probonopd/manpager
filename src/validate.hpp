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

#ifndef VALIDATE_HPP_
#define VALIDATE_HPP_

#include <QValidator>
#include <QtWidgets>

#include "compiler.hpp"

class ValidateIndex final : public QValidator
{
    Q_OBJECT
    Q_DISABLE_COPY(ValidateIndex)

public:
    ValidateIndex(QObject *parent);

    State validate(QString& input, int& pos) const final;
};

/*!
 * This has application specific input validators.  Validators are used with
 * text input fields to validate the input while the user enters text in a
 * given input control.
 * \file validate.hpp
 */

#endif
