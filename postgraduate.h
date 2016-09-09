#pragma once
#include "student.h"
#include "teacher.h"

class Postgraduate : public Student
{
//    Q_OBJECT
public:
    static const int POSTGRADUATE_MAJOR_LEN = 20;

    Postgraduate(const QString classNo = QString(),
                 const QString _name = QString(),
                 const QString _id = QString(),
                 const QString _idNum = QString("000000000000000000"),
                 const Sex _sex = Sex::Unspecified,
                 const QDate _birthDay = QDate(1880, 1, 1),
                 const QString major = QString(),
                 const QString _tutorId = QString());

    virtual ~Postgraduate();

    friend QDataStream &operator <<(QDataStream &out, const Postgraduate &pg);
    friend QDataStream &operator >>(QDataStream &in, Postgraduate &pg);

    /* Parameter: file is an opend file, don't close it here.
     * currently, always return true */
    static bool writeToFile(QFile &file, const QList<Postgraduate> list);

    /* Parameter: file is an opend file, don't close it here */
    static QList<Postgraduate> readFromFile(QFile &file, QString &error);
    virtual QString toString() const override;

public:
    QString getMajor() const;
    bool setMajor(QString major);

    QString getTutorId() const;
    bool setTutorId(QString _id);

protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;

protected:
    QString _major;
    QString _tutorId;
};

inline QString Postgraduate::getMajor() const
{
    return this->_major;
}

inline bool Postgraduate::setMajor(QString major)
{
    if (major.length() > POSTGRADUATE_MAJOR_LEN) {
        this->_errorString = tr("major name %1 too long, current length: %2, max length: %3")
                             .arg(major).arg(major.length()).arg(POSTGRADUATE_MAJOR_LEN);
        return false;
    }

    this->_major = major;
    return true;
}

inline QString Postgraduate::getTutorId() const
{
    return this->_tutorId;
}

inline bool Postgraduate::setTutorId(QString id)
{
    if (id.length() > PERSON_ID_LEN) {
        this->_errorString = tr("tutor's id %1 too long, current length %2, max length %3")
                             .arg(_tutorId).arg(_tutorId.length()).arg(PERSON_ID_LEN);
        return false;
    }

    this->_tutorId = id;
    return true;
}
