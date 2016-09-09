#include "commontabledelegate.h"

#include <QApplication>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRegExp>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "teachingassistant.h"
#include "teacher.h"

CommonTableDelegate::CommonTableDelegate(QVector<CONST::HDG> indexMap, QObject *parent /*= 0*/) :
    QStyledItemDelegate(parent), _indexMap(indexMap)
{
}

QWidget *CommonTableDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    switch(this->_indexMap.at(index.column())){
    case CONST::HDG::ID : {
        QLineEdit * idEditor = new QLineEdit(parent);
        idEditor->setMaxLength(Person::PERSON_ID_LEN);
        return idEditor;
    }
    case CONST::HDG::NAME : {
        QLineEdit * nameEditor = new QLineEdit(parent);
        nameEditor->setMaxLength(Person::PERSON_NAME_LEN);
        return nameEditor;
    }
    case CONST::HDG::SEX : {
        QComboBox *sexEditor = new QComboBox(parent);
        sexEditor->addItem(Person::getSexString(Person::Sex::Male),
                           static_cast<quint8>(Person::Sex::Male));
        sexEditor->addItem(Person::getSexString(Person::Sex::Female),
                           static_cast<quint8>(Person::Sex::Female));
        sexEditor->addItem(Person::getSexString(Person::Sex::Unspecified),
                           static_cast<quint8>(Person::Sex::Unspecified));
        sexEditor->addItem(Person::getSexString(Person::Sex::Other),
                           static_cast<quint8>(Person::Sex::Other));
        return sexEditor;
    }
    case CONST::HDG::BIRTHDAY : {
        QDateEdit *birthdayEditor = new QDateEdit(parent);
        birthdayEditor->setDateRange(QDate(1880, 1, 1), QDate::currentDate());
        birthdayEditor->setDisplayFormat("yyyy-MM-dd");
        birthdayEditor->setCalendarPopup(true);
        birthdayEditor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        return birthdayEditor;
    }
    case CONST::HDG::CLASSNO : {
        QLineEdit * idEditor = new QLineEdit(parent);
        idEditor->setMaxLength(Student::STUDENT_CLASSNO_LEN);
        return idEditor;
    }
    case CONST::HDG::IDNUMBER : {
        QLineEdit * idEditor = new QLineEdit(parent);
        idEditor->setMaxLength(Person::PERSON_ID_NUM_LEN);
        idEditor->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{17}[0-9xX]"), parent));
        //        idEditor->setInputMask("999999-99999999-999>H;_");
        return idEditor;
    }
    case CONST::HDG::MAJOR: {
        QLineEdit *majorEditor = new QLineEdit(parent);
        majorEditor->setMaxLength(Postgraduate::POSTGRADUATE_MAJOR_LEN);
        return majorEditor;
    }
    case CONST::HDG::TUTORID:{
        QLineEdit * idEditor = new QLineEdit(parent);
        idEditor->setMaxLength(Person::PERSON_ID_LEN);
        return idEditor;
    }
    case CONST::HDG::DEPT:{
        QLineEdit *deptEditor = new QLineEdit(parent);
        deptEditor->setMaxLength(Teacher::TEACHER_DEPT_LEN);
        return deptEditor;
    }
    case CONST::HDG::POSITION: {
        QLineEdit *positionEditor = new QLineEdit(parent);
        positionEditor->setMaxLength(Teacher::TEACHER_POSITION_LEN);
        return positionEditor;
    }
    default:
        return QStyledItemDelegate::createEditor(parent,option, index);
    }
}

void CommonTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    switch(this->_indexMap.at(index.column())){
    case CONST::HDG::SEX : {
        QComboBox *sexEditor = qobject_cast<QComboBox*>(editor);
        int idx = sexEditor->findData(index.model()->data(index, Qt::EditRole).toUInt());
        sexEditor->setCurrentIndex(idx);
        break;
    }

    case CONST::HDG::BIRTHDAY: {
        QDateEdit *birthdayEditor = qobject_cast<QDateEdit*>(editor);
        if(birthdayEditor){
            birthdayEditor->setDate(index.model()->data(index, Qt::EditRole).toDate());
        } else {
            birthdayEditor->setDate(QDate());
        }
        break;
    }

    default:
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void CommonTableDelegate::setModelData( QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    switch(this->_indexMap.at(index.column())){
    case CONST::HDG::SEX : {
        QComboBox *sexEditor = qobject_cast<QComboBox*>(editor);
        model->setData(index, sexEditor->itemData(sexEditor->currentIndex(), Qt::UserRole));
        break;
    }
    case CONST::HDG::BIRTHDAY : {
        QDateEdit *birthdayEditor = qobject_cast<QDateEdit*>(editor);
        model->setData(index, birthdayEditor->date());
        break;
    }
    default:
//        QStyledItemDelegate::setModelData(editor, model, index);
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
        model->setData(index, lineEdit->text());
    }
}

void CommonTableDelegate::updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}
