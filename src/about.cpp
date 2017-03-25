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
QDialog(parent, Qt::Dialog|Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::MSWindowsFixedSizeDialogHint)
{
    ui.setupUi(static_cast<QDialog*>(this));
    ui.labelVersion->setText(tr("Version: ") + VERSION);
    ui.labelCopyright->setText(tr("Copyright (C) ") + COPYRIGHT + " Tycho Softworks");
    setWindowTitle(tr("About Manpager"));

    connect(ui.aboutButton, &QToolButton::clicked, this, &About::aboutProject);
    connect(ui.tribalButton, &QToolButton::clicked, this, &About::aboutTribal);

    if(!parent) {
        ui.closeButton->setHidden(true);
    }
    else {
        connect(ui.closeButton, &QPushButton::clicked, this, &About::accept);
    }
}

void About::aboutProject()
{
    QDesktopServices::openUrl(QUrl("https://gitlab.com/tychosoft/manpager"));
}

void About::aboutTribal()
{
    QDesktopServices::openUrl(QUrl("https://www.cherokeesofidaho.org"));
}
