#include "postgraduate.h"

Postgraduate::Postgraduate(const QString classNo /*= QString()*/,
                           const QString name /*= QString()*/,
                           const QString id /*= QString()*/,
                           const QString idNum /*= QString("000000000000000000")*/,
                           const Sex sex /*= QString()*/,
                           const QDate birthDay /*= QDate(1880,1,1)*/,
                           const QString major /*= QString()*/,
                           const QString tutorId /*= QString()*/) :
    Person(name, id, idNum, sex, birthDay),
    Student(classNo, name, id, idNum, sex, birthDay)
{
    if (!setMajor(major)) {
        setMajor(tr("none"));
    }

    if (!setTutorId(tutorId)) {
        setTutorId(tr("none"));
    }
}

Postgraduate::~Postgraduate()
{
}

QDataStream &operator <<(QDataStream &out, const Postgraduate &pg)
{
    return pg.writeBinary(out);
}

QDataStream &operator >> (QDataStream &in, Postgraduate &pg)
{
    return pg.readBinary(in);
}

QDataStream &Postgraduate::writeBinary(QDataStream &out) const
{
    return Student::writeBinary(out) << this->_major << this->_tutorId;
}

QDataStream &Postgraduate::readBinary(QDataStream &in)
{
    return Student::readBinary(in) >> this->_major >> this->_tutorId;
}

QString Postgraduate::toString() const
{
    return tr("%1, Major: %2, Tutor's id: %3").arg(Student::toString()).arg(_major).arg(_tutorId);
}

QList<Postgraduate> Postgraduate::readFromFile(QFile &file, QString &error)
{
    QList<Postgraduate> list;
    QDataStream in(&file);
    quint32 magicNumber;
    in >> magicNumber;

    if (magicNumber != CONST::MAGIC_NUMBER) {
        error = tr("Wrong file format, not a School Personnel Management data file");
        return list;
    }

    quint32 filetype;
    in >> filetype;

    if (filetype != CONST::FILE_TYPE_POSTGRADUATE) {
        error = tr("Wrong file type, not a Postgraduate data file (extension: %1)").arg(CONST::FILE_EXTENSION_POSTGRADUATE);
        return list;
    }

    quint32 version;
    in >> version;

    if (version != CONST::VERSION_1_20131109) {
        error = tr("unknow Postgraduate data file version: %1").arg(version);
        return list;
    }

    in.setVersion(QDataStream::Qt_4_2);
    quint64 size;
    in >> size;
    list.reserve(size);
    Postgraduate stu;

    for (quint64 i = 0; i < size; ++i) {
        in >> stu;
        list.append(stu);
    }

    return list;
}

bool Postgraduate::writeToFile(QFile &file, const QList<Postgraduate> list)
{
    QDataStream out(&file);
    out << CONST::MAGIC_NUMBER << CONST::FILE_TYPE_POSTGRADUATE << CONST::VERSION_1_20131109;
    out.setVersion(QDataStream::Qt_4_2);
    out << static_cast<quint64>(list.size());

    for (Postgraduate item : list) {
        out << item;
    }

    return true;
}
