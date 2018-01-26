#include "mainwindow.h"
#include "droparea.h"
#include "hasher.h"

#include <QDebug>
#include <QMetaType>
#include <QPlainTextEdit>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    qRegisterMetaType<QCryptographicHash::Algorithm>("QCryptographicHash::Algorithm");
    dropArea = new DropArea;
    connect(dropArea, &DropArea::changed,
            this, &MainWindow::getFileHash);

    Hasher *worker = new Hasher;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::operate, worker, &Hasher::doWork);
    connect(worker, &Hasher::resultReady, this, &MainWindow::handleResults);
    workerThread.start();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(dropArea);
    plainTextEditOutput = new QPlainTextEdit(this);
    mainLayout->addWidget(plainTextEditOutput);

    setWindowTitle(tr("QHash"));
    setMinimumSize(350,350);
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
}

void MainWindow::getFileHash(const QMimeData *mimeData)
{
    if (!mimeData) {
        qDebug() << "mimeData is empty?";
        return;
    }
    QString text;
    foreach (QString format, mimeData->formats()) {
        if (format == QLatin1String("text/uri-list")) {
            QList<QUrl> urlList = mimeData->urls();
            for (int i = 0; i < urlList.size() && i < 32; ++i) {
                text.append(urlList.at(i).toString() + QLatin1Char(' '));
            }
            for (int i = 0; i < urlList.size() && i < 32; ++i) {
                QString fileUrl = urlList.at(i).toLocalFile();
                emit operate(fileUrl, QCryptographicHash::Sha256);
            }
        } else {
        }
    }
}

void MainWindow::handleResults(const QString &result)
{
    plainTextEditOutput->appendPlainText(result);
}
