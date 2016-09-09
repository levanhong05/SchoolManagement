#pragma once

#include <QDate>
#include <QList>
#include <QRegExp>
#include <QString>
#include <QDataStream>
#include <QCoreApplication>
#include <QRegExpValidator>

#include <cstdint>

#include "const.h"

class Person
{
    Q_DECLARE_TR_FUNCTIONS(Person)
public:
    static const int PERSON_NAME_LEN = 10;
    static const int PERSON_ID_LEN = 6;
    static const int PERSON_ID_NUM_LEN = 18;
    static const int PERSON_SEX_LEN = 2;
    static const QRegExpValidator *idNumberValidator;

    enum class Sex : quint8 {
        Male = 0,
        Female,
        Other,
        Unspecified
    };

    Person(const QString name = QString(),
           const QString id = QString(),
           const QString idNum = QString("000000000000000000"),
           const Sex sex = Sex::Unspecified,
           const QDate birthDay = QDate(1880, 1, 1));

    virtual ~Person();

    friend QDataStream &operator <<(QDataStream &out, const Person &person);
    friend QDataStream &operator >>(QDataStream &in, Person &person);

public:
    static QString getSexString(Person::Sex sex);
    static const QRegExpValidator *obtainIDNumberValidator();

    virtual QString toString() const;

    QString errorString() const;

    QString getName() const;
    bool setName(QString name);

    QString getId() const;
    bool setId(QString id);

    QString getIdNumber() const;
    bool setIdNumber(QString idNum);

    Sex getSex() const;
    void setSex(Sex sex);

    QDate getBirthday() const;
    bool setBirthday(QDate birthday);

protected:
    virtual QDataStream &writeBinary(QDataStream &out) const;
    virtual QDataStream &readBinary(QDataStream &in);

protected:
    QString _errorString;
    QString _name;
    QString _id;
    QString _idNum;
    Sex _sex;
    /* birtday valid Range: QDate(1880, 1, 1) <= birthDay_ <= QDate::currentDay(); */
    QDate _birthDay;
};

inline QString Person::errorString() const
{
    return this->_errorString;
}
inline QString Person::getName() const
{
    return this->_name;
}

inline bool Person::setName(QString name)
{
    if (name.length() > PERSON_NAME_LEN) {
        this->_errorString = tr("name \"%1\" too long, length: %2, max length: %3").arg(name).arg(name.length()).arg(PERSON_NAME_LEN);
        return false;
    }

    this->_name = name;
    return true;
}

inline QString Person::getId() const
{
    return this->_id;
}

inline bool Person::setId(QString id)
{
    if (id.length() > PERSON_ID_LEN) {
        this->_errorString = tr("id \"%1\" too long, length: %2, max length: %3")
                             .arg(id).arg(id.length()).arg(PERSON_ID_LEN);
        return false;
    }

    this->_id = id;
    return true;
}

inline QString Person::getIdNumber() const
{
    return this->_idNum;
}

inline bool Person::setIdNumber(QString idNum)
{
    int pos = 0;

    if (Person::idNumberValidator->validate(idNum, pos) != QValidator::Acceptable) {
        this->_errorString = tr("ID number format error, should be 18 digitis or "
                                "17 digits and letter x/X, id number: %1") .arg(idNum);
        return false;
    }

    this->_idNum = idNum;
    return true;
}

inline Person::Sex Person::getSex() const
{
    return this->_sex;
}

inline void Person::setSex(Sex sex)
{
    this->_sex = sex;
}

inline QDate Person::getBirthday() const
{
    return this->_birthDay;
}

inline bool Person::setBirthday(QDate birthday)
{
    if (birthday.isValid() && birthday <= QDate::currentDate() && birthday >= QDate(1880, 1, 1)) {
        this->_birthDay = birthday;
        return true;
    } else {
        this->_errorString = tr("date is not valid or out of range, Date is: %1").arg(birthday.toString("yyyy-MM-dd"));
        return false;
    }
}
