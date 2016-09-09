#include "teachermodel.h"

#if QT_VERSION >= 0x050200
    #include <string>
#endif

TeacherModel::TeacherModel(QVector<CONST::HDG> indexMap,  QObject *parent) :
    QAbstractTableModel(parent), _indexMap(indexMap)
{
}

TeacherModel::~TeacherModel()
{
}

int TeacherModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->_list.size();
}

int TeacherModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->_indexMap.size();
}

QVariant TeacherModel::data(const QModelIndex &index, int role) const
{
    //* NOTE: if used across days, Today string will be inaccurate.
    static QString birthTooltip = "Valid date range: 1880-01-01 to Today(" + QDate::currentDate().toString("yyyy-MM-dd") + ")";

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    } else if ((role == Qt::DisplayRole || role == Qt::EditRole) && !this->_list.isEmpty()) {
        const Teacher &t = _list.at(index.row());

        switch (this->_indexMap.at(index.column())) {
            case CONST::HDG::ID:
                return t.getId();

            case CONST::HDG::NAME:
                return t.getName();

            case CONST::HDG::SEX:
                if (role == Qt::DisplayRole) {
                    return Person::getSexString(t.getSex());
                }

                return static_cast<quint8>(t.getSex());

            case CONST::HDG::BIRTHDAY:
                if (role == Qt::DisplayRole) {
                    return t.getBirthday().toString("yyyy-MM-dd");
                }

                return t.getBirthday();

            case CONST::HDG::IDNUMBER:
                return t.getIdNumber();

            case CONST::HDG::POSITION:
                return t.getPosition();

            case CONST::HDG::DEPT:
                return t.getDepartment();

            default:
                return QVariant();
        }
    } else if (role == Qt::ToolTipRole) {
        switch (this->_indexMap.at(index.column())) {
            case CONST::HDG::BIRTHDAY:
                return birthTooltip;

            case CONST::HDG::IDNUMBER:
                return "18 digits or 17 digits with letter X";

            default:
                return QVariant();
        }
    }

    return QVariant();
}

bool TeacherModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        bool result = false;
        Teacher &t = this->_list[index.row()];

        switch (this->_indexMap.at(index.column())) {
            case CONST::HDG::ID:
                result = t.setId(value.toString());
                break;

            case CONST::HDG::NAME:
                result = t.setName(value.toString());
                break;

            case CONST::HDG::SEX:
                t.setSex(static_cast<Person::Sex>(value.toUInt()));
                result = true;
                break;

            case CONST::HDG::BIRTHDAY:
                result = t.setBirthday(value.toDate());
                break;

            case CONST::HDG::IDNUMBER:
                result = t.setIdNumber(value.toString());
                break;

            case CONST::HDG::POSITION:
                result = t.setPosition(value.toString());
                break;

            case CONST::HDG::DEPT:
                result = t.setDepartment(value.toString());
                break;

            default:
                break;
        }

        if (result) {
            emit dataChanged(index, index);
        }

        return result;
    }

    return QAbstractTableModel::setData(index, value, role);
}

Qt::ItemFlags TeacherModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

bool TeacherModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // NOTE: [row, row+count-1], not [row, row+count)
    beginInsertRows(parent, row, row + count - 1);

    for (int i = row; i < row + count; ++i) {
        this->_list.insert(i, Teacher());
    }

    endInsertRows();
    return true;
}

// remove from high index to low index
bool TeacherModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // NOTE: [row, row+count-1], not [row, row+count)
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = row + count - 1; i >= row; i--) {
        this->_list.removeAt(i);
    }

    endRemoveRows();
    return true;
}

QVariant TeacherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Vertical) {
        return section;
    } else {
        return CONST::getHeadingString(this->_indexMap.at(section));
    }
}

void TeacherModel::setDataList(QList<Teacher> list)
{
    beginResetModel();
    this->_list = list;
    endResetModel();
}

QList<Teacher> TeacherModel::getDataList()
{
    return this->_list;
}

QVector<CONST::HDG> TeacherModel::getHeaderIndexs() const
{
    return this->_indexMap;
}

