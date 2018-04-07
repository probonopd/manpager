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

#include "main.hpp"
#include "ui_options.h"

#include <QDebug>

static QCheckBox *Sections[10];
static Ui::Options ui;

Options::Options(QTabWidget *tabs, QAction *sections[]) :
QDialog()
{
    auto main = Main::instance();

    ui.setupUi(static_cast<QDialog *>(this));
    Sections[0] = ui.section1;
    Sections[1] = ui.section2;
    Sections[2] = ui.section3;
    Sections[3] = ui.section4;
    Sections[4] = ui.section5;
    Sections[5] = ui.section6;
    Sections[6] = ui.section7;
    Sections[7] = ui.section8;
    Sections[8] = ui.sectionl;
    Sections[9] = ui.sectionn;

    current = -1;
    tab = tabs->count();
    tabs->addTab(this, tr("Options"));

    ui.list->addItems(main->paths());
    ui.list->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.list->setAttribute(Qt::WA_MacShowFocusRect, false);

    for(unsigned pos = 0; pos < 10; ++pos)
        Sections[pos]->setChecked(sections[pos]->isChecked());

    selected(-1);

    connect(ui.acceptButton, &QPushButton::clicked, this, &Options::accept);
    connect(ui.cancelButton, &QPushButton::clicked, this, &Options::cancel);
    connect(ui.upButton, &QPushButton::clicked, this, &Options::movePathUp);
    connect(ui.downButton, &QPushButton::clicked, this, &Options::movePathDown);
    connect(ui.removeButton, &QPushButton::clicked, this, &Options::removePath);
    connect(ui.appendButton, &QPushButton::clicked, this, &Options::appendPath);
    connect(ui.list, &QListWidget::currentRowChanged, this, &Options::selected);
}

Options::~Options() = default;

void Options::selected(int row)
{
    current = row;

    if(row < 0 || row >= ui.list->count()) {
        ui.upButton->setEnabled(false);
        ui.downButton->setEnabled(false);
        ui.removeButton->setEnabled(false);
        return;
    }

    if(ui.list->count() > 0)
        ui.removeButton->setEnabled(true);
    else {
        ui.upButton->setEnabled(false);
        ui.downButton->setEnabled(false);
        ui.removeButton->setEnabled(false);
        return;
    }

    if(row > 0)
        ui.upButton->setEnabled(true);
    else
        ui.upButton->setEnabled(false);

    if(row < ui.list->count() - 1)
        ui.downButton->setEnabled(true);
    else
        ui.downButton->setEnabled(false);
}

void Options::cancel()
{
    Main::instance()->closeTab(tab);
}

void Options::accept()
{
    auto main = Main::instance();

    QStringList list;
    for(int pos = 0; pos < ui.list->count(); ++pos) {
        QListWidgetItem *item = ui.list->item(pos);
        list << item->text();
    }
    main->applyOptions(list, Sections);
    main->closeTab(tab);
}

void Options::movePathUp()
{
    QListWidgetItem *item = ui.list->takeItem(current);
    ui.list->insertItem(--current, item);
    ++current;
}

void Options::movePathDown()
{
    QListWidgetItem *item = ui.list->takeItem(current);
    ui.list->insertItem(++current, item);
    --current;
}

void Options::removePath()
{
    ui.list->takeItem(current);
    if(current >= ui.list->count())
        selected(-1);
}

void Options::appendPath()
{
    auto path = QFileDialog::getExistingDirectory(this, tr("Directory"), "/");

    if(path.isNull())
        return;

    ui.list->addItem(path);

    if(current < 0)
        selected(0);
}

