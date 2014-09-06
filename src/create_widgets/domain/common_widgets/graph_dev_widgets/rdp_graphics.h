#ifndef RDP_GRAPHICS_H
#define RDP_GRAPHICS_H

#include "create_widgets/domain/_qwidget.h"

class RDP_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit RDP_Graphics(QWidget *parent = 0);

private:
    QCheckBox       *autoPort;
    QSpinBox        *port;
    QCheckBox       *multiUser;
    QCheckBox       *replaceUser;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void autoPortChanged(bool);
};

#endif // RDP_GRAPHICS_H