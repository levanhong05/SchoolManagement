#include "const.h"

QString CONST::getHeadingString(CONST::HDG headingIndex){
    switch (headingIndex) {
    case CONST::HDG::ID:
        return QObject::tr("ID"); case CONST::HDG::NAME: return QObject::tr("Name"); case CONST::HDG::SEX:
        return QObject::tr("Sex");
    case CONST::HDG::BIRTHDAY:
        return QObject::tr("Birthday");
    case CONST::HDG::CLASSNO:
        return QObject::tr("Class No.");
    case CONST::HDG::IDNUMBER:
        return QObject::tr("ID Number");
    case CONST::HDG::MAJOR:
        return QObject::tr("Major");
    case CONST::HDG::TUTORID:
        return QObject::tr("Tutor's ID");
    case CONST::HDG::DEPT:
        return QObject::tr("Department");
    case CONST::HDG::POSITION:
        return QObject::tr("Position");
    default:
        return QObject::tr("Error");
    }
}

