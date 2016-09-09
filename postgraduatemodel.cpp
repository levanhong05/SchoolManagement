#include "postgraduatemodel.h"

#if QT_VERSION >= 0x050200
    #include <string>
#endif

PostgraduateModel::PostgraduateModel(QVector<CONST::HDG> indexMap,  QObject *parent) :
    QAbstractTableModel(parent), _indexMap(indexMap)
{
}

PostgraduateModel::~PostgraduateModel()
{
}

int PostgraduateModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->_list.size();
}

int PostgraduateModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->_indexMap.size();
}

QVariant PostgraduateModel::data(const QModelIndex &index, int role) const
{
    //* NOTE: if used across days, Today string will be inaccurate.
    static QString birthTooltip = "Valid date range: 1880-01-01 to Today(" + QDate::currentDate().toString("yyyy-MM-dd") + ")";

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    } else if ((role == Qt::DisplayRole || role == Qt::EditRole) && !this->_list.isEmpty()) {
        const Postgraduate &t = _list.at(index.row());

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

            case CONST::HDG::CLASSNO:
                return t.getClassNo();

            case CONST::HDG::MAJOR:
                return t.getMajor();

            case CONST::HDG::TUTORID:
                return t.getTutorId();

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

bool PostgraduateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        bool result = false;
        Postgraduate &t = this->_list[index.row()];

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

            case CONST::HDG::CLASSNO:
                result = t.setClassNo(value.toString());
                break;

            case CONST::HDG::MAJOR:
                result = t.setMajor(value.toString());
                break;

            case CONST::HDG::TUTORID:
                result = t.setTutorId(value.toString());
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

Qt::ItemFlags PostgraduateModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

bool PostgraduateModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // NOTE: [row, row+count-1], not [row, row+count)
    beginInsertRows(parent, row, row + count - 1);

    for (int i = row; i < row + count; ++i) {
        this->_list.insert(i, Postgraduate());
    }

    endInsertRows();
    return true;
}

// remove from high index to low index
bool PostgraduateModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // NOTE: [row, row+count-1], not [row, row+count)
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = row + count - 1; i >= row; i--) {
        this->_list.removeAt(i);
    }

    endRemoveRows();
    return true;
}

QVariant PostgraduateModel::headerData(int section, Qt::Orientation orientation, int role) const
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

void PostgraduateModel::setDataList(QList<Postgraduate> list)
{
    beginResetModel();
    this->_list = list;
    endResetModel();
}

QList<Postgraduate> PostgraduateModel::getDataList()
{
    return this->_list;
}

QVector<CONST::HDG> PostgraduateModel::getHeaderIndexs() const
{
    return this->_indexMap;
}

void PostgraduateModel::sort(int column, Qt::SortOrder order)
{
    // std::function is about two times slower than lambda expression
    //std::function<bool(Postgraduate, Postgraduate)> func;
    beginResetModel();

    switch (this->_indexMap[column]) {
        case CONST::HDG::ID:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getId() < b.getId(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getId() >= b.getId(); });
            }

            break;

        case CONST::HDG::NAME:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [this](Postgraduate a, Postgraduate b) -> bool {
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
                qStableSort(this->_list.begin(), this->_list.end(), [this](Postgraduate a, Postgraduate b) -> bool {
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
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getSex() < b.getSex(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getSex() >= b.getSex(); });
            }

            break;

        case CONST::HDG::BIRTHDAY:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getBirthday() < b.getBirthday(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getBirthday() >= b.getBirthday(); });
            }

            break;

        case CONST::HDG::IDNUMBER:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getIdNumber() < b.getIdNumber(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getIdNumber() >= b.getIdNumber(); });
            }

            break;

        case CONST::HDG::CLASSNO:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getClassNo() < b.getClassNo(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getClassNo() >= b.getClassNo(); });
            }

            break;

        case CONST::HDG::MAJOR:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getMajor() < b.getMajor(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getMajor() >= b.getMajor(); });
            }

            break;

        case CONST::HDG::TUTORID:
            if (Qt::AscendingOrder == order) {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getTutorId() < b.getTutorId(); });
            } else {
                qStableSort(this->_list.begin(), this->_list.end(), [](Postgraduate a, Postgraduate b) -> bool {return a.getTutorId() >= b.getTutorId(); });
            }

            break;

        default:
            break;
    }

    endResetModel();
}

void PostgraduateModel::filter()
{
    if (! this->_filterEnabled) {
        return;
    }

    this->_isFiltering = true;
    QList<Postgraduate> newList;

    switch (this->_filterColumn) {
        case CONST::HDG::ID: {
            if (this->_useRegexp) {
                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getId().contains(this->_filterRegexp)) {
                        newList.append(postgrad);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getId().contains(this->_filterString, sensitivity)) {
                        newList.append(postgrad);
                    }
                }
            }
        }
        break;

        case CONST::HDG::NAME: {
            if (this->_useRegexp) {
                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getName().contains(this->_filterRegexp)) {
                        newList.append(postgrad);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getName().contains(this->_filterString, sensitivity)) {
                        newList.append(postgrad);
                    }
                }
            }
        }
        break;

        case CONST::HDG::SEX: {
            for (Postgraduate &postgrad : this->_listOriginal) {
                if (postgrad.getSex() == this->_filterSex) {
                    newList.append(postgrad);
                }
            }
        }
        break;

        case CONST::HDG::BIRTHDAY: {
            for (Postgraduate &postgrad : this->_listOriginal) {
                if (this->_minDate <= postgrad.getBirthday() && postgrad.getBirthday() <= this->_maxDate) {
                    newList.append(postgrad);
                }
            }
        }
        break;

        case CONST::HDG::IDNUMBER: {
            if (this->_useRegexp) {
                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getIdNumber().contains(this->_filterRegexp)) {
                        newList.append(postgrad);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getIdNumber().contains(this->_filterString, sensitivity)) {
                        newList.append(postgrad);
                    }
                }
            }
        }
        break;

        case CONST::HDG::CLASSNO: {
            if (this->_useRegexp) {
                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getClassNo().contains(this->_filterRegexp)) {
                        newList.append(postgrad);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getClassNo().contains(this->_filterString, sensitivity)) {
                        newList.append(postgrad);
                    }
                }
            }
        }
        break;

        case CONST::HDG::MAJOR: {
            if (this->_useRegexp) {
                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getMajor().contains(this->_filterRegexp)) {
                        newList.append(postgrad);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getMajor().contains(this->_filterString, sensitivity)) {
                        newList.append(postgrad);
                    }
                }
            }
        }
        break;

        case CONST::HDG::TUTORID: {
            if (this->_useRegexp) {
                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getTutorId().contains(this->_filterRegexp)) {
                        newList.append(postgrad);
                    }
                }
            } else {
                Qt::CaseSensitivity sensitivity = this->_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

                for (Postgraduate &postgrad : this->_listOriginal) {
                    if (postgrad.getTutorId().contains(this->_filterString, sensitivity)) {
                        newList.append(postgrad);
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

void PostgraduateModel::setRegexp(QString pattern)
{
    this->_filterRegexp.setPattern(pattern);

    if (this->_caseSensitive) {
        this->_filterRegexp.setPatternOptions(QRegularExpression::DontCaptureOption);
    } else {
        this->_filterRegexp.setPatternOptions(QRegularExpression::DontCaptureOption | QRegularExpression::CaseInsensitiveOption);
    }
}
