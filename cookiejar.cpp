#include "cookiejar.h"
#include <QNetworkCookie>

CookieJar::CookieJar(QObject *parent)
:QNetworkCookieJar(parent)
{
}

QList<QNetworkCookie> CookieJar::allCookies() const
{
	return QNetworkCookieJar::allCookies();
}
