#include "modemrestarter.h"
#include <QFile>
#include <QRegExp>
//#include <QUrlQuery>
#include <QCoreApplication>

ModemRestarter::ModemRestarter(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
        SLOT(provideAuthenication(QNetworkReply*,QAuthenticator*)));

    connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(DeviceInfoReplyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://192.168.1.1/status/status_deviceinfo.htm")));
}

void ModemRestarter::DeviceInfoReplyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QRegExp rx("Default Gateway(\\D*\\d\\D\\>\\s)([\\d.]*)");
        QString data = reply->readAll();
        int pos = 0;

        if ((pos = rx.indexIn(data, pos)) != -1) {
             data = rx.cap(2);
             if (data.compare("0.0.0.0") == 0)
             {
				 QByteArray postData;
				 QUrl params;
                 params.addQueryItem("restoreFlag", "0");
                 params.addQueryItem("Restart", "RESTART");
				 postData.append(params.toString());
				 postData.remove(0,1);
				 
                 QNetworkRequest request(QUrl("http://192.168.1.1/Forms/tools_system_1"));
                 request.setHeader(QNetworkRequest::ContentTypeHeader,
                     "application/x-www-form-urlencoded");

                 disconnect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(DeviceInfoReplyFinished(QNetworkReply*)));
                 connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(RestartReplyFinished(QNetworkReply*)));

                 manager->post(request, postData);
             }
			 else
			 {
                QCoreApplication::exit(0);
			 }
        }
    }

    reply->deleteLater();
}

void ModemRestarter::RestartReplyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
	{
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 303)
        {
            manager->get(QNetworkRequest(QUrl((reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString()))));
        }
        else
        {
            QCoreApplication::exit(0);
        }
    }

    reply->deleteLater();
}

void ModemRestarter::provideAuthenication(QNetworkReply *reply, QAuthenticator *ator)
{
    ator->setUser(QString("admin"));
    ator->setPassword(QString("admin"));
}
