/**
 ** Copyright (C) 2017 Tycho Softworks.
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

#include "main.hpp"
#include "ui_statusbar.h"

#include <QtWidgets>
#include <QStatusBar>

static Ui::Statusbar ui;

Statusbar *Statusbar::Instance = nullptr;

Statusbar::Statusbar(QWidget *parent, QStatusBar *bar, bool showDisabled) :
QWidget(parent)
{
    Q_ASSERT(Instance == nullptr);
    Instance = this;

    visibleDisabled = showDisabled;
    visibleIndicators = 0;

    ui.setupUi(this);
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    ui.line->setFixedSize(0, 0);
#endif
    bar->setFixedHeight(height() + 2);
    bar->addPermanentWidget(this);

    f3Key = new QShortcut(QKeySequence(Qt::Key_F3), parent);
    f3Key->setContext(Qt::ApplicationShortcut);

    f5Key = new QShortcut(QKeySequence(Qt::Key_F5), parent);
    f5Key->setContext(Qt::ApplicationShortcut);

    f6Key = new QShortcut(QKeySequence(Qt::Key_F6), parent);
    f6Key->setContext(Qt::ApplicationShortcut);

    f8Key = new QShortcut(QKeySequence(Qt::Key_F8), parent);
    f8Key->setContext(Qt::ApplicationShortcut);

    findKey = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), parent);
    findKey->setContext(Qt::ApplicationShortcut);

    closeKey = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), parent);
    closeKey->setContext(Qt::ApplicationShortcut);

    refreshKey = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), parent);
    refreshKey->setContext(Qt::ApplicationShortcut);

    clearIndicators();
    Main *main = Main::instance();
    connect(ui.f3Indicator, &QPushButton::clicked, main, &Main::searchText);
    connect(ui.f5Indicator, &QPushButton::clicked, main, &Main::reloadIndex);
    connect(ui.f6Indicator, &QPushButton::clicked, main, &Main::clearTabs);
    connect(ui.f8Indicator, &QPushButton::clicked, main, &Main::changeSettings);
    connect(f3Key, &QShortcut::activated, main, &Main::searchText);
    connect(f5Key, &QShortcut::activated, main, &Main::reloadIndex);
    connect(f6Key, &QShortcut::activated, main, &Main::clearTabs);
    connect(f8Key, &QShortcut::activated, main, &Main::changeSettings);
    connect(findKey, &QShortcut::activated, main, &Main::enableFind);
    connect(closeKey, &QShortcut::activated, main, &Main::closeCurrent);
    connect(refreshKey, &QShortcut::activated, main, &Main::reloadIndex);
}

void Statusbar::clearIndicators()
{
    if(!visibleDisabled) {
        visibleIndicators = 0;
        ui.f3Indicator->setVisible(false);
        //hide();
    }
    ui.f3Indicator->setEnabled(false);
    ui.f8Indicator->setEnabled(false);
    f3Key->setEnabled(false);
    f8Key->setEnabled(false);
    findKey->setEnabled(false);
    visibleIndicators = 2;  // f5 and f6 keys always visible
}

void Statusbar::enableSettings()
{
    if(!ui.f8Indicator->isVisible()) {
        ui.f8Indicator->setVisible(true);
        if(!visibleIndicators++)
            show();
    }

    ui.f8Indicator->setEnabled(true);
    f8Key->setEnabled(true);
}

void Statusbar::disableSettings()
{
    if(ui.f8Indicator->isVisible() && !visibleDisabled) {
        ui.f8Indicator->setVisible(false);
        if(!--visibleIndicators)
            hide();
    }

    ui.f8Indicator->setEnabled(false);
    f8Key->setEnabled(false);
}

void Statusbar::enableSearch()
{
    if(!ui.f3Indicator->isVisible()) {
        ui.f3Indicator->setVisible(true);
        if(!visibleIndicators++)
            show();
    }

    ui.f3Indicator->setEnabled(true);
    f3Key->setEnabled(true);
    findKey->setEnabled(true);
}

void Statusbar::disableSearch()
{
    if(ui.f3Indicator->isVisible() && !visibleDisabled) {
        ui.f3Indicator->setVisible(false);
        if(!--visibleIndicators)
            hide();
    }

    ui.f3Indicator->setEnabled(false);
    f3Key->setEnabled(false);
    findKey->setEnabled(false);
}


