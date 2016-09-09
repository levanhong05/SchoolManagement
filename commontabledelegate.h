#pragma once

#include <QStyledItemDelegate>
#include <QVector>

#include "const.h"

class CommonTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CommonTableDelegate(QVector<CONST::HDG> indexMap, QObject *parent = 0);

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QVector<CONST::HDG> _indexMap;

};
