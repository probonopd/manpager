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

#include "config.hpp"
#include "about.hpp"
#include "ui_about.h"
#include <QUrl>
#include <QDesktopServices>

static Ui::About ui;

About::About(QWidget *parent) :
QDialog(parent, Qt::Popup|Qt::WindowTitleHint|Qt::WindowCloseButtonHint)
{
    ui.setupUi(static_cast<QDialog*>(this));
    ui.labelVersion->setText(tr("Version: ") + PROJECT_VERSION);
    ui.labelCopyright->setText(tr("Copyright (C) ") + PROJECT_COPYRIGHT + " Tycho Softworks");
    setWindowTitle(tr("About ") + PROJECT_TARGET);

    connect(ui.aboutButton, &QToolButton::clicked, this, []{
          QDesktopServices::openUrl(QUrl("https://gitlab.com/tychosoft/" PROJECT_ARCHIVE));
    });

    connect(ui.supportButton, &QToolButton::clicked, this, []{
          QDesktopServices::openUrl(QUrl("https://cloud.tychosoft.com"));
    });

    connect(ui.developButton, &QToolButton::clicked, this, []{
          QDesktopServices::openUrl(QUrl("https://gitlab.com/tychosoft/" PROJECT_ARCHIVE));
    });

    connect(ui.contactButton, &QToolButton::clicked, this, []{
          QDesktopServices::openUrl(QUrl("https://www.gnutelephony.org"));
    });

    if(!parent) {
        ui.closeButton->setHidden(true);
    }
    else {
        connect(ui.closeButton, &QPushButton::clicked, this, &About::accept);
    }
}

