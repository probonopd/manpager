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

#include <QAbstractTableModel>
#include <QString>
#include <QObject>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QAction>

#include "compiler.hpp"

typedef enum {NONE, GZIP} comp_t;

typedef struct  {
	QString name;
	QString section;
    comp_t mode;
    int path;
    char id;
} item_t;

class Index final : public QAbstractTableModel
{
	Q_OBJECT
    Q_DISABLE_COPY(Index)

public:
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
