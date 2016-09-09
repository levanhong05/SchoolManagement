#pragma once

#include <QDialog>
#include <QLocale>
#include <QTableView>
#include <QTabWidget>
#include <QMainWindow>

#include "commontabledelegate.h"
#include "commonfilterdialog.h"

#include "studentmodel.h"
#include "teachermodel.h"
#include "postgraduatemodel.h"
#include "teachingassistantmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const int TAB_NUMBER = 4;

    enum TAB {
        STUDENT = 0,
        TEACHER = 1,
        POSTGRAD = 2,
        TA = 3
    };

public slots:
    bool openFile();
    bool saveFile();
    void clearAllData();
    void addNewRow();
    void insertRowBefore();
    void deleteRows();

    void openFilterDialog();

protected:
    bool openStudentFile(QFile &file, QString &error);
    bool saveStudentFile(QFile &file, QString &error);

    bool openTeacherFile(QFile &file, QString &error);
    bool saveTeacherFile(QFile &file, QString &error);

    bool openPostgraduateFile(QFile &file, QString &error);
    bool savePostgraduateFile(QFile &file, QString &error);

    bool openTeachingAssistantFile(QFile &file, QString &error);
    bool saveTeachingAssistantFile(QFile &file, QString &error);

private:
    void initUI();
    void initStudentTab();
    void initTeacherTab();
    void initPostgraduateTab();
    void initTeachingAssistantTab();

    void addMenuBarToolBar();

private:
    Ui::MainWindow *ui;

    QTabWidget *_tabs;

    QTableView *_tableView[TAB_NUMBER];

    // save old editTriggers when show filter dialog, and disable edit when
    // filtering, for changes will lost when close filter dialog
    // restore  editTriggers on dialog close
    QAbstractItemView::EditTriggers _editTrigger;

    StudentModel *_studentTableModel;
    TeacherModel *_teacherTableModel;
    PostgraduateModel *_postgradTableModel;
    TeachingAssistantModel *_taTableModel;

    // this dialog is not added to MainWindow as it's child, so that a icon will show in taskbar
    CommonFilterDialog *_filterDialog = nullptr;
};

