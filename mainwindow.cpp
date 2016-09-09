#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>

#include <QFile>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QKeySequence>

#include <QFileDialog>
#include <QMessageBox>

#include <QModelIndexList>
#include <QItemSelectionModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _tabs(new QTabWidget(this))
    //filterDialog(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(tr("Personnel Management System"));
    initUI();
    addMenuBarToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    for (int i = 0; i < TAB_NUMBER; ++i) {
        this->_tableView[i] = new QTableView(this);
        this->_tableView[i]->setSortingEnabled(true);
        this->_tableView[i]->setSelectionMode(QAbstractItemView::ExtendedSelection);
        this->_tableView[i]->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

    initStudentTab();
    initTeacherTab();
    initPostgraduateTab();
    initTeachingAssistantTab();

    this->_tabs->addTab(this->_tableView[MainWindow::TAB::STUDENT], tr("&Student"));
    this->_tabs->addTab(this->_tableView[MainWindow::TAB::TEACHER], tr("&Teacher"));
    this->_tabs->addTab(this->_tableView[MainWindow::TAB::POSTGRAD], tr("&Postgraduate"));
    this->_tabs->addTab(this->_tableView[MainWindow::TAB::TA], tr("Teaching &Assistant"));

    setCentralWidget(this->_tabs);
}

