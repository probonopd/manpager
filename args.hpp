/*
 * Copyright (C) 2017 Tycho Softworks.
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

#ifndef __ARGS_HPP__
#define __ARGS_HPP__

#include <QCommandLineParser>
#include <QStringList>
#include <QPair>
#include <QDebug>

#include "compiler.hpp"

class Args final
{
public:
    enum ArgType {
        VersionArgument,
        HelpArgument,
/*! \cond PRIVATE */
        PositionalArgument,
        GenericArgument
/*! \endcond */
    };

    inline Args(const QStringList& flags, const QString& help, const QString& range, const QString& value) : opt(flags, help, range, value), mode(GenericArgument) {}
    
    inline Args(const QStringList& flags, const QString& help) : opt(flags, help), mode(GenericArgument) {}

    inline Args(const QPair<QString,QString>& pos) : opt(pos.first, pos.second), mode(PositionalArgument) {}

    inline Args(ArgType builtin) : opt("tmp", ""), mode(builtin) {}

    static void add(QCommandLineParser& args, const QList<Args>& list);
    static bool conflicting(const QCommandLineParser& args, const QStringList& options);
    static bool includes(const QCommandLineParser& args, const QStringList& options);
    static const QString exePath(const QString& path);

private:
    QCommandLineOption opt;
    ArgType mode;

    inline static void add(QCommandLineParser& args, const Args& opt) {
        args.addOption(opt.opt);
    }

    static void add(QCommandLineParser& args, ArgType use, const Args& opt = Args(GenericArgument));
};

/*!
 * Command line parsing support.
 * \file args.hpp
 */

/*!
 * \class Args
 * \brief Provides a simple means to initialize command line argument lists.
 * This was originally created simply because the Qt version on Debian 8 (5.3)
 * did not correctly support c++11 initializer lists for QCommandLineOption.
 * Since then it has been expanded to be more generally useful and offer other
 * features needed in argument parsing.
 *
 * The main function to drive this is really
 * Args::add(QCommandLineParser&,const QList<Args>&) to construct a list of
 * arguments that represents the full command line interface all at once.  The
 * various Args class constructors are meant to be used as list initializers
 * for this to create a list of QCommandLineOption objects (as derived thru
 * Args constructors).  Afterward the QCommandLineParser.process(parser) method
 * can be used to process the argument list that was constructed.
 * \author David Sugar <tychosoft@gmail.com>
 *
 * \enum Args::ArgType
 * Used to distinguise the type of argument entry in an arguments list.
 * Normally only the Args::VersionArgument and Args::HelpArgument are used
 * with the Args(ArgType) constructor as part of an initialization list.
 *
 * \var Args::VersionArgument
 * \brief Adds --version flag to command parser.
 * \var Args::HelpArgument
 * \brief Adds --help flag to command parser.
 *
 * \fn Args::Args(const QStringList& flags, const QString& help, const QString& range, const QString& value)
 * Used to add a --flag=value option to the args initializer list.
 * \param flags List of flag strings (synonyms).
 * \param help Message text to use for --help to describe this flag.
 * \param range Message text to describe valid range of values.
 * \param value Default value to use for this flag option.
 *
 * \fn Args::Args(const QStringList& flags, const QString& help)
 * Used to add a --flag or -f flag boolean option in an Args initializer list.
 * \param flags List of flag strings.
 * \param help Message text to use for --help to describe this flag.
 *
 * \fn Args::Args(const QPair<QString,QString>& pos)
 * Used to add a positional option in an Args initializer list.
 * \param pos A string pair to hold both the positional argument id for the
 * "usage" message, and the message text for a --help command description.
 *
 * \fn Args::Args(ArgType mode)
 * Used in Args::add() to add a Args::VersionArgument or Args::HelpArgument
 * option in an Args initializer list.
 * \param mode Either Args::Version or Args::HelpArgument
 *
 * \fn Args::add(QCommandLineParser &args, const QList<Args> &list)
 * Used to add a (c++11 style) initializer list of command line options to
 * the current parser. This is often the only member function directly used.
 * The individual constructors provide the means to add entries by type.
 * \param args A command line parser instance.
 * \param list A list of arguments, typically a c++11 style initializer list.
 *
 * \fn Args::exePath(const QString& path)
 * Compute a path relative to the executable programs path.  This can be used
 * to find things like ../share from /usr/bin/exec-name.
 * \param path Relative path to search from.
 * \return full Path of target directory or file releative to exe file.
 *
 * \fn Args::conflicting(const QCommandLineParser& args, const QStringList& options)
 * Used to find if conflicting command line options that cannot be used
 * together are present.  This is typically for parsers that use --subcommand
 * style parsing.
 * \param args Parser instance to examine.
 * \param options List of options by name that cannot co-exist.
 * \return true if conflicting options are present.
 *
 * \fn Args::includes(const QCommandLineParser& args, const QString& options)
 * Find if at least one of the required options is included.
 * \param args Parser instance to examine.
 * \param options List of options by name to search for.
 * \return true if any of the list members is found.
 *
 * \section Example
 * A typical use case.
 * \code
    #include "args.hpp"
    ...
    QApplication myapp(argc, argv);
    QCommandLineParser args;
    Args::add(args, {
        {Args::HelpArgument},
        {Args::VersionArgument},
        {{"reset"}, tr("Reset Config")},
    });
    args.process(myapp);
  \endcode
 */

#endif
