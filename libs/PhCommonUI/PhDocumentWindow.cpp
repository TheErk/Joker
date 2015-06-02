#include "PhTools/PhDebug.h"

#include "PhDocumentWindow.h"

PhDocumentWindow::PhDocumentWindow(PhDocumentWindowSettings *settings)
	: PhWindow(settings),
	_settings(settings)
{
	connect(&_watcher, SIGNAL(fileChanged(QString)), this, SLOT(onExternalChange(QString)));
}

void PhDocumentWindow::processArg(int argc, char *argv[])
{
#warning /// @todo move to PhApplication
	for(int i = 1; i < argc; i++) {
		if(QFile::exists(argv[i]))
			_settings->setCurrentDocument(argv[i]);
	}
	if(QFile::exists(_settings->currentDocument()))
		openDocument(_settings->currentDocument());
	else
		updateRecentDocumentMenu();
}

void PhDocumentWindow::resetDocument()
{
	_settings->setCurrentDocument("");
	this->setWindowTitle("");
	if(!_watcher.files().isEmpty())
		_watcher.removePaths(_watcher.files());
}

bool PhDocumentWindow::openDocument(const QString &fileName)
{
	resetDocument();
	if(_watcher.addPath(fileName))
		PHDEBUG << "now watching " << fileName;
	_settings->setCurrentDocument(fileName);
	if(!fileName.isEmpty())
		_settings->setLastDocumentFolder(QFileInfo(fileName).absolutePath());
	this->setWindowTitle(fileName);

	QStringList recentDocList = _settings->recentDocumentList();
	recentDocList.removeAll(fileName);
	recentDocList.insert(0, fileName);
	while(recentDocList.size() > _settings->maxRecentDocument())
		recentDocList.removeLast();

	_settings->setRecentDocumentList(recentDocList);

	updateRecentDocumentMenu();

	return true;
}

bool PhDocumentWindow::eventFilter(QObject *sender, QEvent *event)
{
	switch (event->type()) {
	case QEvent::FileOpen:
		{
			QString fileName = static_cast<QFileOpenEvent*>(event)->file();
			PHDEBUG << "File dragged on the application dock icon (MacOS):" + fileName;
			openDocument(fileName);
			break;
		}
	case QEvent::DragEnter: // Accept and process a file drop on the window
		event->accept();
		break;
	case QEvent::Drop:
		{
			const QMimeData* mimeData = static_cast<QDropEvent *>(event)->mimeData();

			// If there is one file (not more) we open it
			if (mimeData->urls().length() == 1) {
				QString fileName = mimeData->urls().first().toLocalFile();
				PHDEBUG << "File dragged on the application window:" + fileName;
				openDocument(fileName);
			}
			break;
		}
	default:
		break;
	}

	return PhWindow::eventFilter(sender, event);
}

void PhDocumentWindow::onOpenRecentDocumentTriggered()
{
	QString fileName = sender()->objectName();
	PHDEBUG << fileName;
	openDocument(fileName);
}

void PhDocumentWindow::updateRecentDocumentMenu()
{
	if(recentDocumentMenu()) {
		QStringList recentDocList = _settings->recentDocumentList();

		if(_settings->recentDocumentList().count()) {
			recentDocumentMenu()->clear();
			foreach(QString doc, recentDocList) {
				QAction *action = recentDocumentMenu()->addAction(doc);
				action->setObjectName(doc);
				connect(action, SIGNAL(triggered()), this, SLOT(onOpenRecentDocumentTriggered()));
			}
		}
	}
}

void PhDocumentWindow::onExternalChange(const QString &path)
{
	PHDEBUG << "File changed :" << path;
	openDocument(_settings->currentDocument());
}