void MainWindow::initStudentTab()
{
    const int index = static_cast<int>(MainWindow::TAB::STUDENT);

    QVector<CONST::HDG> headerIndexMap(6);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::CLASSNO;
    headerIndexMap[3] = CONST::HDG::SEX;
    headerIndexMap[4] = CONST::HDG::BIRTHDAY;
    headerIndexMap[5] = CONST::HDG::IDNUMBER;

    this->_studentTableModel = new StudentModel(headerIndexMap, this);
    this->_tableView[index]->setModel(this->_studentTableModel);
    this->_tableView[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->_tableView[index]));
    this->_tableView[index]->resizeColumnsToContents();
    this->_tableView[index]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::initTeacherTab()
{
    const int index = static_cast<int>(MainWindow::TAB::TEACHER);

    QVector<CONST::HDG> headerIndexMap(7);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::SEX;
    headerIndexMap[3] = CONST::HDG::BIRTHDAY;
    headerIndexMap[4] = CONST::HDG::DEPT;
    headerIndexMap[5] = CONST::HDG::POSITION;
    headerIndexMap[6] = CONST::HDG::IDNUMBER;

    this->_teacherTableModel = new TeacherModel(headerIndexMap, this);
    this->_tableView[index]->setModel(this->_teacherTableModel);
    this->_tableView[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->_tableView[index]));
    this->_tableView[index]->resizeColumnsToContents();
    this->_tableView[index]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::initPostgraduateTab()
{
    const int index = static_cast<int>(MainWindow::TAB::POSTGRAD);

    QVector<CONST::HDG> headerIndexMap(8);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::SEX;
    headerIndexMap[3] = CONST::HDG::CLASSNO;
    headerIndexMap[4] = CONST::HDG::MAJOR;
    headerIndexMap[5] = CONST::HDG::TUTORID;
    headerIndexMap[6] = CONST::HDG::BIRTHDAY;
    headerIndexMap[7] = CONST::HDG::IDNUMBER;

    this->_postgradTableModel = new PostgraduateModel(headerIndexMap, this);
    this->_tableView[index]->setModel(this->_postgradTableModel);
    this->_tableView[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->_tableView[index]));
    this->_tableView[index]->resizeColumnsToContents();
    this->_tableView[index]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::initTeachingAssistantTab()
{
    const int index = static_cast<int>(MainWindow::TAB::TA);

    QVector<CONST::HDG> headerIndexMap(10);
    headerIndexMap[0] = CONST::HDG::ID;
    headerIndexMap[1] = CONST::HDG::NAME;
    headerIndexMap[2] = CONST::HDG::SEX;
    headerIndexMap[3] = CONST::HDG::CLASSNO;
    headerIndexMap[4] = CONST::HDG::MAJOR;
    headerIndexMap[5] = CONST::HDG::TUTORID;
    headerIndexMap[6] = CONST::HDG::DEPT;
    headerIndexMap[7] = CONST::HDG::POSITION;
    headerIndexMap[8] = CONST::HDG::BIRTHDAY;
    headerIndexMap[9] = CONST::HDG::IDNUMBER;

    this->_taTableModel = new TeachingAssistantModel(headerIndexMap, this);
    this->_tableView[index]->setModel(this->_taTableModel);
    this->_tableView[index]->setItemDelegate(new CommonTableDelegate(headerIndexMap, this->_tableView[index]));
    this->_tableView[index]->resizeColumnsToContents();
    this->_tableView[index]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::addMenuBarToolBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = new QAction(QIcon(":/icon/asserts/icon/Open.png"), tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);

    QAction *saveAction = new QAction(QIcon(":/icon/asserts/icon/Save.png"), tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();

    QAction *quitAction = new QAction(QIcon(":/icon/asserts/icon/Quit.png"), tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, &qApp->quit);
    fileMenu->addAction(quitAction);

    // Edit menu
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction *addRowAction = new QAction(QIcon(":/icon/asserts/icon/Plus.png"), tr("&Add row"), this);
    addRowAction->setShortcut(Qt::CTRL + Qt::Key_Y);
    connect(addRowAction, &QAction::triggered, this, &MainWindow::addNewRow);
    editMenu->addAction(addRowAction);

    QAction *insertRowAction = new QAction(QIcon(":/icon/asserts/icon/Break.png"), tr("&Insert row before"), this);
    insertRowAction->setToolTip(tr("Insert a new row before selected row"));
    insertRowAction->setShortcut(Qt::CTRL + Qt::Key_I);
    connect(insertRowAction, &QAction::triggered, this, &MainWindow::insertRowBefore);
    editMenu->addAction(insertRowAction);

    QAction *deleteRowsAction = new QAction(QIcon(":/icon/asserts/icon/Remove.png"), tr("&Delete selected row(s)"), this);
    deleteRowsAction->setShortcut(QKeySequence::Delete);
    connect(deleteRowsAction, &QAction::triggered, this, &MainWindow::deleteRows);
    editMenu->addAction(deleteRowsAction);

    QAction *filterAction = new QAction(QIcon(":/icon/asserts/icon/Search.png"), tr("&Find && Filter"), this);
    filterAction->setIconText(tr("Find"));
    filterAction->setShortcut(QKeySequence::Find);
    connect(filterAction, &QAction::triggered, this, &MainWindow::openFilterDialog);
    editMenu->addAction(filterAction);
    editMenu->addSeparator();

    QAction *clearDataAction = new QAction(QIcon(":/icon/asserts/icon/Clear.png"), tr("&Clear data"), this);
    connect(clearDataAction, &QAction::triggered, this, &MainWindow::clearAllData);
    editMenu->addAction(clearDataAction);

    ui->mainToolBar->addAction(openAction);
    ui->mainToolBar->addAction(saveAction);
    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction(addRowAction);
    ui->mainToolBar->addAction(insertRowAction);
    ui->mainToolBar->addAction(deleteRowsAction);
    ui->mainToolBar->addAction(clearDataAction);
    ui->mainToolBar->addAction(filterAction);
    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addAction(quitAction);

    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
}

bool MainWindow::openFile()
{
    int index = this->_tabs->currentIndex();
    bool succeed = true;

    QString fileFormatString;
    QString ext;

    switch (static_cast<MainWindow::TAB>(index)) {
        case MainWindow::TAB::STUDENT:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Student").arg(CONST::FILE_EXTENSION_STUDENT);
            ext = CONST::FILE_EXTENSION_STUDENT;
            break;

        case MainWindow::TAB::TEACHER:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teacher").arg(CONST::FILE_EXTENSION_TEACHER);
            ext = CONST::FILE_EXTENSION_TEACHER;
            break;

        case MainWindow::TAB::POSTGRAD:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Postgraduate").arg(CONST::FILE_EXTENSION_POSTGRADUATE);
            ext = CONST::FILE_EXTENSION_POSTGRADUATE;
            break;

        case MainWindow::TAB::TA:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teaching Assistant").arg(CONST::FILE_EXTENSION_TA);
            ext = CONST::FILE_EXTENSION_TA;
            break;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), ".", fileFormatString);

    if (fileName.isEmpty()) {
        return false;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Open File"),
                             tr("Can't open file: \n\n%1\n\n%2").arg(fileName).arg(file.errorString()));
        return false;
    }

    QString error = "";

    switch (static_cast<MainWindow::TAB>(index)) {
        case MainWindow::TAB::STUDENT: {  // student table
            succeed = openStudentFile(file, error);
            break;
        }

        case MainWindow::TAB::TEACHER: {
            succeed = openTeacherFile(file, error);
            break;
        }

        case MainWindow::TAB::POSTGRAD: {
            succeed = openPostgraduateFile(file, error);
            break;
        }

        case MainWindow::TAB::TA: {
            succeed = openTeachingAssistantFile(file, error);
            break;
        }
    }

    file.close();

    if (succeed) {
        this->_tableView[index]->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, tr("Error"), error);
    }

    return succeed;
}

