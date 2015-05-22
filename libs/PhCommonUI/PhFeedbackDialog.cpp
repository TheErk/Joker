/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhFile.h"

#include "PhFeedbackDialog.h"
#include "ui_PhFeedbackDialog.h"


PhFeedbackDialog::PhFeedbackDialog(PhFeedbackSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhFeedbackDialog),
	_settings(settings)
{

	ui->setupUi(this);

	ui->problemLabel->setText(ui->problemLabel->text().arg(QString(APP_NAME)));

	if(!_settings->emailList().isEmpty())
		ui->comboBoxEmails->addItems(_settings->emailList());
}

PhFeedbackDialog::~PhFeedbackDialog()
{
	delete ui;
}

void PhFeedbackDialog::on_buttonBox_accepted()
{
	QStringList emails;
	for(int i = 0; i < ui->comboBoxEmails->count(); i++) {
		emails += ui->comboBoxEmails->itemText(i);
	}
	emails.removeOne(ui->comboBoxEmails->currentText());
	emails.insert(0, ui->comboBoxEmails->currentText());
	_settings->setEmailList(emails);



	QString systemConfig;
	QString crashLog;
	QString appLog;
	QString header;
	QString preferences;


	header = "";

	header += "--------Feedback report: " + QString(APP_NAME) + " v" + QString(APP_VERSION) + "--------\n";
	header += "From : " + ui->comboBoxEmails->currentText() + "\n";
	if(!ui->textEditComment->toPlainText().isEmpty())
		header += "Message : " + ui->textEditComment->toPlainText() + "\n";

	// Get the system infos
	QString commandString;
#if defined(Q_OS_MAC)
	commandString = "/usr/sbin/system_profiler SPHardwareDataType";
#else
	commandString = "echo Not handled in Window and Linux.";
#endif

	QProcess process;
	process.start(commandString);
	if(process.waitForFinished()) {
		systemConfig = QString(process.readAllStandardOutput());
	}

	// Get the preferences
#if defined(Q_OS_MAC)
	commandString = QString("defaults read com.Phonations.%1").arg(QString(APP_NAME));
#else
	commandString = "echo Not handled in Window and Linux.";
#endif

	process.start(commandString);
	if(process.waitForFinished()) {
		preferences = QString(process.readAllStandardOutput());
	}

	// Get the application log
#if defined(Q_OS_MAC)
	commandString = QString("tail -n 100 %1/Library/Logs/%2/%3.log").arg(QDir::homePath()).arg(ORG_NAME).arg(QString(APP_NAME));
#else
	commandString = "echo Not handled in Window and Linux.";
#endif

	process.start(commandString);
	if(process.waitForFinished()) {
		appLog = QString(process.readAllStandardOutput());
	}

	// Get the crash log
	QString crashFolder = QDir::homePath() + "/Library/Logs/DiagnosticReports/";
	QDir crashDir(crashFolder);

	QStringList crashFilters;
	crashFilters.append(QString(APP_NAME) + "*.crash" );
	crashDir.setNameFilters(crashFilters);
	QStringList crashFiles = crashDir.entryList();
	QString lastCrashFilePath = crashFolder + crashFiles.first();
	PHDEBUG << "last crash log:" << lastCrashFilePath;
	foreach(QString file, crashFiles) {
		QString filePath = crashFolder + file;
		if(QFileInfo(filePath).created() > QFileInfo(lastCrashFilePath).created())
			lastCrashFilePath = filePath;
	}
	PHDEBUG << lastCrashFilePath;
	QFile crashFile(lastCrashFilePath);
	if(!crashFile.open(QIODevice::ReadOnly)) {
		PHDEBUG << "Crash log : " << crashFile.errorString();
	}
	else {
		QTextStream in(&crashFile);
		while(!in.atEnd()) {
			crashLog += in.readLine()  + "\n";
		}
		crashFile.close();
	}


	QString name = QString("name=%1&").arg(QHostInfo::localHostName());

	header.remove("[=|&]");
	header.insert(0, "header=");
	header.append("&");

	QString post;

	post = name + header;

	if(!preferences.isEmpty()) {
		preferences.remove("[=|&]");
		preferences.insert(0, "preferences=");
		preferences.append("&");
		post += preferences;
		PHDEBUG << "add preferences:" << preferences.length();
	}

	if(!systemConfig.isEmpty()) {
		systemConfig.remove("[=|&]");
		systemConfig.insert(0, "configuration=");
		systemConfig.append("&");
		post += systemConfig;
		PHDEBUG << "add systemConfig:" << systemConfig.length();
	}

	if(!appLog.isEmpty()) {
		appLog.replace("&", "amp");
		appLog.insert(0, "applicationLog=");
		appLog.append("&");
		post += appLog;
		PHDEBUG << "add appLog:" << appLog.length();
	}

	if(!crashLog.isEmpty()) {
		crashLog.remove("[=|&]");
		crashLog.insert(0, "crashLog=");
		crashLog.append("&");
		post += crashLog;
		PHDEBUG << "add crashLog:" << crashLog.length();
	}

	QNetworkRequest request(QUrl("http://www.phonations.com/feedback.php"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSyncRequestFinished(QNetworkReply*)));

	// Send it
	manager->post(request, post.toUtf8());
	QMessageBox::information(this, "Information", tr("Thank you for your feedback!"), QMessageBox::Ok, QMessageBox::Ok);
	hide();
}

void PhFeedbackDialog::on_buttonBox_rejected()
{
	hide();
}

void PhFeedbackDialog::onSyncRequestFinished(QNetworkReply * reply)
{
	delete reply;
}
