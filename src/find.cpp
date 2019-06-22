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

#include "viewer.hpp"
#include "ui_find.h"

bool Find::Sensitivity = false;

Find::Find(Viewer *view, const QString& search) :
QDialog(view, Qt::Popup|Qt::WindowTitleHint|Qt::WindowCloseButtonHint)
{
    Ui::FindDialog ui;

    ui.setupUi(qobject_cast<QDialog *>(this));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Find Text"));

    editor = ui.seekEdit;
    editor->setText(search);
    editor->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui.seekEdit, &QLineEdit::returnPressed, this, &Find::onEnter);
    connect(ui.nextButton, &QPushButton::clicked, this, &Find::onForward);
    connect(ui.prevButton, &QPushButton::clicked, this, &Find::onBackward);

    activateWindow();
    editor->setFocus();
    show();
}

void Find::onEnter()
{
    if(editor->text().isEmpty()) {
        emit updateSearch("");
        close();
        return;
    }
    onForward();
    close();
}

void Find::onForward()
{
    auto *view = static_cast<Viewer *>(parent());
    auto seeking = editor->text();

    view->setDirection(true, Sensitivity);

    if(seeking.isEmpty()) {
        emit updateSearch("");
        close();
        return;
    }

    if(view->currentSearch().isEmpty()) {
        QTextCursor cursor(view->textCursor());
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
        view->setTextCursor(cursor);
    }
    emit updateSearch(seeking);
}

void Find::onBackward()
{
    auto *view = static_cast<Viewer *>(parent());
    auto seeking = editor->text();

    view->setDirection(false, Sensitivity);
    
    if(seeking.isEmpty()) {
        emit updateSearch("");
        close();
        return;
    }

    if(view->currentSearch().isEmpty()) {
        QTextCursor cursor(view->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        view->setTextCursor(cursor);
    }
    emit updateSearch(seeking);
}