bool MainWindow::saveFile()
{
    bool succeed = true;
    bool addedExtension = false;
    int index = this->_tabs->currentIndex();
    QString fileFormatString;
    QString ext;

    switch (static_cast<MainWindow::TAB>(index)) {
        case MainWindow::TAB::STUDENT:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Student").arg(CONST::FILE_EXTENSION_STUDENT);
            ext = CONST::FILE_EXTENSION_STUDENT;
            break;

        case MainWindow::TAB::TEACHER:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teacher").arg(CONST::FILE_EXTENSION_TEACHER);
            ext = CONST::FILE_EXTENSION_TEACHER;
            break;

        case MainWindow::TAB::POSTGRAD:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Postgraduate").arg(CONST::FILE_EXTENSION_POSTGRADUATE);
            ext = CONST::FILE_EXTENSION_POSTGRADUATE;
            break;

        case MainWindow::TAB::TA:
            fileFormatString = tr("Personnel %1 Data File(*.%2);;All files(*)").arg("Teaching Assistant").arg(CONST::FILE_EXTENSION_TA);
            ext = CONST::FILE_EXTENSION_TA;
            break;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), ".", fileFormatString);

    if (fileName.isEmpty()) {
        return false;
    }

    // add extension if necessary
    if (!fileName.endsWith("." + ext)) {
        fileName +=  "." + ext;
    }

    QFile file(fileName);

    if (addedExtension && file.exists()) {
        if (QMessageBox::Yes != QMessageBox::question(this, tr("File exists"),
                tr("The file\n\n%1\n\nalready exist, replace it ?")
                .arg(fileName))) {
            return false;
        }
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Save File"),
                             tr("Can't open file: \n\n%1\n\n%2").arg(fileName).arg(file.errorString()));
        return false;
    }

    QString error = "";

    switch (static_cast<MainWindow::TAB>(this->_tabs->currentIndex())) {
        case MainWindow::TAB::STUDENT: {  // student table
            succeed = saveStudentFile(file, error);
            break;
        }

        case MainWindow::TAB::TEACHER: {
            succeed = saveTeacherFile(file, error);
            break;
        }

        case MainWindow::TAB::POSTGRAD: {
            succeed = savePostgraduateFile(file, error);
            break;
        }

        case MainWindow::TAB::TA: {
            succeed = saveTeachingAssistantFile(file, error);
            break;
        }
    }

    file.close();

    if (!succeed) {
        QMessageBox::warning(this, tr("Error"), error);
    }

    return succeed;
}

// file is an opened valid file, don't close it here, the caller should close it
bool MainWindow::openStudentFile(QFile &file, QString &error)
{
    QList<Student> list = Student::readFromFile(file, error);

    if (error.length() > 0) {
        error = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   %2").arg(file.fileName()).arg(error);
        return false;
    }

    this->_studentTableModel->setDataList(list);
    return true;
}

bool MainWindow::saveStudentFile(QFile &file, QString &error)
{
    if (!Student::writeToFile(file, this->_studentTableModel->getDataList())) {
        error = tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
        return false;
    }

    return true;
}

// file is an opened valid file, don't close it here, the caller should close it
bool MainWindow::openTeacherFile(QFile &file, QString &error)
{
    QList<Teacher> list = Teacher::readFromFile(file, error);

    if (error.length() > 0) {
        error = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   %2").arg(file.fileName()).arg(error);
        return false;
    }

    this->_teacherTableModel->setDataList(list);
    return true;
}

