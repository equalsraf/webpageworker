#include "networkaccessmanager.h"
#include <QNetworkRequest>
#include <QDebug>

NetworkAccessManager::NetworkAccessManager(QObject *parent)
:QNetworkAccessManager(parent)
{
	m_cookiejar = new CookieJar(this);
	setCookieJar(m_cookiejar);
}

CookieJar* NetworkAccessManager::cookieJar()
{
	return m_cookiejar;
}

QNetworkReply* NetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& req, QIODevice *data)
{
	// We only gather GET Urls
	if ( op == QNetworkAccessManager::GetOperation ) {
		emit requestedUrl(req.url());
	}
	return QNetworkAccessManager::createRequest(op, req, data);
}
