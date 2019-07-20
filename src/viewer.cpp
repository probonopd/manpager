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
#include "toolbar.hpp"
#include <QDebug>
#include <cctype>

#define BOLD        0x01
#define ITALIC      0x02
#define UNDERLINE   0x04

static void getargs(char *text, char **result)
{
    unsigned count = 0;
    char *out = nullptr;

    // lower case dot commands...

    if(*text == '.') {
        while(*text && !isspace(*text)) {
            *text = static_cast<char>(tolower(*text));
            ++text;
        }
    }
    *(text++) = 0;

    while(*text && isspace(*text))
            ++text;

    if(*text == 0) {
        result[0] = nullptr;
        return;
    }

    while(*text && count < 31) {
        if(*text == '\"') {
            out = ++text;
            while(*out && *out != '\"') {
                if(out[0] == '\\' && out[1] == '\"')
                    out += 2;
                else
                    ++out;
            }
            if(*out)
                *(out++) = 0;
        }
        else {
            out = text;
            while(!isspace(*out))
                ++out;
            if(*out)
                *(out++) = 0;
        }
        result[count++] = text;
        text = out;

        while(*text && isspace(*text))
            ++text;
    }
    if(out) {
        *(++out) = '\n';
        out[1] = 0;
    }
    result[count++] = out;
    result[count] = nullptr;
}