bool MainWindow::saveTeacherFile(QFile &file, QString &error)
{
    if (!Teacher::writeToFile(file, this->_teacherTableModel->getDataList())) {
        error = tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
        return false;
    }

    return true;
}

// file is an opened valid file, don't close it here, the caller should close it
bool MainWindow::openPostgraduateFile(QFile &file, QString &error)
{
    QList<Postgraduate> list = Postgraduate::readFromFile(file, error);

    if (error.length() > 0) {
        error = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   %2").arg(file.fileName()).arg(error);
        return false;
    }

    this->_postgradTableModel->setDataList(list);
    return true;
}

bool MainWindow::savePostgraduateFile(QFile &file, QString &error)
{
    if (!Postgraduate::writeToFile(file, this->_postgradTableModel->getDataList())) {
        error = tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
        return false;
    }

    return true;
}

// file is an opened valid file, don't close it here, the caller should close it
bool MainWindow::openTeachingAssistantFile(QFile &file, QString &error)
{
    QList<TeachingAssistant> list = TeachingAssistant::readFromFile(file, error);

    if (error.length() > 0) {
        error = tr("Error occurred while open following file:\n\n%1\n\nreason:\n   %2").arg(file.fileName()).arg(error);
        return false;
    }

    this->_taTableModel->setDataList(list);
    return true;
}

bool MainWindow::saveTeachingAssistantFile(QFile &file, QString &error)
{
    if (!TeachingAssistant::writeToFile(file, this->_taTableModel->getDataList())) {
        error = tr("Error occurred while saving to the file:\n\n%1\n\nReason:\n  Unknown").arg(file.fileName());
        return false;
    }

    return true;
}

