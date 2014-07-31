#ifndef WEBPAGEWORKER_NETWORKACCESSMANAGER_H
#define WEBPAGEWORKER_NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QUrl>

class NetworkAccessManager: public QNetworkAccessManager
{
	Q_OBJECT
public:
	NetworkAccessManager(QObject *parent=0);

signals:
	void requestedUrl(QUrl);

protected:
	virtual QNetworkReply *createRequest(QNetworkAccessManager::Operation, const QNetworkRequest& req, QIODevice *data=0);

};

#endif
