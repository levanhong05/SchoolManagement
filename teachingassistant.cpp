#include "teachingassistant.h"

TeachingAssistant::TeachingAssistant(
    const QString classNo /*= QString()*/,
    const QString name /*= QString()*/,
    const QString id /*= QString()*/,
    const QString idNum /*= QString("000000000000000000")*/,
    const Sex sex /*= QString()*/,
    const QDate birthDay /*= QDate(1880,1,1)*/,
    const QString major /*= QString()*/,
    const QString tutorId /*= QString()*/,
    const QString dept /*= QString()*/,
    const QString position /*= QString()*/
) : Person(name, id, idNum, sex, birthDay),
    Teacher(name, id, idNum, sex, birthDay, dept, position),
    Postgraduate(classNo, name, id, idNum, sex, birthDay, major, tutorId)
{
}

TeachingAssistant::~TeachingAssistant()
{
}

QDataStream &operator <<(QDataStream &out, const TeachingAssistant &ta)
{
    return ta.writeBinary(out);
}

QDataStream &operator >>(QDataStream &in, TeachingAssistant &ta)
{
    return ta.readBinary(in);
}

QString TeachingAssistant::toString() const
{
    return tr("%1, Department: %2, Position: %3").arg(Postgraduate::toString()).arg(_dept).arg(_position);
}

QDataStream &TeachingAssistant::writeBinary(QDataStream &out) const
{
    return Postgraduate::writeBinary(out) << this->_dept << this->_position;
}

QDataStream &TeachingAssistant::readBinary(QDataStream &in)
{
    return Postgraduate::readBinary(in) >> this->_dept >> this->_position;
}

QList<TeachingAssistant> TeachingAssistant::readFromFile(QFile &file, QString &error)
{
    QList<TeachingAssistant> list;
    QDataStream in(&file);
    quint32 magicNumber;
    in >> magicNumber;

    if (magicNumber != CONST::MAGIC_NUMBER) {
        error = tr("Wrong file format, not a School Personnel Management data file");
        return list;
    }

    quint32 filetype;
    in >> filetype;

    if (filetype != CONST::FILE_TYPE_TEACHING_ASSISTANT) {
        error = tr("Wrong file type, not a TeachingAssistant data file (extension: %1)").arg(CONST::FILE_EXTENSION_TA);
        return list;
    }

    quint32 version;
    in >> version;

    if (version != CONST::VERSION_1_20131109) {
        error = tr("unknow TeachingAssistant data file version: %1").arg(version);
        return list;
    }

    in.setVersion(QDataStream::Qt_4_2);
    quint64 size;
    in >> size;
    list.reserve(size);
    TeachingAssistant stu;

    for (quint64 i = 0; i < size; ++i) {
        in >> stu;
        list.append(stu);
    }

    return list;
}

bool TeachingAssistant::writeToFile(QFile &file, const QList<TeachingAssistant> list)
{
    QDataStream out(&file);
    out << CONST::MAGIC_NUMBER << CONST::FILE_TYPE_TEACHING_ASSISTANT << CONST::VERSION_1_20131109;
    out.setVersion(QDataStream::Qt_4_2);
    out << static_cast<quint64>(list.size());

    for (TeachingAssistant item : list) {
        out << item;
    }

    return true;
}