void MainWindow::clearAllData()
{
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(this->_tabs->currentIndex());

    switch (tab) {
        case MainWindow::TAB::STUDENT:
            if (this->_studentTableModel->rowCount() <= 0) {
                return;
            }

            break;

        case MainWindow::TAB::TEACHER:
            if (this->_teacherTableModel->rowCount() <= 0) {
                return;
            }

            break;

        case MainWindow::TAB::POSTGRAD:
            if (this->_postgradTableModel->rowCount() <= 0) {
                return;
            }

            break;

        case MainWindow::TAB::TA:
            if (this->_taTableModel->rowCount() <= 0) {
                return;
            }

            break;
    }

    if (QMessageBox::Yes != QMessageBox::question(this,
                                  tr("Confirm deletion"),
                                  tr("Are you sure to clear <b>all data</b> in this Table, this <b>can't be undo</b>"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No)) {
        return;
    }

    switch (tab) {
        case MainWindow::TAB::STUDENT:
            this->_studentTableModel->setDataList(QList<Student>());
            break;

        case MainWindow::TAB::TEACHER:
            this->_teacherTableModel->setDataList(QList<Teacher>());
            break;

        case MainWindow::TAB::POSTGRAD:
            this->_postgradTableModel->setDataList(QList<Postgraduate>());
            break;

        case MainWindow::TAB::TA:
            this->_taTableModel->setDataList(QList<TeachingAssistant>());
            break;
    }
}

void MainWindow::addNewRow()
{
    int tabIndex = this->_tabs->currentIndex();

    switch (static_cast<MainWindow::TAB>(tabIndex)) {
        case MainWindow::TAB::STUDENT:
            this->_studentTableModel->insertRows(this->_studentTableModel->rowCount(), 1);
            break;

        case MainWindow::TAB::TEACHER:
            this->_teacherTableModel->insertRows(this->_teacherTableModel->rowCount(), 1);
            break;

        case MainWindow::TAB::POSTGRAD:
            this->_postgradTableModel->insertRows(this->_postgradTableModel->rowCount(), 1);
            break;

        case MainWindow::TAB::TA:
            this->_taTableModel->insertRows(this->_taTableModel->rowCount(), 1);
            break;
    }

    this->_tableView[tabIndex]->resizeColumnsToContents();
    this->_tableView[tabIndex]->scrollToBottom();
}

void MainWindow::insertRowBefore()
{
    int tabIndex = this->_tabs->currentIndex();
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(tabIndex);
    QItemSelectionModel *selectionModel = this->_tableView[tabIndex]->selectionModel();

    if (!selectionModel->hasSelection()) {
        int rowCount = 0;

        switch (tab) {
            case MainWindow::TAB::STUDENT:
                rowCount = this->_studentTableModel->rowCount();
                break;

            case MainWindow::TAB::TEACHER:
                rowCount = this->_teacherTableModel->rowCount();
                break;

            case MainWindow::TAB::POSTGRAD:
                rowCount = this->_postgradTableModel->rowCount();
                break;

            case MainWindow::TAB::TA:
                rowCount = this->_taTableModel->rowCount();
                break;
        }

        if (rowCount > 0) {
            QMessageBox::warning(this, tr("No row selected"),
                                 tr("Please select a row before which new row will be inserted"));
        } else {
            addNewRow();
        }

        return;
    }

    if (selectionModel->selectedRows().size() != 1) {
        QMessageBox::warning(this, tr("Multiple rows selected"), tr("Please select only one row."));
        return;
    }

    switch (tab) {
        case MainWindow::TAB::STUDENT:
            this->_studentTableModel->insertRows(selectionModel->selectedRows().at(0).row(), 1);
            break;

        case MainWindow::TAB::TEACHER:
            this->_teacherTableModel->insertRows(selectionModel->selectedRows().at(0).row(), 1);
            break;

        case MainWindow::TAB::POSTGRAD:
            this->_postgradTableModel->insertRows(selectionModel->selectedRows().at(0).row(), 1);
            break;

        case MainWindow::TAB::TA:
            this->_taTableModel->insertRows(selectionModel->selectedRows().at(0).row(), 1);
            break;
    }

    this->_tableView[tabIndex]->resizeColumnsToContents();
}

void MainWindow::deleteRows()
{
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(this->_tabs->currentIndex());
    QItemSelectionModel *selectionModel = this->_tableView[this->_tabs->currentIndex()]->selectionModel();

    if (!selectionModel->hasSelection()) {
        QMessageBox::warning(this, tr("No row selected"), tr("Please select at least one row"));
        return;
    }

    QModelIndexList indexList = selectionModel->selectedRows();
    // sort selected row from high index to low index, and delete them in the same order
    qSort(indexList.begin(), indexList.end(), [](QModelIndex & left, QModelIndex & right) {
        return left.row() >= right.row();
    });

    // detect consecutive selections
    QList<QPair<int, int>> pairs = QList<QPair<int, int>>();
    int start = indexList.at(0).row();
    int current = start;

    for (int i = 0; i < indexList.size(); ++i) {
        int row = indexList.at(i).row();

        if (row - current <= 0 && row - current >= -1) {
            current = row;
        } else {
            pairs.append(QPair<int, int>(current, start - current + 1));
            start = row;
            current = start;
        }
    }

    pairs.append(QPair<int, int>(current, start - current + 1));

    switch (tab) {
        case MainWindow::TAB::STUDENT: {
            for (QPair<int, int> pair : pairs) {
                this->_studentTableModel->removeRows(pair.first, pair.second);
            }

            break;
        }

        case MainWindow::TAB::TEACHER: {
            for (QPair<int, int> pair : pairs) {
                this->_teacherTableModel->removeRows(pair.first, pair.second);
            }

            break;
        }

        case MainWindow::TAB::POSTGRAD:
            for (QPair<int, int> pair : pairs) {
                this->_postgradTableModel->removeRows(pair.first, pair.second);
            }

            break;

        case MainWindow::TAB::TA:
            for (QPair<int, int> pair : pairs) {
                this->_taTableModel->removeRows(pair.first, pair.second);
            }

            break;
    }
}

void MainWindow::openFilterDialog()
{
    QVector<CONST::HDG> headingIndex;
    MainWindow::TAB tab = static_cast<MainWindow::TAB>(this->_tabs->currentIndex());

    switch (tab) {
        case MainWindow::TAB::STUDENT: {
            headingIndex = this->_studentTableModel->getHeaderIndexs();
            // don't specify a parent, so that this dialog has its own icon in TaskBar area
            this->_filterDialog = new CommonFilterDialog(headingIndex);
            this->_filterDialog->setAttribute(Qt::WA_DeleteOnClose);

            connect(this->_filterDialog, &CommonFilterDialog::filterColumnChanged, this->_studentTableModel, &StudentModel::setFilterColumn);
            connect(this->_filterDialog, &CommonFilterDialog::filterTextChanged, this->_studentTableModel, &StudentModel::setFilterString);
            connect(this->_filterDialog, &CommonFilterDialog::useRegExp, this->_studentTableModel, &StudentModel::setFilterUseRegexp);
            connect(this->_filterDialog, &CommonFilterDialog::caseSensitivityChanged, this->_studentTableModel, &StudentModel::setFilterCaseSentivity);
            connect(this->_filterDialog, &CommonFilterDialog::sexTypeChanged, this->_studentTableModel, &StudentModel::setFilterSex);
            connect(this->_filterDialog, &CommonFilterDialog::fromBirthdayChanged, this->_studentTableModel, &StudentModel::setFilterMinDate);
            connect(this->_filterDialog, &CommonFilterDialog::toBirthdayChanged, this->_studentTableModel, &StudentModel::setFilterMaxDate);

            connect(this->_filterDialog, &CommonFilterDialog::finished, [this]() {
                this->_studentTableModel->setEnableFilter(false);
            });

            this->_studentTableModel->setEnableFilter(true);
            this->_studentTableModel->setFilterCaseSentivity(false);
            this->_studentTableModel->setFilterUseRegexp(false);
            this->_studentTableModel->setFilterColumn(CONST::HDG::ID);
            this->_studentTableModel->setFilterMinDate(QDate(1880, 1, 1));
            this->_studentTableModel->setFilterMaxDate(QDate::currentDate());
            this->_studentTableModel->setFilterSex(Person::Sex::Male);
            this->_studentTableModel->setFilterString("");
            break;
        }

        case MainWindow::TAB::TEACHER:
            headingIndex = this->_teacherTableModel->getHeaderIndexs();
            // don't specify a parent, so that this dialog has its own icon in TaskBar area
            this->_filterDialog = new CommonFilterDialog(headingIndex);
            this->_filterDialog->setAttribute(Qt::WA_DeleteOnClose);

            connect(this->_filterDialog, &CommonFilterDialog::filterColumnChanged, this->_teacherTableModel, &TeacherModel::setFilterColumn);
            connect(this->_filterDialog, &CommonFilterDialog::filterTextChanged, this->_teacherTableModel, &TeacherModel::setFilterString);
            connect(this->_filterDialog, &CommonFilterDialog::useRegExp, this->_teacherTableModel, &TeacherModel::setFilterUseRegexp);
            connect(this->_filterDialog, &CommonFilterDialog::caseSensitivityChanged, this->_teacherTableModel, &TeacherModel::setFilterCaseSentivity);
            connect(this->_filterDialog, &CommonFilterDialog::sexTypeChanged, this->_teacherTableModel, &TeacherModel::setFilterSex);
            connect(this->_filterDialog, &CommonFilterDialog::fromBirthdayChanged, this->_teacherTableModel, &TeacherModel::setFilterMinDate);
            connect(this->_filterDialog, &CommonFilterDialog::toBirthdayChanged, this->_teacherTableModel, &TeacherModel::setFilterMaxDate);

            connect(this->_filterDialog, &CommonFilterDialog::finished, [this]() {
                this->_teacherTableModel->setEnableFilter(false);
            });

            this->_teacherTableModel->setEnableFilter(true);
            this->_teacherTableModel->setFilterCaseSentivity(false);
            this->_teacherTableModel->setFilterUseRegexp(false);
            this->_teacherTableModel->setFilterColumn(CONST::HDG::ID);
            this->_teacherTableModel->setFilterMinDate(QDate(1880, 1, 1));
            this->_teacherTableModel->setFilterMaxDate(QDate::currentDate());
            this->_teacherTableModel->setFilterSex(Person::Sex::Male);
            this->_teacherTableModel->setFilterString("");
            break;

        case MainWindow::TAB::POSTGRAD: {
            headingIndex = this->_postgradTableModel->getHeaderIndexs();
            // don't specify a parent, so that this dialog has its own icon in TaskBar area
            this->_filterDialog = new CommonFilterDialog(headingIndex);
            this->_filterDialog->setAttribute(Qt::WA_DeleteOnClose);

            connect(this->_filterDialog, &CommonFilterDialog::filterColumnChanged, this->_postgradTableModel, &PostgraduateModel::setFilterColumn);
            connect(this->_filterDialog, &CommonFilterDialog::filterTextChanged, this->_postgradTableModel, &PostgraduateModel::setFilterString);
            connect(this->_filterDialog, &CommonFilterDialog::useRegExp, this->_postgradTableModel, &PostgraduateModel::setFilterUseRegexp);
            connect(this->_filterDialog, &CommonFilterDialog::caseSensitivityChanged, this->_postgradTableModel, &PostgraduateModel::setFilterCaseSentivity);
            connect(this->_filterDialog, &CommonFilterDialog::sexTypeChanged, this->_postgradTableModel, &PostgraduateModel::setFilterSex);
            connect(this->_filterDialog, &CommonFilterDialog::fromBirthdayChanged, this->_postgradTableModel, &PostgraduateModel::setFilterMinDate);
            connect(this->_filterDialog, &CommonFilterDialog::toBirthdayChanged, this->_postgradTableModel, &PostgraduateModel::setFilterMaxDate);

            connect(this->_filterDialog, &CommonFilterDialog::finished, [this]() {
                this->_postgradTableModel->setEnableFilter(false);
            });

            this->_postgradTableModel->setEnableFilter(true);
            this->_postgradTableModel->setFilterCaseSentivity(false);
            this->_postgradTableModel->setFilterUseRegexp(false);
            this->_postgradTableModel->setFilterColumn(CONST::HDG::ID);
            this->_postgradTableModel->setFilterMinDate(QDate(1880, 1, 1));
            this->_postgradTableModel->setFilterMaxDate(QDate::currentDate());
            this->_postgradTableModel->setFilterSex(Person::Sex::Male);
            this->_postgradTableModel->setFilterString("");
            break;
        }

        case MainWindow::TAB::TA: {
            headingIndex = this->_taTableModel->getHeaderIndexs();
            // don't specify a parent, so that this dialog has its own icon in TaskBar area
            this->_filterDialog = new CommonFilterDialog(headingIndex);
            this->_filterDialog->setAttribute(Qt::WA_DeleteOnClose);

            connect(this->_filterDialog, &CommonFilterDialog::filterColumnChanged, this->_taTableModel, &TeachingAssistantModel::setFilterColumn);
            connect(this->_filterDialog, &CommonFilterDialog::filterTextChanged, this->_taTableModel, &TeachingAssistantModel::setFilterString);
            connect(this->_filterDialog, &CommonFilterDialog::useRegExp, this->_taTableModel, &TeachingAssistantModel::setFilterUseRegexp);
            connect(this->_filterDialog, &CommonFilterDialog::caseSensitivityChanged, this->_taTableModel, &TeachingAssistantModel::setFilterCaseSentivity);
            connect(this->_filterDialog, &CommonFilterDialog::sexTypeChanged, this->_taTableModel, &TeachingAssistantModel::setFilterSex);
            connect(this->_filterDialog, &CommonFilterDialog::fromBirthdayChanged, this->_taTableModel, &TeachingAssistantModel::setFilterMinDate);
            connect(this->_filterDialog, &CommonFilterDialog::toBirthdayChanged, this->_taTableModel, &TeachingAssistantModel::setFilterMaxDate);

            connect(this->_filterDialog, &CommonFilterDialog::finished, [this]() {
                this->_taTableModel->setEnableFilter(false);
            });

            this->_taTableModel->setEnableFilter(true);
            this->_taTableModel->setFilterCaseSentivity(false);
            this->_taTableModel->setFilterUseRegexp(false);
            this->_taTableModel->setFilterColumn(CONST::HDG::ID);
            this->_taTableModel->setFilterMinDate(QDate(1880, 1, 1));
            this->_taTableModel->setFilterMaxDate(QDate::currentDate());
            this->_taTableModel->setFilterSex(Person::Sex::Male);
            this->_taTableModel->setFilterString("");
            break;
        }
    }

    // show as non-model dialog
    this->_editTrigger = this->_tableView[this->_tabs->currentIndex()]->editTriggers();
    this->_tableView[this->_tabs->currentIndex()]->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(this->_filterDialog, &CommonFilterDialog::finished, [this]() {
        this->_tableView[this->_tabs->currentIndex()]->setEditTriggers(this->_editTrigger);
    });

    this->_filterDialog->show();
}
