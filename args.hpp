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

#include <QCommandLineParser>
#include <QStringList>
#include <QPair>
#include <QDebug>

#include "compiler.hpp"

class Args
{
public:
    typedef enum {
        VersionArgument,
        HelpArgument,
        PositionalArgument,
        GenericArgument
    } builtin_t;

    inline Args(const QStringList& flags, const QString& help, const QString& range, const QString& value) : opt(flags, help, range, value), mode(GenericArgument) {}
    
    inline Args(const QStringList& flags, const QString& help) : opt(flags, help), mode(GenericArgument) {}

    inline Args(const QPair<QString,QString>& pos) : opt(pos.first, pos.second), mode(PositionalArgument) {}

    inline Args(builtin_t builtin) : opt("tmp", ""), mode(builtin) {}

    inline static void add(QCommandLineParser& args, const Args& opt) {
        args.addOption(opt.opt);
    }

    static void add(QCommandLineParser& args, builtin_t use, const Args& opt = Args(GenericArgument));
    static void add(QCommandLineParser& args, const QList<Args>& list);
    static bool conflicting(const QCommandLineParser& args, const QStringList& options);
    static bool includes(const QCommandLineParser& args, const QStringList& options);
    static const QString exePath(const QString& path);

private:
    QCommandLineOption opt;
    builtin_t mode;
};
