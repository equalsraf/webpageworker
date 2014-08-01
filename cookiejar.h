#ifndef WEBPAGEWORKER_COOKIEJAR_H
#define WEBPAGEWORKER_COOKIEJAR_H

#include <QNetworkCookieJar>

class CookieJar: public QNetworkCookieJar
{
public:
	CookieJar(QObject *parent=0);
	virtual QList<QNetworkCookie> allCookies() const;
};

#endif
