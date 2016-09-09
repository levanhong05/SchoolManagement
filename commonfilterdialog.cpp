#include "commonfilterdialog.h"

#include <QRegExp>

#include <QLabel>
#include <QFrame>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>

CommonFilterDialog::CommonFilterDialog(QVector<CONST::HDG> headings, QWidget *parent) :
    QDialog(parent), _headingList(headings)
{
    setWindowTitle(tr("Student Table Filter"));

    QVBoxLayout *layout = new QVBoxLayout();
    this->_stackedLayout = new QStackedLayout;

    // header
    // Column selector
    this->_filterColumn = new QComboBox;

    for (CONST::HDG column : this->_headingList) {
        this->_filterColumn->addItem(CONST::getHeadingString(column), static_cast<int>(column));
    }

    connect(this->_filterColumn, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &CommonFilterDialog::handleFilterColumnChange);

    this->_filterColumn->setCurrentIndex(0);

    QFormLayout *headerLayout = new QFormLayout;
    headerLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    headerLayout->addRow(tr("Filter by &column:"), this->_filterColumn);
    layout->addLayout(headerLayout);

    // horizonal line
    QFrame *hline = new QFrame;
    hline->setFrameShape(QFrame::HLine);
    hline->setLineWidth(2);
    layout->addWidget(hline);

    // Frame 0: string filter
    QVBoxLayout *stringLayout = new QVBoxLayout;
    this->_filterLineEdit = new QLineEdit(this);
    connect(this->_filterLineEdit, &QLineEdit::textChanged, this, &CommonFilterDialog::filterTextChanged);
    this->_filterLineEdit->setPlaceholderText(tr("what are you looking for ?"));

    stringLayout->addWidget(this->_filterLineEdit);

    // Frame 0 - options layout
    QGroupBox *filterTextOptionBox = new QGroupBox(tr("Option"));
    QVBoxLayout *filterTextOptionLayout = new QVBoxLayout;
    this->_caseSensitivity = new QCheckBox(tr("Case &sensitive"));
    connect(this->_caseSensitivity, &QCheckBox::toggled, this, &CommonFilterDialog::caseSensitivityChanged);

    this->_caseSensitivity->setChecked(false);
    this->_useRegexp = new QCheckBox(tr("&Regexp(PCRE)"));
    connect(this->_useRegexp, &QCheckBox::toggled, this, &CommonFilterDialog::useRegExp);

    this->_useRegexp->setChecked(false);

    filterTextOptionLayout->addWidget(this->_caseSensitivity);
    filterTextOptionLayout->addWidget(this->_useRegexp);
    filterTextOptionBox->setLayout(filterTextOptionLayout);
    stringLayout->addWidget(filterTextOptionBox);

    QWidget *widget = new QWidget;
    widget->setLayout(stringLayout);
    this->_stackedLayout->addWidget(widget);

    // Frame 1: Birthday filter
    QFormLayout *dateRangeLayout = new QFormLayout;
    this->_fromDate = new QDateEdit();
    this->_fromDate->setDisplayFormat("yyyy-MM-dd");
    this->_fromDate->setCalendarPopup(true);

    connect(this->_fromDate, &QDateEdit::dateChanged, this, &CommonFilterDialog::fromBirthdayChanged);
    dateRangeLayout->addRow(tr("&From: "), this->_fromDate);

    this->_toDate = new QDateEdit();
    this->_toDate->setDisplayFormat("yyyy-MM-dd");
    this->_toDate->setCalendarPopup(true);

    connect(this->_toDate, &QDateEdit::dateChanged, this, &CommonFilterDialog::toBirthdayChanged);
    dateRangeLayout->addRow(tr("&To: "), this->_toDate);

    // emit initial dateChanged signal
    this->_toDate->setDate(QDate::currentDate());
    this->_fromDate->setDate(this->_toDate->date().addYears(-15));

    // set max date range: 1880.01.01 to Today
    this->_fromDate->setMinimumDate(QDate(1880, 1, 1));
    this->_fromDate->setMaximumDate(this->_toDate->date());

    this->_toDate->setMinimumDate(this->_fromDate->date());
    this->_toDate->setMaximumDate(QDate::currentDate());

    // confine user always select meaningful date range, i.e. fromdate always <= todate
    connect(this->_fromDate, &QDateEdit::dateChanged, [this](const QDate & date) {
        if (date != this->_toDate->minimumDate()) {
            this->_toDate->setMinimumDate(date);
        }
    });

    connect(this->_toDate, &QDateEdit::dateChanged, [this](const QDate & date) {
        if (date != this->_fromDate->maximumDate()) {
            this->_fromDate->setMaximumDate(date);
        }
    });

    widget = new QWidget;
    widget->setLayout(dateRangeLayout);
    this->_stackedLayout->addWidget(widget);

    // Frame 2: Sex filter
    this->_sexSelector = new QListWidget;
    this->_sexSelector->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    QListWidgetItem *item = new QListWidgetItem(Person::getSexString(Person::Sex::Male),
            this->_sexSelector, static_cast<quint8>(Person::Sex::Male));

    this->_sexSelector->addItem(item);

    item = new QListWidgetItem(Person::getSexString(Person::Sex::Female),
                               this->_sexSelector, static_cast<quint8>(Person::Sex::Female));

    this->_sexSelector->addItem(item);

    item = new QListWidgetItem(Person::getSexString(Person::Sex::Other),
                               this->_sexSelector, static_cast<quint8>(Person::Sex::Other));

    this->_sexSelector->addItem(item);

    item = new QListWidgetItem(Person::getSexString(Person::Sex::Unspecified),
                               this->_sexSelector, static_cast<quint8>(Person::Sex::Unspecified));

    this->_sexSelector->addItem(item);

    connect(this->_sexSelector, &QListWidget::itemActivated, [this](QListWidgetItem * item) {
        emit sexTypeChanged(static_cast<Person::Sex>(item->type()));
    });

    this->_sexSelector->setSelectionMode(QAbstractItemView::SingleSelection);
    this->_sexSelector->setCurrentRow(0);

    QVBoxLayout *sexLayout = new QVBoxLayout;
    sexLayout->addWidget(this->_sexSelector);
    sexLayout->addWidget(new QLabel(tr("<b>Double click to sort</b>")));
    widget = new QWidget;
    widget->setLayout(sexLayout);

    this->_stackedLayout->addWidget(widget);
    layout->addLayout(this->_stackedLayout);

    // button
    QPushButton *okButton = new QPushButton(tr("&Close"));
    okButton->setDefault(true);

    connect(okButton, &QPushButton::clicked, this, &CommonFilterDialog::done);
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    setLayout(layout);
}


void CommonFilterDialog::handleFilterColumnChange(int idx)
{
    CONST::HDG index = static_cast<CONST::HDG>(this->_filterColumn->itemData(idx).toInt());
    emit filterColumnChanged(index);

    switch (index) {
        case CONST::HDG::ID:
        case CONST::HDG::IDNUMBER:
        case CONST::HDG::CLASSNO:
        case CONST::HDG::DEPT:
        case CONST::HDG::MAJOR:
        case CONST::HDG::NAME:
        case CONST::HDG::POSITION:
        case CONST::HDG::TUTORID:
            this->_stackedLayout->setCurrentIndex(0);
            break;

        case CONST::HDG::BIRTHDAY:
            this->_stackedLayout->setCurrentIndex(1);
            break;

        case CONST::HDG::SEX:
            this->_stackedLayout->setCurrentIndex(2);
            break;

        case CONST::HDG::COUNT:
            break;
    }
}