Viewer::Viewer(QTabWidget *tabs, QIODevice& input, const QString& title)
{
    qint64 len;
    char buf[512];
    QString text;
    int argc;
    char *argv[32];
    unsigned format[9];
    unsigned indent = 0;
    bool blank = false;
    bool pre = false, pnl = false;
    bool bold, underline, italic;
    char *body, *tail;

    while((len = input.readLine(buf, sizeof(buf))) > 0) {
        bold = underline = italic = false;
        argv[0] = argv[1] = nullptr;
        memset(format, 0, sizeof(format));
        argc = 0;

        if(buf[0] != '\n')
            blank = false;
        else if(buf[0] == '\n' && indent && !blank) {
            text = text + "</p><p>";
            blank = true;
            continue;
        }

        // skip comments...
        if(!strncmp(buf, ".\\\"", 3))
            continue;

        if(*buf == '.')
            getargs(buf, argv);
        else
            argv[0] = buf;

        if(!strcmp(buf, ".th")) {
            text = text + "<h1 align=\"center\">" + argv[0] + "</h1>\n\n";
            continue;
        }
        if(!strcmp(buf, ".sh")) {
            while(indent--)
                text = text + "</p></blockquote>\n";
            text = text + "<h2>" + argv[0] + "</h2><blockquote><p>\n";
            indent = 1;
            continue;
        }

        if(!strcmp(buf, ".fi")) {
            text = text + "</pre>";
            pre = false;
            continue;
        }

        if(!strcmp(buf, ".nf") && !pre) {
            text = text + "<pre>";
            pre = true;
            continue;
        }

        if(pre)
            goto body;

        if(!strcmp(buf, ".pp") && !argv[0]) {
            text = text + "</p><p>\n";
            continue;
        }

        if(!strcmp(buf, ".tp")) {
            text = text + "</p><p>\n";
            argv[0] = nullptr;
            continue;
        }

        if(!strcmp(buf, ".br") && !argv[0]) {
            text = text + "<br>\n";
            continue;
        }

        if(!strcmp(buf, ".b") || !strcmp(buf, ".br"))
            format[0] = BOLD;
        else if(!strcmp(buf, ".ib")) {
            format[0] = ITALIC;
            format[1] = BOLD;
        }
        else if(!strcmp(buf, ".ri"))
            format[1] = ITALIC;
        else if(!strcmp(buf, ".rb"))
            format[1] = BOLD;
        else if(!strcmp(buf, ".ru"))
            format[1] = UNDERLINE;
        else if(!strcmp(buf, ".bi")) {
            format[0] = BOLD;
            format[1] = ITALIC;
        }
        else if(!strcmp(buf, ".i") || !strcmp(buf, ".ir"))
            format[0] = ITALIC;
        else if(!strcmp(buf, ".u") || !strcmp(buf, ".ur"))
            format[0] = UNDERLINE;
        else if(!strcmp(buf, ".ip") || !strcmp(buf, ".hp") || !strcmp(buf, ".de"))
            text = text + "</p><p>";
        else if(buf[0] == '.')              // if unknown, skip
            continue;

body:
        if(!indent)
            continue;

        while((body = argv[argc]) != nullptr) {
            if(pre) {
                text = text + body;
                ++argc;
                continue;
            }
            if(format[argc] & BOLD) {
                bold = true;
                text = text + "<b>";
            }
            if(format[argc] & ITALIC) {
                text = text + "<i>";
                italic = true;
            }
            if(format[argc] & UNDERLINE) {
                text = text + "<u>";
                underline = true;
            }

            bool nl = false;

            tail = body + strlen(body);

            while(tail > body && isspace(*(--tail))) {
                if(*tail == '\n')
                    nl = true;
                *tail = 0;
            }

            while(body && *body) {
                switch(*body) {
                case '>':
                    text = text + "&gt;";
                    break;
                case '<':
                    text = text + "&lt;";
                    break;
                case '\"':
                    text = text + "&quot;";
                    break;
                case '&':
                    text = text + "&amp;";
                    break;
                case '\\':
                    if(body[1] == '&' || body[1] == ':') {
                        if(!pnl) {
                            text = text + "<pre>";
                            pnl = true;
                        }
                        else
                            text = text + "\n";
                        ++body;
                        break;
                    }
                    if(tolower(body[1]) == 'p') {
                        text = text + "</p><p>";
                        ++body;
                        break;
                    }
                    if(tolower(body[1]) == 't') {
                        text = text + QChar(9);
                        ++body;
                        break;
                    }
                    if(body[1] == '/' || body[1] == ',') {
                        ++body;
                        break;
                    }
                    if(tolower(body[1]) == 'f') {
                        if(body[2] == '(') {
                            body += 3;
                            break;
                        }
                        if(body[2] == 'P') {
                            ++body;
                            break;
                        }
                        if(tolower(body[2]) == 'u') {
                            if(!underline) {
                                text = text + "<u>";
                                underline = true;
                            }
                            body += 2;
                            break;
                        }

                        if(tolower(body[2]) == 'i') {
                            if(!italic) {
                                text = text + "<i>";
                                italic = true;
                            }
                            body += 2;
                            break;
                        }

                        if(tolower(body[2]) == 'b') {
                            if(!bold) {
                                text = text + "<b>";
                                bold = true;
                            }
                            body += 2;
                            break;
                        }

                        if(tolower(body[2]) == 'r') {
                            if(underline)
                                text = text + "</u>";
                            if(italic)
                                text = text + "</i>";
                            if(bold)
                                text = text + "</b>";
                            italic = underline = bold = false;
                            body += 2;
                            break;
                        }

                    }
                    if(body[1])             // fallthrough
                        ++body;
                    else {                  // backslash can glue lines...
                        nl = false;
                        break;
                    }
                    break;
                default:
                    text = text + QChar(*body);
                }
                ++body;
            }
            if(italic)
                text = text + "</i>";
            if(underline)
                text = text + "</u>";
            if(bold)
                text = text + "</b>";
            bold = italic = underline = false;
            ++argc;
            if(pnl) {
                pnl = false;
                text = text + "</pre>";
            }
            else if(nl)
                text = text + QChar('\n');
        }
    }

    findText = "";
    findBy =nullptr;
    find = nullptr;

    setReadOnly(true);
    setEnabled(true);
    setHtml(text);

    tabPosition = tabs->count();
    tabs->addTab(this, title);
    tabs->setCurrentIndex(tabPosition);
    tabs->setTabsClosable(true);

    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Sunken);
}

void Viewer::activateFind()
{
    if(find)
        return;

    find = new Find(this, findText);
    connect(find, &Find::updateSearch, this, &Viewer::onUpdateSearch);
    connect(find, &QObject::destroyed, this, &Viewer::closeFind);
}

void Viewer::findNextText()
{
    if(findText.isEmpty())
        activateFind();
    else
        searchText();
}

void Viewer::closeFind(QObject *object)
{
    Q_UNUSED(object);
    find = nullptr;
}

void Viewer::setDirection(bool forward, bool sensitivity)
{
    if(forward)
        findBy = nullptr;
    else
        findBy = QTextDocument::FindBackward;

    if(sensitivity)
        findBy |= QTextDocument::FindCaseSensitively;
}

void Viewer::searchText()
{
    if(!findText.isEmpty()) {
        qDebug() << "Searching for" << findText;
        if(!QTextEdit::find(findText, findBy))
            findText = "";
    }
}

void Viewer::onUpdateSearch(const QString& text)
{
    findText = text;
    searchText();
}

