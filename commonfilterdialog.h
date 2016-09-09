#pragma once

#include <QDate>
#include <QDialog>
#include <QVector>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedLayout>

#include "const.h"
#include "person.h"

class CommonFilterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CommonFilterDialog(QVector<CONST::HDG> headings, QWidget *parent = 0);

signals:
    void filterColumnChanged(const CONST::HDG &column);
    void filterTextChanged(const QString &newText);
    void fromBirthdayChanged(const QDate &newDate);
    void toBirthdayChanged(const QDate &newDate);
    void sexTypeChanged(const Person::Sex newSex);
    void caseSensitivityChanged(bool sensitive);
    void useRegExp(bool yes);

protected slots:
    void handleFilterColumnChange(int idx);

private:
    QLineEdit *_filterLineEdit;
    QComboBox *_filterColumn;
    QVector<CONST::HDG> _headingList;

    QCheckBox *_caseSensitivity;
    QCheckBox *_useRegexp;

    QStackedLayout *_stackedLayout;
    QListWidget *_sexSelector;

    QDateEdit *_fromDate;
    QDateEdit *_toDate;
};
