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

#ifndef __INDEX_HPP__
#define __INDEX_HPP__

#include <QAbstractTableModel>
#include <QString>
#include <QObject>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QAction>

#include "compiler.hpp"

class Index final : public QAbstractTableModel
{
	Q_OBJECT
    Q_DISABLE_COPY(Index)

public:
    typedef enum {NONE, GZIP} Compression;

    typedef struct  {
        QString name;
        QString section;
        Compression compressed;
        int path;
        char id;
    } item_t;

    Index(QObject *parent, QObject *main, QAction *sections[], QStringList paths);
    ~Index() final;

    QString nameAt(int row) const;
    const item_t& itemAt(int row) const;

    int search(const QString& name) const;
    int find(const QString& name) const;
    void selectAt(int pos, const QString& name);

private:
	QList<item_t> items;
    bool triggered;
    int rows, first, last;
    int *map;

    inline QString nameFrom(int pos) const {
        return items[map[pos]].name + "." + items[map[pos]].section;
    }

    inline QString findFrom(int pos, int len) const {
        return items[map[pos]].name.left(len);
    }

    int rowCount(const QModelIndex& parent) const final;
    int columnCount(const QModelIndex& parent) const final;
    QVariant data(const QModelIndex &index, int role) const final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;

signals:
    void selected(const QString& text);
};

/*!
 * Data model for index of manual pages.
 * \file index.hpp
 */

/*!
 * \class Index
 * \brief Index data model of manual page files.
 * This is a model to load and access manual pages.  It contains a list of
 * files, what manual section they belong to, and the ui name to show them as.
 * \author David Sugar <tychosoft@gmail.com>
 *
 * \typedef Index::item_t
 * Individual manual page item node.
 *
 * \enum Index::Compression
 * The file compression used for the file being referenced.
 *
 * \var Index::NONE
 * \brief No file compression used.
 * \var Index::GZIP
 * \brief Gzip file compression used.
 */

#endif
