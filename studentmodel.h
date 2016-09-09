#pragma once

/* QT Q_OBJECTS doesn't support templated classes, so convert back to StudentTabloeModel */

#include <QList>
#include <QVector>
#include <QAtomicInt>
#include <QStringList>
#include <QRegularExpression>
#include <QAbstractTableModel>

#if QT_VERSION >= 0x050200
    #include <QCollator>
#else
    #include <locale>
#endif

#include "student.h"

class StudentModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StudentModel(QVector<CONST::HDG> indexMap, QObject *parent = 0);
    ~StudentModel();

    // SortProxyModel using this model as source model should override these same functions
    // must override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // optional override
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order);

    void setDataList(QList<Student> list);
    QList<Student> getDataList();
    QVector<CONST::HDG> getHeaderIndexs() const;

public slots:
    void setEnableFilter(bool enable);
    void setFilterColumn(const CONST::HDG column);
    void setFilterString(const QString &newString);
    void setFilterSex(const Person::Sex sex);
    void setFilterCaseSentivity(bool sensitive);
    void setFilterUseRegexp(bool yes);
    void setFilterMinDate(const QDate &min);
    void setFilterMaxDate(const QDate &max);

protected:
//#ifdef _MSC_VER
#if QT_VERSION >= 0x050200
    const QCollator _vi_VN_collate = QCollator(QLocale(QLocale::Vietnamese, QLocale::Vietnam));
#else
#   ifdef _WIN32
    const std::locale _vi_VN_collate = std::locale("Vietnamese_vietnam");
#   else
    const std::locale _vi_VN_collate = std::locale("vi_VN.utf8");
#   endif
    const std::collate<char> &_vi_VN_collate = std::use_facet<std::collate<char> >(_vi_VN_collate);
#endif

    void filter();

    void setRegexp(const QString pattern);

private:
    QList<Student> _list = QList<Student>();
    QList<Student> _listOriginal = _list;

    bool _isFiltering = false;
    bool _filterEnabled = false;
    bool _useRegexp = false;
    bool _caseSensitive = false;

    CONST::HDG _filterColumn = CONST::HDG::ID;
    QRegularExpression _filterRegexp = QRegularExpression(".*", QRegularExpression::DontCaptureOption);
    Person::Sex _filterSex = Person::Sex::Male;
    QString _filterString = "";

    QDate _minDate = QDate(1880, 1, 1);
    QDate _maxDate = QDate::currentDate();

    // column index map to CONST::HDG enum types
    QVector<CONST::HDG> _indexMap = QVector<CONST::HDG>(static_cast<int>(CONST::HDG::COUNT));
};

inline void StudentModel::setEnableFilter(bool enable)
{
    this->_filterEnabled = enable;

    if (enable) {
        this->_listOriginal = this->_list;
    } else {
        beginResetModel();
        this->_list = this->_listOriginal;
        endResetModel();
    }
}

inline void StudentModel::setFilterColumn(const CONST::HDG column)
{
    if (this->_isFiltering == false && column != this->_filterColumn) {
        this->_filterColumn = column;

        filter();
    }
}

inline void StudentModel::setFilterString(const QString &newString)
{
    if (this->_isFiltering == false && newString != this->_filterString) {
        this->_filterString = newString;

        if (this->_useRegexp) {
            setRegexp(newString);
        }

        filter();
    }
}

inline void StudentModel::setFilterSex(const Person::Sex sex)
{
    if (this->_isFiltering == 0 && sex != this->_filterSex) {
        this->_filterSex = sex;

        filter();
    }
}

inline void StudentModel::setFilterCaseSentivity(bool sensitive)
{
    if (this->_isFiltering == 0 && sensitive != this->_caseSensitive) {
        this->_caseSensitive = sensitive;

        if (this->_useRegexp) {
            setRegexp(this->_filterString);
        }

        filter();
    }
}

inline void StudentModel::setFilterUseRegexp(bool yes)
{
    if (this->_isFiltering == 0 && yes != this->_useRegexp) {
        this->_useRegexp = yes;

        if (yes) {
            setRegexp(this->_filterString);
        }

        filter();
    }
}

inline void StudentModel::setFilterMinDate(const QDate &min)
{
    if (this->_isFiltering == 0 && min != this->_minDate) {
        this->_minDate = min;

        filter();
    }
}

inline void StudentModel::setFilterMaxDate(const QDate &max)
{
    if (this->_isFiltering == 0 && max != this->_maxDate) {
        this->_maxDate = max;

        filter();
    }
}
