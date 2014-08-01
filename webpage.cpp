#include "webpage.h"

#include <QWebFrame>
#include <QWebElement>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QtCore>

WebPage::WebPage(QObject *parent)
:QWebPage(parent), m_failed(false), m_quitOnLoad(false)
{
	setNetworkAccessManager(&m_nam);
	setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	connect(mainFrame(), &QWebFrame::loadFinished,
		this, &WebPage::pageLoaded);
	connect(mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
		this, &WebPage::mainFrameJSCleared);
	connect(this, &QWebPage::frameCreated,
		this, &WebPage::newFrameCreated);
}

QString WebPage::url() 
{
	return mainFrame()->url().toString();
}

void WebPage::load(const QString& url) 
{ 
	mainFrame()->load(QUrl::fromUserInput(url));
}


void WebPage::newFrameCreated(QWebFrame* f)
{
	connect(f, &QWebFrame::loadFinished,
			[f](bool ok) {
				injectVideoJS(f);
			});
	connect(f, &QWebFrame::javaScriptWindowObjectCleared,
			[f]() {
				injectVideoJS(f);
			});
	injectVideoJS(f);
}

void WebPage::pageLoaded(bool ok)
{
	if ( !ok ) {
		m_failed = !ok;
		addResult("error", QVariant("Unable to load URL: "+ url() ));
		done(-1);
		return;
	}

	if ( m_script.isEmpty() ) {
		addResult("error", QVariant("No script handler was defined "));
		done(-1);
		return;
	}
	// Use this to test for WebPage ... although it should
	// not be needed
	QVariant missing = mainFrame()->evaluateJavaScript("typeof(WebPage)===\"undefined\"");

	mainFrame()->evaluateJavaScript(m_script);
	if (!m_latestException.isEmpty()) {
		addResult("error", QVariant(m_latestException));
		done(-1);
		return;
	}

	if (m_quitOnLoad) {
		done(0);
	}
}

void WebPage::setLatestException(const QString& e)
{
	m_latestException = e;
}

void WebPage::setHandlerScript(const QString& script)
{
	if ( script.isEmpty() ) {
		m_script = "";
	} else {
		m_script = "try {\n" + script + "\n} catch(e) {\nWebPage.setLatestException(e.toString());\n}";
	}
}
 
void WebPage::setQuitOnLoad(bool quit)
{
	m_quitOnLoad = quit;
}
/**
 * Get outer HTML for element
 * - This exists mostly for debbugging
 */
QString WebPage::findFirstHTML(const QString& sel)
{
	return mainFrame()->findFirstElement(sel).toOuterXml();
}

QString WebPage::findFirst(const QString& sel)
{
	return mainFrame()->findFirstElement(sel).toInnerXml();
}

QString WebPage::findFirstAttr(const QString& sel, const QString& attr)
{
	return mainFrame()->findFirstElement(sel).attribute(attr);
}

QVariantList WebPage::find(const QString& sel)
{
	QVariantList result;
	QWebElementCollection col = mainFrame()->findAllElements(sel);
	foreach(const QWebElement& el, col) {
		result << toVariant(el);
	}
	return result;
}

QVariant WebPage::toVariant(const QWebElement& el)
{
	QVariantMap res;

	QVariantMap attrs;
	foreach(QString attr, el.attributeNames()) {
		attrs.insert(attr, el.attribute(attr));
	}
	res.insert("attrs", attrs);
	res.insert("text", el.toPlainText());
	res.insert("xml", el.toOuterXml());
	return res;
}

QStringList WebPage::findAttr(const QString& sel, const QString& attr)
{
	QStringList result;
	QWebElementCollection col = mainFrame()->findAllElements(sel);
	foreach(const QWebElement& el, col) {
		if ( el.hasAttribute(attr) && !el.attribute(attr).isEmpty() ) {
			result << el.attribute(attr);
		}
	}
	return result;
}

bool WebPage::click(const QString& sel)
{
	QWebElement target = mainFrame()->findFirstElement(sel);
	if ( target.isNull() ) {
		return false;
	} else {
		target.evaluateJavaScript("this.click()");
		return true;
	}
}


QString WebPage::fixUrl(const QString& url)
{
	return QUrl::fromPercentEncoding(url.toUtf8());
}

QString WebPage::absUrl(const QString& in)
{
	QUrl relative_url(in);
	return mainFrame()->url().resolved(relative_url).toString();
}

void WebPage::javaScriptConsoleMessage(const QString& msg, int line, const QString& source)
{
	qDebug() << __func__ << source << line << msg;
}

QString WebPage::html() 
{
	return mainFrame()->toHtml(); 
}

QString WebPage::text() 
{
	return mainFrame()->toPlainText(); 
}

bool WebPage::failed()
{
	return m_failed;
}

void WebPage::setUserAgent(const QString& ua)
{
	m_userAgent = ua;
}

QString WebPage::userAgentForUrl(const QUrl& u) const
{
	if ( m_userAgent.isEmpty() ) {
		return QWebPage::userAgentForUrl(u);
	} else {
		return m_userAgent;
	}
}

bool WebPage::javaScriptConfirm(QWebFrame *, const QString& msg)
{
	return false;
}

void WebPage::mainFrameJSCleared()
{
	mainFrame()->addToJavaScriptWindowObject("WebPage", this);
	QFile f(":/js/webpage.js");
	if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
		qDebug() << "Unable to read :/js/webpage.js";
		return;
	}
	QString script = QString::fromUtf8(f.readAll());
	mainFrame()->evaluateJavaScript(script);
	injectVideoJS(mainFrame());
}

void WebPage::injectVideoJS(QWebFrame *frame)
{
	QFile f(":/js/video.js");
	if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
		qDebug() << "Unable to read :/js/video.js";
		return;
	}
	QString script = QString::fromUtf8(f.readAll());
	frame->evaluateJavaScript(script);
}

void WebPage::done(int code)
{
	QVariantMap res;
	foreach(QString key, m_result.keys()) {
		res.insert(key, m_result.value(key));
	}

	// Cookies
	QStringList cookies;
	foreach(QNetworkCookie c, m_nam.cookieJar()->allCookies()) {
		cookies << c.toRawForm();
	}
	res.insert("cookies", cookies);

	emit handlerFinished(res, code);
}

QVariant WebPage::getData(const QString& name, QVariant def)
{
	if ( m_data.contains(name) ) {
		return m_data.value(name);
	}
	return def;
}

void WebPage::setData(const QString& name, const QVariant& val)
{
	m_data.insert(name, val);
}

void WebPage::addResult(const QString& name, const QString& val)
{
	QJsonParseError err;
	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8(), &err);

	if ( err.error != QJsonParseError::NoError ) {
		qDebug() << "Error parsing JSON " << err.errorString() << val;
	}

	return addResult(name, d.toVariant());
}

void WebPage::addResult(const QString& name, const QVariant& val)
{
	if ( !m_result.contains(name) ) {
		m_result.insert(name, QVariantList());
	}

	if ( val.type() == QVariant::List ) {
		m_result[name].append(val.toList());
	} else {
		m_result[name].append(val);
	}
}

void WebPage::log(const QString& msg)
{
	qDebug() << msg;
}