void TeacherModel::sort(int column, Qt::SortOrder order)
{
    // std::function is about two times slower than lambda expression
    // std::function<bool(Teacher, Teacher)> func;
    beginResetModel();

    switch (this->_indexMap[column]) {
        case CONST::HDG::ID:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getId() < b.getId(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getId() >= b.getId(); });
            }

            break;

        case CONST::HDG::NAME:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [this](Teacher a, Teacher b) -> bool {
#if QT_VERSION >= 0x050200
                    return this->_vi_VN_collate.compare(a.getName(), b.getName()) < 0;
#else
                    std::string sa = a.getName().toStdString();
                    std::string sb = b.getName().toStdString();
                    const char *pa = sa.data();
                    const char *pb = sb.data();
                    return this->_vi_VN_collate.compare(pa, pa + sa.size(), pb, pb + sb.size()) < 0;
#endif
                });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [this](Teacher a, Teacher b) -> bool {
#if QT_VERSION >= 0x050200
                    return this->_vi_VN_collate.compare(a.getName(), b.getName()) >= 0;
#else
                    std::string sa = a.getName().toStdString();
                    std::string sb = b.getName().toStdString();
                    const char *pa = sa.data();
                    const char *pb = sb.data();
                    return this->_vi_VN_collate.compare(pa, pa + sa.size(), pb, pb + sb.size()) >= 0;
#endif
                });
            }

            break;

        case CONST::HDG::SEX:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getSex() < b.getSex(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getSex() >= b.getSex(); });
            }

            break;

        case CONST::HDG::BIRTHDAY:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getBirthday() < b.getBirthday(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getBirthday() >= b.getBirthday(); });
            }

            break;

        case CONST::HDG::IDNUMBER:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getIdNumber() < b.getIdNumber(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getIdNumber() >= b.getIdNumber(); });
            }

            break;

        case CONST::HDG::POSITION:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getPosition() < b.getPosition(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getPosition() >= b.getPosition(); });
            }

            break;

        case CONST::HDG::DEPT:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getDepartment() < b.getDepartment(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Teacher a, Teacher b) -> bool {return a.getDepartment() >= b.getDepartment(); });
            }

            break;

        default:
            break;
    }

    endResetModel();
}

void TeacherModel::filter()
{
    if (! this->_filterEnabled) {
        return;
    }

    this->_isFiltering = true;
    QList<Teacher> newList;

    switch (this->_filterColumn) {
        case CONST::HDG::ID: {
            if (this->_useRegexp) {
                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getId().contains(this->_filterRegexp)) {
                        newList.append(teacher);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getId().contains(this->_filterString, sensitivity)) {
                        newList.append(teacher);
                    }
                }
            }
        }
        break;

        case CONST::HDG::NAME: {
            if (this->_useRegexp) {
                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getName().contains(this->_filterRegexp)) {
                        newList.append(teacher);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getName().contains(this->_filterString, sensitivity)) {
                        newList.append(teacher);
                    }
                }
            }
        }
        break;

        case CONST::HDG::SEX: {
            for (Teacher &teacher : this->_listOriginal) {
                if (teacher.getSex() == this->_filterSex) {
                    newList.append(teacher);
                }
            }
        }
        break;

        case CONST::HDG::BIRTHDAY: {
            for (Teacher &teacher : this->_listOriginal) {
                if (this->_minDate <= teacher.getBirthday() && teacher.getBirthday() <= this->_maxDate) {
                    newList.append(teacher);
                }
            }
        }
        break;

        case CONST::HDG::IDNUMBER: {
            if (this->_useRegexp) {
                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getIdNumber().contains(this->_filterRegexp)) {
                        newList.append(teacher);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getIdNumber().contains(this->_filterString, sensitivity)) {
                        newList.append(teacher);
                    }
                }
            }
        }
        break;

        case CONST::HDG::POSITION: {
            if (this->_useRegexp) {
                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getPosition().contains(this->_filterRegexp)) {
                        newList.append(teacher);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getPosition().contains(this->_filterString, sensitivity)) {
                        newList.append(teacher);
                    }
                }
            }
        }
        break;

        case CONST::HDG::DEPT: {
            if (this->_useRegexp) {
                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getDepartment().contains(this->_filterRegexp)) {
                        newList.append(teacher);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Teacher &teacher : this->_listOriginal) {
                    if (teacher.getDepartment().contains(this->_filterString, sensitivity)) {
                        newList.append(teacher);
                    }
                }
            }
        }
        break;

        default:
            return;
    }

    beginResetModel();
    this->_list = newList;
    endResetModel();
    this->_isFiltering = false;
}

void TeacherModel::setRegexp(QString pattern)
{
    this->_filterRegexp.setPattern(pattern);

    if (this->_caseSensitive) {
        this->_filterRegexp.setPatternOptions(QRegularExpression::DontCaptureOption);
    } else {
        this->_filterRegexp.setPatternOptions(QRegularExpression::DontCaptureOption | QRegularExpression::CaseInsensitiveOption);
    }
}
