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

#include "index.hpp"
#include "events.hpp"
#include <QApplication>
#include <QStringList>
#include <QMap>
#include <QDir>

static const char *cmap = "12345678ln";
static QStringList mandirs = QStringList() << "man1" << "man2" << "man3" << "man4" << "man5" << "man6" << "man7" << "man8" << "manl" << "mann";

Index::Index(QObject *parent, QObject *main, QAction *sections[], QStringList paths) :
QAbstractTableModel(parent)
{
	int ext;
	item_t item;
	map = nullptr;
    rows = first = last = 0;
    QMap<QString, int> byname;

    for(int section = 0; section < 10; ++section) {
        StatusEvent ev(tr("loading ") + cmap[section] + "...");
        QApplication::sendEvent(main, &ev);
        for(int path = 0; path < paths.size(); ++path) {
            QDir dir(paths[path] + QDir::separator() + mandirs[section]);
            if(!dir.exists())
                continue;
            if(!sections[section]->isChecked())
                continue;

            item.path = path;
            item.compressed = NONE;
            item.id = cmap[section];

            QStringList list = dir.entryList(QDir::Files);
            for(int pos = 0; pos < list.size(); ++pos) {
                QString entry = list[pos];
                ext = entry.lastIndexOf('.');
                if(ext < 2)
                    continue;

                if(entry.mid(ext) == ".gz") {
                    item.compressed = GZIP;
                    entry = entry.left(ext);
                    ext = entry.lastIndexOf('.');
                    if(ext < 2)
                        continue;
                }

                item.name = entry.left(ext);
                item.section = entry.mid(++ext);
                items << item;

                byname[entry] = rows;
                ++rows;
            }
        }
    }

    if(!rows)
        return;

    map = new int[rows];
    foreach(int value, byname)
        map[last++] = value;

    rows = last;
}

Index::~Index()
{
	if(map) {
        delete[] map;
		map = nullptr;
	}
}

int Index::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return rows;
}

int Index::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant Index::data(const QModelIndex& index, int role) const
{
    int row = index.row();

    if(row >= (last - first) || row < 0 || role != Qt::DisplayRole)
        return QVariant();

    switch(index.column()) {
    case 0:
        return items[map[row + first]].name;
    case 1:
        return items[map[row + first]].section;
    default:
        return QVariant();
    }
}

QVariant Index::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch(section) {
    case 0:
        return tr("Name");
    case 1:
        return tr("Section");
    default:
        return QVariant();
    }
}

QString Index::nameAt(int row) const
{
    const item_t& item = itemAt(row);
    return item.name + "." + item.section;
}

const Index::item_t& Index::itemAt(int row) const
{
    return items[map[row + first]];
}

void Index::selectAt(int pos, const QString& name)
{
    int len = name.length();

    if(len < 1) {
        first = 0;
        last = rows;
        return;
    }

    first = last = pos;
    while(last < rows && name == findFrom(last, len))
        ++last;

    // signal input combobox if single entry found

    if(last != first + 1) {
        triggered = false;
        return;
    }

    if(triggered)
        return;

    emit selected(nameFrom(first));
    triggered = true;
}

int Index::search(const QString& name) const
{
    int diff = (last - first) / 2;
    int pos = first + diff;
    QString check = nameFrom(pos);
    int len = name.length();

    while(diff > 0) {
        diff /= 2;

        if(name == check.left(len))
            return pos - first;

        if(name < check) {
            if(diff)
                pos -= diff;
            else
                --pos;
        }
        else {
            if(diff)
                pos += diff;
            else
                ++pos;
        }
        if(pos < first)
            pos = first;
        if(pos >= last)
            pos = last - 1;

        check = nameFrom(pos);
    }

    while(name < check.left(len) && pos > first) {
        --pos;
        check = nameFrom(pos);
    }

    while(name > check.left(len) && pos < (last - 1)) {
        ++pos;
        check = nameFrom(pos);
    }

    if(name != check.left(len))
        return -1;

    return pos - first;
}

int Index::find(const QString& name) const
{
    int diff = rows / 2;
    int pos = diff;
    int len = name.length();
    if(rows < 1)
        return -1;

    QString check = findFrom(pos, len);

    while(diff > 0) {
        diff /= 2;

        if(name == check)
            break;

        if(name < check) {
            if(diff)
                pos -= diff;
            else
                --pos;
        }
        else {
            if(diff)
                pos += diff;
            else
                ++pos;
        }
        if(pos >= rows)
            pos = rows - 1;

        check = findFrom(pos, len);
    }

    while(name < check && pos > 0)
        check = findFrom(--pos, len);

    while(name > check && pos < rows - 1)
        check = findFrom(++pos, len);

    if(name != findFrom(pos, len))
        return -1;

    while(pos > 0 && name == findFrom(pos - 1, len))
        --pos;

    return pos;
}
