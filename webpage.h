#ifndef WEBPAGEWORKER_WEBPAGE_H
#define WEBPAGEWORKER_WEBPAGE_H

#include <QWebPage>
#include "networkaccessmanager.h"

class WebPage: public QWebPage
{
	Q_OBJECT
	Q_PROPERTY(QString url READ url)
	Q_PROPERTY(QString html READ html)
	Q_PROPERTY(QString text READ text)
public:
	WebPage(QObject *parent=0);

	QString url();
	void setUserAgent(const QString& );
	QString html();
	QString text();
	void setHandlerScript(const QString& script); 

	static QVariant toVariant(const QWebElement& el);

	Q_INVOKABLE QVariant getData(const QString& name, QVariant def=QVariant());
	Q_INVOKABLE void setData(const QString& name, const QVariant& val);
	Q_INVOKABLE void addResult(const QString& name, const QString& val);
	void addResult(const QString& name, const QVariant& val);
	Q_INVOKABLE void log(const QString& msg);

	void setQuitOnLoad(bool);
public slots:

	QString findFirstHTML(const QString&);
	QString findFirst(const QString&);
	QString findFirstAttr(const QString&, const QString&);
	QVariantList find(const QString&);
	QStringList findAttr(const QString&, const QString&);
	bool click(const QString& );
	QString fixUrl(const QString&);
	QString absUrl(const QString&);
	bool failed();
	void load(const QString& url);
	void finishProcessing(const QVariant&, int error=0);
	void done(int error=0);
	void setLatestException(const QString&);

signals:
	void handlerFinished(const QVariant&, int);

protected slots:
	void pageLoaded(bool);
	void mainFrameJSCleared();
	void resetLatestException() {m_latestException="";}
	void newFrameCreated(QWebFrame*);

protected:
	virtual void javaScriptConsoleMessage(const QString& msg, int line, const QString& source) Q_DECL_OVERRIDE;
	virtual QString userAgentForUrl(const QUrl&) const Q_DECL_OVERRIDE;
	virtual bool javaScriptConfirm( QWebFrame* frame, const QString& msg ) Q_DECL_OVERRIDE;
	static void injectVideoJS(QWebFrame *f);

private:
	QString m_userAgent;
	int postponeTime;
	bool m_failed, m_quitOnLoad;
	QString m_script;
	QString m_latestException;
	QMap<QString, QVariantList> m_result;
	QVariantMap m_data;
	NetworkAccessManager m_nam;
};

#endif
