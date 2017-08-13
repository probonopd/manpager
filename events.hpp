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

#ifndef __EVENTS_HPP__
#define __EVENTS_HPP__

#include <QEvent>
#include <QString>
#include "compiler.hpp"

// TODO: this may become obsolete.

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

/*!
 * Custom events for the application.  Custom event messages are used for
 * events from the same thread context to force them into the Qt event queue,
 * since signal-slots called from the same thread context are invoked
 * immediately.  This is related to not separating loaders into their own
 * dispatch threads, and using busy cursor to block qt, so may become
 * unessisary with a better design.  At the time I also simply wanted to
 * experiment with custom events.
 * \file events.hpp
 */

#endif
