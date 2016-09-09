#pragma once
#include "person.h"

#include <QFile>

class Teacher : virtual public Person
{
public:
    static const int TEACHER_DEPT_LEN = 20;
    static const int TEACHER_POSITION_LEN = 10;

    Teacher(const QString _name = QString(),
            const QString _id = QString(),
            const QString _idNum = QString("000000000000000000"),
            const Sex _sex = Sex::Unspecified,
            const QDate _birthDay = QDate(1880, 1, 1),
            const QString _dept = QString(),
            const QString _position = QString());
    virtual ~Teacher();

    friend QDataStream &operator <<(QDataStream &out, const Teacher &teacher);
    friend QDataStream &operator >>(QDataStream &in, Teacher &teacher);

public:
    /* Parameter: file is an opend file, don't close it here.
     * currently, always return true */
    static bool writeToFile(QFile &file, const QList<Teacher> list);

    /* Parameter: file is an opend file, don't close it here */
    static QList<Teacher> readFromFile(QFile &file, QString &error);

    virtual QString toString() const override;

protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;
public:
    QString getDepartment() const;
    bool setDepartment(QString dept);

    QString getPosition() const;
    bool setPosition(QString position);

protected:
    QString _dept;
    QString _position;
};


inline QString Teacher::getDepartment() const
{
    return this->_dept;
}

inline bool Teacher::setDepartment(QString dept)
{
    if (dept.length() > TEACHER_DEPT_LEN) {
        this->_errorString = tr("department name %1 too long, current length: %2, max length: %3")
                             .arg(dept).arg(dept.length()).arg(TEACHER_DEPT_LEN);
        return false;
    }

    this->_dept = dept;
    return true;
}

inline QString Teacher::getPosition() const
{
    return this->_position;
}

inline bool Teacher::setPosition(QString position)
{
    if (position.length() > TEACHER_POSITION_LEN) {
        this->_errorString = tr("position name %1 too long, current length %2, max length: %3")
                             .arg(position).arg(position.length()).arg(TEACHER_POSITION_LEN);
        return false;
    }

    this->_position = position;
    return true;
}
