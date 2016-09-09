#pragma once
#include "person.h"

#include <QFile>

class Student : virtual public Person
{
//    Q_OBJECT
public:
    static const int STUDENT_CLASSNO_LEN = 6;

    Student(const QString classNo = QString(),
            const QString _name = QString(),
            const QString _id = QString(),
            const QString _idNum = QString("000000000000000000"),
            const Sex _sex = Sex::Unspecified,
            const QDate _birthDay = QDate(1880, 1, 1));

    virtual ~Student();
    friend QDataStream &operator <<(QDataStream &out, const Student &student);
    friend QDataStream &operator >>(QDataStream &in, Student &student);

public:
    /* Parameter: file is an opend file, don't close it here.
     * currently, always return true */
    static bool writeToFile(QFile &file, const QList<Student> list);

    /* Parameter: file is an opend file, don't close it here */
    static QList<Student> readFromFile(QFile &file, QString &error);

public:
    virtual QString toString() const override;
    QString getClassNo() const;
    bool setClassNo(QString classNo);

protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;

protected:
    QString classNo;
};

inline QString Student::getClassNo() const
{
    return this->classNo;
}

inline bool Student::setClassNo(QString classNo)
{
    if (classNo.length() > STUDENT_CLASSNO_LEN) {
        this->_errorString = tr("Class Number \"%1\" too long, length: %2, max length: %3")
                             .arg(classNo).arg(classNo.length()).arg(STUDENT_CLASSNO_LEN);
        return false;
    }

    this->classNo = classNo;
    return true;
}
