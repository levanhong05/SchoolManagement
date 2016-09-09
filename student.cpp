#include "student.h"

#include <QFile>

Student::Student(const QString classNo /*= QString()*/,
                 const QString name /*= QString()*/,
                 const QString id /*= QString()*/,
                 const QString idNum /*= QString("000000000000000000")*/,
                 const Sex sex /*= QString()*/,
                 const QDate birthDay /*= QDate(1880,1,1)*/):
    Person(name, id, idNum, sex, birthDay), classNo(classNo)
{
}

Student::~Student()
{
}

QDataStream &operator <<(QDataStream &out, const Student &student)
{
    return student.writeBinary(out);
}

QDataStream &operator >> (QDataStream &in, Student &student)
{
    return student.readBinary(in);
}

QDataStream &Student::writeBinary(QDataStream &out) const
{
    return Person::writeBinary(out) << this->classNo;
}

QDataStream &Student::readBinary(QDataStream &in)
{
    return Person::readBinary(in) >> this->classNo;
}

QString Student::toString() const
{
    return tr("%1, Class No.: %2").arg(Person::toString()).arg(classNo);
}

QList<Student> Student::readFromFile(QFile &file, QString &error)
{
    QList<Student> list;
    QDataStream in(&file);
    quint32 magicNumber;
    in >> magicNumber;

    if (magicNumber != CONST::MAGIC_NUMBER) {
        error = tr("Wrong file format, not a School Personnel Management data file");
        return list;
    }

    quint32 filetype;
    in >> filetype;

    if (filetype != CONST::FILE_TYPE_STUDENT) {
        error = tr("Wrong file type, not a Student data file (extension: %1)").arg(CONST::FILE_EXTENSION_STUDENT);
        return list;
    }

    quint32 version;
    in >> version;

    if (version != CONST::VERSION_1_20131109) {
        error = tr("unknow Student data file version: %1").arg(version);
        return list;
    }

    in.setVersion(QDataStream::Qt_4_2);
    quint64 size;
    in >> size;
    list.reserve(size);
    Student stu;

    for (quint64 i = 0; i < size; ++i) {
        in >> stu;
        list.append(stu);
    }

    return list;
}

bool Student::writeToFile(QFile &file, const QList<Student> list)
{
    QDataStream out(&file);
    out << CONST::MAGIC_NUMBER << CONST::FILE_TYPE_STUDENT << CONST::VERSION_1_20131109;
    out.setVersion(QDataStream::Qt_4_2);
    out << static_cast<quint64>(list.size());

    for (Student item : list) {
        out << item;
    }

    return true;
}
