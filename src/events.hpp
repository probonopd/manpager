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

#include <QEvent>
#include <QString>
#include "compiler.hpp"

typedef enum {
    ERROR_EVENT = QEvent::User + 1,
    STATUS_EVENT
} events_t;

class ErrorEvent final : public QEvent
{
    Q_DISABLE_COPY(ErrorEvent)

public:
    ErrorEvent(QString msg) : QEvent(static_cast<QEvent::Type>(ERROR_EVENT)) {
        Text = msg;
	}

    ~ErrorEvent() final;

    const QString text() const {
        return Text;
    }

private:
    QString Text;
};

class StatusEvent final : public QEvent
{
public:
    StatusEvent(QString msg) : QEvent(static_cast<QEvent::Type>(STATUS_EVENT)) {
        Text = msg;
    }

    ~StatusEvent() final;

    const QString text() const {
        return Text;
    }

private:
    QString Text;
};
