#include <QApplication>
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QWebInspector>
#include <QWebSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include "webpage.h"

/**
 * Print error and exit with status -1
 */
void exit_error(const QString& msg)
{
	QJsonDocument doc;
	QJsonObject obj = doc.object();
	obj.insert("error", QJsonValue::fromVariant(QStringList() << msg));
	doc.setObject(obj);
	printf("%s", doc.toJson().data());
	exit(-1);
}

/**
 * Display command usage and exit with status -1
 *
 */
void exit_usage()
{
	fprintf(stderr, "webpageworker [options] <URL>\n");
	fprintf(stderr, "\t--inspector        Open the web inspector console\n");
	fprintf(stderr, "\t--quit-on-load     Don't wait for WebPage.done(), quit after running\n");
	fprintf(stderr, "\t                   the script\n");
	fprintf(stderr, "\t--script <file>    Run given script on page\n");
	fprintf(stderr, "\t--stdin            Read script from stdin\n");
	fprintf(stderr, "\t--timeout <time>   Max time(seconds) we wait before aborting\n");
	fprintf(stderr, "\t--user-agent <UA>  Set user agent\n");
	fprintf(stderr, "\t--view             Display webview\n");
	exit(-1);
}

int main(int argc, char *argv[])
{
	bool opt_stdin = false;
	QString scriptPath;
	unsigned int timeout=0;
	bool opt_inspector = false;
	bool opt_view = false;
	bool opt_quit_on_load = false;
	QApplication app(argc, argv);

	if ( app.arguments().size() < 2 ) {
		exit_error("Invalid arguments");
	}

	QString arg_url = app.arguments().at(app.arguments().size()-1);
	if (arg_url == "--help" || arg_url == "-h") {
		exit_usage();
	}

	QUrl url = QUrl::fromUserInput(arg_url);
	if (!url.isValid() || url.isRelative()) {
		exit_error("Invalid URL: " + arg_url);
	}

	WebPage *w = new WebPage();
	for (int i=1; i<app.arguments().size()-1; i++) {
		QString arg = app.arguments().at(i);
		if ( arg == "--stdin") {
			opt_stdin = true;
		} else if ( arg == "--help" || arg == "-h") {
			exit_usage();
		} else if ( arg == "--inspector") {
			opt_inspector = true;
		} else if ( arg == "--quit-on-load") {
			opt_quit_on_load = true;
		} else if ( arg == "--user-agent") {
			w->setUserAgent(app.arguments().at(i+1));
			i++;
		} else if ( arg == "--script") {
			scriptPath = app.arguments().at(i+1);
			i++;
		} else if ( arg == "--timeout") {
			timeout = app.arguments().at(i+1).toUInt();
			i++;
		} else if ( arg == "--view") {
			opt_view = true;
		} else {
			exit_error("Unknown argument: " + app.arguments().at(i) );
		}
	}

	if (opt_quit_on_load) {
		w->setQuitOnLoad(true);
	}

	if ( opt_inspector ) {
		QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
		QWebInspector *inspector = new QWebInspector();
		inspector->setPage(w);
		inspector->show();
	}

	QFile f;
	if ( opt_stdin ) {
		// Read javascript from stdin until a \0 is found
		f.open(stdin, QIODevice::ReadOnly | QIODevice::Text);

	} else if (scriptPath.isEmpty() && !opt_inspector) {
		exit_error("No script was given");
	} else {
		// Read javascript from file
		f.setFileName(scriptPath);
		if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
			exit_error("Unable to read script file: " + f.errorString());
		}
	}
	QString script = QString::fromUtf8(f.readAll());
	w->setHandlerScript(script);

	QObject::connect(w, &WebPage::handlerFinished, [opt_inspector](const QVariant& v, int code){

		QJsonDocument res = QJsonDocument::fromVariant(v);
		printf("%s", res.toJson().data());
		if (!opt_inspector) {
			QApplication::exit(code);
		}
	});

	w->settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
	w->settings()->setAttribute(QWebSettings::PluginsEnabled, false);
	w->settings()->setAttribute(QWebSettings::JavaEnabled, false);
	w->settings()->setAttribute(QWebSettings::AutoLoadImages, false);
	w->settings()->setAttribute(QWebSettings::DnsPrefetchEnabled, true);
	w->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

	if (opt_view) {
		QWebView *v = new QWebView();
		v->setPage(w);
		v->show();
	}

	if (timeout>0) {
		QTimer *t = new QTimer();
		t->setSingleShot(true);
		QObject::connect(t, &QTimer::timeout, [&app](){
				app.quit();
			});
		t->start(timeout*1000);
	}
	w->load(url.toString());
	return app.exec();
}
