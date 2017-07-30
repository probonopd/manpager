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

#include "compiler.hpp"
#include "args.hpp"

bool Args::conflicting(const QCommandLineParser& args, const QStringList& options) {
    unsigned count = 0;

    foreach(auto opt, options) {
        if(args.isSet(opt))
            ++count;
    }
    if(count > 1)
        return true;

    return false;
}

bool Args::includes(const QCommandLineParser& args, const QStringList& options) {
    foreach(auto opt, options) {
        if(args.isSet(opt))
            return true;
    }
    return false;
}

void Args::add(QCommandLineParser& args, const QList<Args>& list) 
{
    foreach(auto op, list) {
        add(args, op.mode, op);
    }
}

void Args::add(QCommandLineParser& args, builtin_t use, const Args& opt) {
    switch(use) {
    case VersionArgument:
        args.addVersionOption();
        break;
    case HelpArgument:
        args.addHelpOption();
        break;
    case PositionalArgument:
        args.addPositionalArgument(opt.opt.names()[0], opt.opt.description());
        break;
    case GenericArgument:
        add(args, opt);
        break;
    }
}

const QString Args::exePath(const QString& path)
{
    if(path.startsWith("../"))
        return QCoreApplication::applicationDirPath() + "/" + path;
    if(path.startsWith("./"))
        return QCoreApplication::applicationDirPath() + "/" + path.mid(3);
    if(path == ".")
        return QCoreApplication::applicationDirPath();
    return path;
}

