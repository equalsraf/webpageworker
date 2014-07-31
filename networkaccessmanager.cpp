#include "networkaccessmanager.h"
#include <QNetworkRequest>
#include <QDebug>

NetworkAccessManager::NetworkAccessManager(QObject *parent)
:QNetworkAccessManager(parent)
{

}

QNetworkReply* NetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest& req, QIODevice *data)
{
	// We only gather GET Urls
	if ( op == QNetworkAccessManager::GetOperation ) {
		emit requestedUrl(req.url());
	}
	return QNetworkAccessManager::createRequest(op, req, data);
}
