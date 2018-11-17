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

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <QApplication>
#include <QObject>
#include <QMainWindow>
#include <QActionGroup>
#include <QStringList>

#include "compiler.hpp"
#include "toolbar.hpp"
#include "statusbar.hpp"
#include "options.hpp"
#include "index.hpp"

class Main final : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(Main)

public:
    Main(bool reset = false);
    ~Main() final;

    static Main *instance() {
        return Instance;
    }

    inline static const QSettings& config() {
        return instance()->settings;
    }

    const QStringList paths() const {
        return manpaths;
    }

    void applyOptions(QStringList& pathlist, QCheckBox *sections[]);
    void closeOptions();

private:
    QSettings settings;
    QActionGroup *searchGroup, *selectGroup;
    Index *index;
    Toolbar *toolbar;
    Statusbar *statusbar;
    QStringList manpaths;
    Options *options;

    static Main *Instance;

    void resizeEvent(QResizeEvent *e) final;

    void load(int row);
    void sync();

signals:
    void resized();
    void updated();

public slots:
    void status(const QString& text);
    void error(const QString& text);
    void closeTab(int tab);
    void changeTab(int tab);
    void closeCurrent();
    void changeSettings();
    void reloadIndex();
    void clearTabs();
    void searchIndex(const QString& entry);
    void openNamed(const QString& text);
    void searchText();
    void enableFind();
    void updateIndex(Index *ind);

private slots:
    void about();
    void changeColumns();
    void openContext(const QPoint& pos);
    void showContextMenu(const QPoint& pos);
    void openViewer();
    void openDocument();
    void openAt(const QModelIndex& cursor);
    void openTab(int row);
};
	
/*!
 * \mainpage Manpager
 * This is a simple manpage viewer, originally written around 2011, originally
 * for Qt4 as part of Coastal-Qt.  It was later updated to compile on both
 * Qt4 and 5.  This current version is a new and only slightly refactored
 * version of the original, but for Qt5 only now.  There is further work that
 * can be done to improve this old application, including use of threaded
 * dispatch for the index loader.
 */

/*!
 * The main instance of the application.  This includes a Main singleton that
 * is both the main window and controller for the application, and the main()
 * function for argument parsing.
 * \file main.hpp
 */

#endif
