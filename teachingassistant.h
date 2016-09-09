#pragma once

#include <QObject>

#include "teacher.h"
#include "postgraduate.h"

class TeachingAssistant : public Teacher, public Postgraduate
{
public:
    TeachingAssistant(const QString classNo = QString(),
                      const QString _name = QString(),
                      const QString _id = QString(),
                      const QString _idNum = QString("000000000000000000"),
                      const Sex _sex = Sex::Unspecified,
                      const QDate _birthDay = QDate(1880, 1, 1),
                      const QString major = QString(),
                      const QString _tutorId = QString(),
                      const QString _dept = QString(),
                      const QString _position = QString());

    virtual ~TeachingAssistant();

    friend QDataStream &operator <<(QDataStream &out, const TeachingAssistant &ta);
    friend QDataStream &operator >>(QDataStream &in, TeachingAssistant &ta);

    virtual QString toString() const override;
    /* Parameter: file is an opend file, don't close it here.
     * currently, always return true */
    static bool writeToFile(QFile &file, const QList<TeachingAssistant> list);

    /* Parameter: file is an opend file, don't close it here */
    static QList<TeachingAssistant> readFromFile(QFile &file, QString &error);

protected:
    virtual QDataStream &writeBinary(QDataStream &out) const override;
    virtual QDataStream &readBinary(QDataStream &in) override;
};

