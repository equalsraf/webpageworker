#ifndef WEBPAGEWORKER_NETWORKACCESSMANAGER_H
#define WEBPAGEWORKER_NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QUrl>
#include "cookiejar.h"

class NetworkAccessManager: public QNetworkAccessManager
{
	Q_OBJECT
public:
	NetworkAccessManager(QObject *parent=0);
	CookieJar* cookieJar();

signals:
	void requestedUrl(QUrl);

protected:
	virtual QNetworkReply *createRequest(QNetworkAccessManager::Operation, const QNetworkRequest& req, QIODevice *data=0);

private:
	CookieJar *m_cookiejar;
};

#endif
