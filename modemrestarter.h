#ifndef MODEMRESTARTER_H
#define MODEMRESTARTER_H

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QAuthenticator>

class ModemRestarter : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
public:
    explicit ModemRestarter(QObject *parent = 0);

signals:

public slots:
    void provideAuthenication(QNetworkReply *reply, QAuthenticator *ator);
    void DeviceInfoReplyFinished (QNetworkReply *reply);
    void RestartReplyFinished (QNetworkReply *reply);

};

#endif // MODEMRESTARTER_H
