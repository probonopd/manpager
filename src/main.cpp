/**
 ** Copyright (C) 2011-2019 David Sugar <tychosoft@gmail.com>.
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
#include "about.hpp"
#include "viewer.hpp"
#include "args.hpp"
#include "ui_main.h"

#include <QStandardPaths>

static Ui::MainWindow ui;
static QAction *manSections[10];
static Viewer *currentView = nullptr;
static QSettings settings("tychosoft.com", "textseeker");
static bool night = false;

Main *Main::Instance = nullptr;

Main::Main()
{
    Q_ASSERT(Instance == nullptr);

    index = nullptr;
    options = nullptr;
    Instance = this;

    // ui setup and object initialization...

    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setIconSize(QSize(16, 16));

    ui.setupUi(static_cast<QMainWindow *>(this));
    toolbar = new Toolbar(this, ui.toolBar);
    statusbar = new Statusbar(ui.centralwidget, ui.statusbar);
    statusbar->enableSettings();
    status(tr("loading..."));

    manSections[0] = ui.actionSection1;
    manSections[1] = ui.actionSection2;
    manSections[2] = ui.actionSection3;
    manSections[3] = ui.actionSection4;
    manSections[4] = ui.actionSection5;
    manSections[5] = ui.actionSection6;
    manSections[6] = ui.actionSection7;
    manSections[7] = ui.actionSection8;
    manSections[8] = ui.actionSectionl;
    manSections[9] = ui.actionSectionn;

    searchGroup = new QActionGroup(this);
    ui.actionIndex->setActionGroup(searchGroup);
    ui.actionKeywords->setActionGroup(searchGroup);

    ui.indexView->setEnabled(false);
    ui.indexView->setShowGrid(false);
    ui.indexView->setSortingEnabled(false);
    ui.indexView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.indexView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.indexView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.indexView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

// TODO: Context menu; openDocument not reliable for this
//    ui.indexView->setContextMenuPolicy(Qt::CustomContextMenu);
    setContextMenuPolicy(Qt::CustomContextMenu);

    // apply and initialize settings...

#ifdef PROJECT_TESTDATA
    manpaths <<  PROJECT_TESTDATA;
#endif
    if(manpaths.count() < 1)
        manpaths = settings.value("manpaths").toStringList();

    if(manpaths.count() < 1) {
        QDir dir;
#ifdef Q_OS_WIN
        dir = "c:\\Tools\\man";
        if(dir.exists())
            manpaths << dir.path();
#else
        manpaths = QString(getenv("MANPATH")).split(":", QString::SkipEmptyParts);
        if(manpaths.count() < 1) {
            manpaths << "/usr/share/man";
            dir.setPath("/usr/local/share/man");
            if(dir.exists())
                manpaths << dir.path();
            dir.setPath("/opt/local/man");
            if(dir.exists())
                manpaths << dir.path();
        }
#endif
    }

    settings.beginGroup("Sections");
    ui.actionSection1->setChecked(settings.value("1", ui.actionSection1->isChecked()).toBool());
    ui.actionSection2->setChecked(settings.value("2", ui.actionSection2->isChecked()).toBool());
    ui.actionSection3->setChecked(settings.value("3", ui.actionSection3->isChecked()).toBool());
    ui.actionSection4->setChecked(settings.value("4", ui.actionSection4->isChecked()).toBool());
    ui.actionSection5->setChecked(settings.value("5", ui.actionSection5->isChecked()).toBool());
    ui.actionSection6->setChecked(settings.value("6", ui.actionSection6->isChecked()).toBool());
    ui.actionSection7->setChecked(settings.value("7", ui.actionSection7->isChecked()).toBool());
    ui.actionSection8->setChecked(settings.value("8", ui.actionSection8->isChecked()).toBool());
    ui.actionSectionl->setChecked(settings.value("l", ui.actionSectionl->isChecked()).toBool());
    ui.actionSectionn->setChecked(settings.value("n", ui.actionSectionn->isChecked()).toBool());
    settings.endGroup();

    resize(settings.value("size", QSize(760, 540)).toSize());
    sync();

    // app menus

#ifdef Q_OS_MAC
    auto appBar = new QMenuBar(this);
    auto appMenu = appBar->addMenu("Manpager");

    auto appQuit = new QAction(tr("&Quit"), appMenu);
    appQuit->setMenuRole(QAction::QuitRole);

    auto appAbout = new QAction(tr("&About"), appMenu);
    appAbout->setMenuRole(QAction::AboutRole);
    appAbout->setEnabled(true);

    auto appConfig = new QAction(tr("&Settings"), appMenu);
    appConfig->setMenuRole(QAction::PreferencesRole);
    appConfig->setEnabled(true);

    connect(appQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(appConfig, &QAction::triggered, this, &Main::changeSettings);
    connect(appAbout,  &QAction::triggered, this, &Main::about);

    appMenu->addAction(appAbout);
    appMenu->addAction(appConfig);
    appMenu->addAction(appQuit);
    appBar->show();
#endif

    // initialize connections and states

    connect(ui.actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui.actionAbout, &QAction::triggered, this, &Main::about);
    connect(ui.actionOptions, &QAction::triggered, this, &Main::changeSettings);
    connect(ui.actionReload, &QAction::triggered, this, &Main::reloadIndex);

    connect(ui.indexView, &QAbstractItemView::activated, this, &Main::openAt);
    connect(ui.actionView, &QAction::triggered, this, &Main::openViewer);
    connect(ui.actionOpen, &QAction::triggered, this, &Main::openDocument);
    connect(ui.actionClear, &QAction::triggered, this, &Main::clearTabs);
    connect(ui.tabs, &QTabWidget::tabCloseRequested, this, &Main::closeTab);
    connect(ui.tabs, &QTabWidget::currentChanged, this, &Main::changeTab);

//    connect(ui.indexView, &QTableView::customContextMenuRequested, this, &Main::openContext);
    connect(this, &QMainWindow::customContextMenuRequested, this, &Main::showContextMenu);
    connect(this, &Main::resized, this, &Main::changeColumns, Qt::QueuedConnection);
    connect(this, &Main::updated, this, &Main::reloadIndex, Qt::QueuedConnection);

    show();
    emit updated();
}

Main::~Main()
{
    sync();
}

void Main::applyOptions(QStringList& list, QCheckBox *sections[])
{
    if(sections) {
        for(unsigned pos = 0; pos < 10; ++pos)
            manSections[pos]->setChecked(sections[pos]->isChecked());
    }

    manpaths = list;
    sync();
    emit updated();
}

void Main::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    emit resized();
}

void Main::sync()
{
#ifndef PROJECT_TESTDATA
    settings.beginGroup("Sections");
    settings.setValue("1", ui.actionSection1->isChecked());
    settings.setValue("2", ui.actionSection2->isChecked());
    settings.setValue("3", ui.actionSection3->isChecked());
    settings.setValue("4", ui.actionSection4->isChecked());
    settings.setValue("5", ui.actionSection5->isChecked());
    settings.setValue("6", ui.actionSection6->isChecked());
    settings.setValue("7", ui.actionSection7->isChecked());
    settings.setValue("8", ui.actionSection8->isChecked());
    settings.setValue("l", ui.actionSectionl->isChecked());
    settings.setValue("n", ui.actionSectionn->isChecked());
    settings.endGroup();

    qDebug() << "SAVED PATH" << manpaths;

    settings.setValue("size", size());
    settings.setValue("manpaths", manpaths);
    settings.sync();
#endif
}

void Main::status(const QString& text)
{
    if(night)
        ui.statusbar->setStyleSheet("color: gray");
    else
        ui.statusbar->setStyleSheet("color: black");

    ui.statusbar->showMessage(text);
    ui.statusbar->update();
    ui.statusbar->repaint();
    ui.statusbar->update();
}

void Main::error(const QString& text)
{
    ui.statusbar->setStyleSheet("color: red");
    ui.statusbar->showMessage(text);
    ui.statusbar->update();
    ui.statusbar->repaint();
}

void Main::openContext(const QPoint& pos)
{
    QMenu m;
    m.addAction(ui.actionView);
    m.addAction(ui.actionOpen);
    m.exec(ui.indexView->mapToGlobal(pos));
}

void Main::showContextMenu(const QPoint& pos)
{
    QMenu m;
    m.addAction(ui.actionAbout);
    m.addAction(ui.actionOptions);

    m.addSeparator();
    QMenu *sections = m.addMenu(tr("Sections"));
    sections->addAction(ui.actionSection1);
    sections->addAction(ui.actionSection2);
    sections->addAction(ui.actionSection3);
    sections->addAction(ui.actionSection4);
    sections->addAction(ui.actionSection5);
    sections->addAction(ui.actionSection6);
    sections->addAction(ui.actionSection7);
    sections->addAction(ui.actionSection8);
    sections->addAction(ui.actionSectionl);
    sections->addAction(ui.actionSectionn);

    m.addSeparator();
    m.addAction(ui.actionClear);
    m.addAction(ui.actionReload);
    m.addAction(ui.actionQuit);
    m.exec(mapToGlobal(pos));
}

void Main::about()
{
    About info(this);
    info.exec();
}

void Main::enableFind()
{
    if(currentView)
        currentView->activateFind();
}

void Main::searchText()
{
    if(currentView)
        currentView->findNextText();
}

void Main::reloadIndex()
{
    toolbar->disableSearch();
    status(tr("loading..."));

    ui.indexView->setModel(nullptr);
    ui.tabs->setCurrentIndex(0);

    if(index) {
        delete index;
        index = nullptr;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    setEnabled(false);
    auto ind = new Index(manSections, paths());
    connect(ind, &Index::updateStatus, this, &Main::status);
    connect(ind, &Index::updateIndex, this, &Main::updateIndex);

    ind->start();
}

void Main::updateIndex(Index *ind)
{
    index = ind;
    ui.indexView->setModel(ind);
    ui.indexView->setEnabled(true);
    ui.indexView->selectRow(0);
    toolbar->enableSearch();
    setEnabled(true);
    status(tr("ready"));
    connect(ind, &Index::selected, toolbar, &Toolbar::selectedIndex);
    QApplication::restoreOverrideCursor();
    emit resized();
}

void Main::changeColumns()
{
    int size = ui.indexView->width() - ui.indexView->columnWidth(1) - 1;
    ui.indexView->setColumnWidth(0, size);
    ui.indexView->horizontalHeader()->resizeSection(0, size);
    ui.indexView->update();
}

void Main::changeSettings()
{
    if(!options)
        options = new Options(ui.tabs, manSections);

    ui.tabs->setCurrentIndex(options->tabIndex());
    ui.tabs->setTabsClosable(true);
}

void Main::searchIndex(const QString& entry)
{
    int pos;
    bool select = false;
    QString text = entry;

    int ext = entry.lastIndexOf('.');

    if(ext > 2)
        text = entry.left(ext);

    pos = index->find(text);

    ui.indexView->setModel(nullptr);

    if(text.length() < 1) {
        status(tr("ready"));
        index->selectAt(0, text);
        select = true;
    }
    else if(pos >= 0) {
        status(tr("searching ") + QChar('\"') + text + QChar('\"'));
        index->selectAt(pos, text);
        select = true;
    }
    else {
        error(tr("not found ") + QChar('\"') + text + QChar('\"'));
    }

    ui.indexView->setModel(index);
    changeColumns();

    if(select)
        ui.indexView->selectRow(0);
}

void Main::openDocument()
{
    auto pos = ui.indexView->currentIndex();
    auto name = index->nameAt(pos.row());
    auto item = index->itemAt(pos.row());
    auto path = paths()[item.path] + QDir::separator() + "man" + item.id + QDir::separator() + name;

    if(item.compressed == Index::GZIP)
        path += ".gz";

    status(tr("opening ") + name);
    QUrl url = QUrl::fromLocalFile(path);

    if(QDesktopServices::openUrl(url)) {
        status(tr("ready"));
        return;
    }

    error(tr("failed to open ") + name);
}

void Main::openViewer()
{
    openAt(ui.indexView->currentIndex());
}

void Main::openNamed(const QString& text)
{
    int ext = text.lastIndexOf('.');

    if(ext < 2) {
        error(tr("specify section in input"));
        return;
    }

    int pos = index->search(text);

    if (pos < 0) {
        error(tr("not found ") + QChar('\"') + text + QChar('\"'));
        return;
    }

    openTab(pos);
}

void Main::openAt(const QModelIndex& cursor)
{
    openTab(cursor.row());
}

void Main::openTab(int row)
{
    auto name = index->nameAt(row);
    auto item = index->itemAt(row);
    auto path = paths()[item.path] + QDir::separator() + "man" + item.id + QDir::separator() + name;

    for(int tab = 1; tab < ui.tabs->count(); ++tab) {
        if(ui.tabs->tabText(tab) == name) {
            ui.tabs->setCurrentIndex(tab);
            return;
        }
    }

    status(tr("loading ") + name);

    if(item.compressed == Index::GZIP) {
        path += ".gz";
        QString cmd = "gunzip";
        QStringList args;
        args << "-c" << path;
        QProcess gunzip(this);
        gunzip.start(cmd, args);
        gunzip.setReadChannel(QProcess::StandardOutput);
        if(!gunzip.waitForStarted()) {
            error(tr("failed to load ") + name);
            return;
        }
        gunzip.waitForReadyRead();
        new Viewer(ui.tabs, gunzip, name);
        gunzip.waitForFinished();
    }
    else {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            error(tr("failed to load ") + name);
            return;
        }
        new Viewer(ui.tabs, file, name);
        file.close();
    }

    status(tr("loaded ") + name);
}

void Main::clearTabs()
{
    closeTab(0);
}

void Main::changeTab(int tab)
{
    auto priorView = currentView;

    qDebug() << "CHANGED " << tab;
    if(options && options->tabIndex() == tab)
        statusbar->disableSettings();
    else
        statusbar->enableSettings();

    if(tab == 0 || (options && options->tabIndex() == tab)) {
        statusbar->disableSearch();
        currentView = nullptr;
    }
    else {
        statusbar->enableSearch();
        currentView = static_cast<Viewer *>(ui.tabs->widget(tab));
    }
    if(priorView && currentView != priorView)
        priorView->release();
}

void Main::closeCurrent()
{
    closeTab(ui.tabs->currentIndex());
}

void Main::closeTab(int tab)
{
    // close of index tab actually closes all other open manpages
    if(tab == 0) {
        int count = ui.tabs->count();
        for(tab = 1; tab < count; ++tab)
            closeTab(1);
        toolbar->enableSearch();
        if(options) {
            options->deleteLater();
            options = nullptr;
        }
        return;
    }

    if(options && options->tabIndex() == tab) {
        ui.tabs->removeTab(tab);
        options->deleteLater();
        options = nullptr;
        statusbar->enableSettings();
    }
    else {
        auto view = static_cast<Viewer *>(ui.tabs->widget(tab));
        if(!view)
            return;

        if(view == currentView)
            currentView = nullptr;

        ui.tabs->removeTab(tab);
        view->deleteLater();
    }

    if(ui.tabs->count() < 2)
        ui.tabs->setTabsClosable(false);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);
    QCoreApplication::setOrganizationName("tychosoft.com");
    QCoreApplication::setOrganizationDomain("tychosoft.com");
    QCoreApplication::setApplicationName("Manpager");
    QTranslator localize;

    QApplication app(argc, argv);
    QCommandLineParser args;
    args.setApplicationDescription("System Manual Page Viewer");
    Args::add(args, {
        {Args::HelpArgument},
        {Args::VersionArgument},
        {{"night"}, "Set dark color icon scheme"},
        {{"reset"}, "Reset Config"},
    });

    if(localize.load("manpager_" + QLocale::system().name(), ":/i18n"))
        QApplication::installTranslator(&localize);

    args.process(app);

    if(args.isSet("reset"))
        settings.clear();

    auto color = qApp->palette().color(QPalette::Base);
    auto sum = color.blue() + color.red() + color.green();

    if(sum > 312 && !args.isSet("night") && settings.value("scheme", "light") != "night") {
        Q_CLEANUP_RESOURCE(night);
        Q_INIT_RESOURCE(light);
    }
    else {
        Q_CLEANUP_RESOURCE(light);
        Q_INIT_RESOURCE(night);
        night = true;
    }

// delayed resource loading until we have color scheme...
#if defined(Q_OS_MAC)
    QFile style(":/styles/macos.qss");
#elif defined(Q_OS_WINDOWS)
    QFile style(":/styles/windows.qss");
#else
    QFile style(":/styles/desktop.qss");
#endif

    if(style.exists()) {
        style.open(QFile::ReadOnly);
        QString qss = QLatin1String(style.readAll());
        style.close();
        qApp->setStyleSheet(qss);
    }

    Main w;
    w.show();
    return QApplication::exec();
}


