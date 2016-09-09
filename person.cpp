#include "person.h"

#include <QStringBuilder>
#include <type_traits>

Person::Person(const QString name /*= QString()*/,
               const QString id /*= QString()*/,
               const QString idNum/* = QString("000000000000000000")*/,
               const Sex sex /*= Sex::Unspecified*/,
               const QDate birthDay /*= QDate(1880,1,1)*/) : _sex(sex)
{
    if (!setName(name)) {
        setName("");
    }

    if (!setId(id)) {
        _id = tr("");
    }

    if (!setIdNumber(idNum)) {
        setIdNumber(QString("000000000000000000"));
    }

    if (!setBirthday(birthDay)) {
        _birthDay = QDate(1880, 1, 1);
    }
}

Person::~Person()
{
}

const QRegExpValidator *Person::idNumberValidator =  new QRegExpValidator(QRegExp("^\\d{17}[0-9xX]$") , nullptr);

QDataStream &Person::writeBinary(QDataStream &out) const
{
    out << this->_name << this->_id << this->_idNum
        << static_cast<std::underlying_type<Person::Sex>::type>(this->_sex) << this->_birthDay;

    return out;
}

QDataStream &Person::readBinary(QDataStream &in)
{
    std::underlying_type<Person::Sex>::type sex;
    in >> this->_name >> this->_id >> this->_idNum >> sex >> this->_birthDay;
    _sex = static_cast<Person::Sex>(sex);

    return in;
}

QDataStream &operator <<(QDataStream &out, const Person &person)
{
    return person.writeBinary(out);
}

QDataStream &operator >>(QDataStream &in, Person &person)
{
    return person.readBinary(in);
}

QString Person::toString() const
{
    return tr("Name: %1, Id: %2, Id number: %3, Sex: %4, Birthday: %5")
           .arg(_name).arg(_id).arg(_idNum).arg(getSexString(_sex)).arg(_birthDay.toString("yyyy-MM-dd"));
}

QString Person::getSexString(Sex sex)
{
    switch (sex) {
        case Person::Sex::Male:
            return tr("Male");

        case Person::Sex::Female:
            return tr("Female");

        case Person::Sex::Unspecified:
            return tr("Unspecified");

        default:
            return tr("Other");
    }
}

const QRegExpValidator *Person::obtainIDNumberValidator()
{
    return idNumberValidator;
}
