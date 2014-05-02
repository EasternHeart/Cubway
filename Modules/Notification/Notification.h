#ifndef _MODULES_NOTIFICATION_H
#define _MODULES_NOTIFICATION_H

#include <QObject>
#include <QString>

class NotificationModule: public QObject
{
    Q_OBJECT
public slots:
    void Send(QString title, QString body, QString icon);
};

#endif //_MODULES_NOTIFICATION_H