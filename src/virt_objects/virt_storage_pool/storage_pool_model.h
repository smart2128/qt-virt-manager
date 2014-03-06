#ifndef STORAGE_POOL_MODEL_H
#define STORAGE_POOL_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include <QDebug>
#include "storage_pool_index.h"

class StoragePoolModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StoragePoolModel(QObject *parent = 0);
    ~StoragePoolModel();
    QList<StoragePoolIndex *>  virtStoragePoolDataList;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int);
    bool removeRow(int);

signals:

private:
    QString  column0;
    QString  column1;
    QString  column2;
    QString  column3;
    QIcon    activeIcon;
    QIcon    no_activeIcon;
    QIcon    defined;
    QIcon    created;

public slots:
};

#endif // STORAGE_POOL_MODEL_H
