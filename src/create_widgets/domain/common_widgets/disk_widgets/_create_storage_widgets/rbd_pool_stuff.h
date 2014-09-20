#ifndef RBD_POOL_STUFF_H
#define RBD_POOL_STUFF_H

#include "_pool_stuff.h"

class RBD_Pool_Stuff : public _Pool_Stuff
{
    Q_OBJECT
public:
    explicit RBD_Pool_Stuff(QWidget *parent = 0);

signals:

public slots:
    QString getStorageXMLDescFileName() const;
};

#endif // RBD_POOL_STUFF_H