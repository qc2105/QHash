#include "mainwindow.h"
#include "droparea.h"
#include "hasher.h"

#include <QDebug>
#include <QMetaType>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDir>



MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    qRegisterMetaType<QCryptographicHash::Algorithm>("QCryptographicHash::Algorithm");
    dropArea = new DropArea;
    connect(dropArea, &DropArea::changed,
            this, &MainWindow::getUrls);

    Hasher *worker = new Hasher;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::operate, worker, &Hasher::doWork);
    connect(worker, &Hasher::resultReady, this, &MainWindow::handleResults);
    workerThread.start();

    menu[0] = new QMenu("算法");
    act[0] = new QAction("SHA256", this);
    act[0]->setCheckable(true);
    menu[0]->addAction(act[0]);
    act[1] = new QAction("SHA512", this);
    act[1]->setCheckable(true);
    menu[0]->addAction(act[1]);
    act[2] = new QAction("MD5", this);
    act[2]->setCheckable(true);
    menu[0]->addAction(act[2]);

    menu[1] = new QMenu("清屏");
    act[3] = new QAction("上部", this);
    menu[1]->addAction(act[3]);
    act[4] = new QAction("下部", this);
    menu[1]->addAction(act[4]);
    act[5] = new QAction("全部", this);
    menu[1]->addAction(act[5]);

    menuBar = new QMenuBar(this);
    menuBar->addMenu(menu[0]);
    menuBar->addMenu(menu[1]);
    menuBar->setGeometry(0,0, this->width(),30);

    defaultHashAlgorithm = QCryptographicHash::Sha256;

    connect(menuBar, SIGNAL(triggered(QAction*)), this, SLOT(trigerMenu(QAction*)));



    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(menuBar);
    mainLayout->addWidget(dropArea);
    plainTextEditOutput = new QPlainTextEdit(this);
    mainLayout->addWidget(plainTextEditOutput);

    setWindowTitle(tr("文件完整性校验器"));
    setMinimumSize(500,350);
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
}

void MainWindow::calculateFileHash(QString fileUrl)
{
    // if fileUrl represents a normal file
    QFileInfo fileInfo(fileUrl);
    if (fileInfo.isFile())
    {
        if (hashAlgorithmList.size() <= 0) {
            emit operate(fileUrl, defaultHashAlgorithm);
        } else {
            QCryptographicHash::Algorithm hashAlgorithm;
            foreach (hashAlgorithm, hashAlgorithmList) {
                emit operate(fileUrl, hashAlgorithm);
            }
        }
    }
    else if (fileInfo.isDir())
    // if fileUrl represents a directory
    {
        qDebug() << fileUrl << ": is a directory.";
        QDir dir(fileUrl);
        foreach(QFileInfo tempFileInfo, dir.entryInfoList())
        {
            if(tempFileInfo.baseName() != "") { // if it is not dot and not dotdot
                qDebug() << tempFileInfo.filePath();
                calculateFileHash(tempFileInfo.absoluteFilePath());
            }
        }
    }
}

void MainWindow::getUrls(const QMimeData *mimeData)
{
    if (!mimeData) {
        qDebug() << "mimeData is empty?";
        return;
    }
    QString text;
    foreach (QString format, mimeData->formats()) {
        if (format == QLatin1String("text/uri-list")) {
            QList<QUrl> urlList = mimeData->urls();
            for (int i = 0; i < urlList.size() && i < MAXNFILES; ++i) {
                text.append(urlList.at(i).toString() + QLatin1Char(' '));
            }
            for (int i = 0; i < urlList.size() && i < MAXNFILES; ++i) {
                QString fileUrl = urlList.at(i).toLocalFile();
                calculateFileHash(fileUrl);
            }
        } else {
        }
    }
}

void MainWindow::handleResults(const QString &result)
{
    qDebug() << result;
    plainTextEditOutput->appendPlainText(result);
}


void MainWindow::trigerMenu(QAction *act)
{
    if(act->text() == "SHA256")
    {
        if (act->isChecked()) {
            hashAlgorithmList.append(QCryptographicHash::Sha256);
        } else {
            hashAlgorithmList.removeAll(QCryptographicHash::Sha256);
        }
    }
    else if (act->text() == "SHA512")
    {
        if (act->isChecked()) {
            hashAlgorithmList.append(QCryptographicHash::Sha512);
        } else {
            hashAlgorithmList.removeAll(QCryptographicHash::Sha512);
        }
    }
    else if (act->text() == "MD5")
    {
        if (act->isChecked()) {
            hashAlgorithmList.append(QCryptographicHash::Md5);
        } else {
            hashAlgorithmList.removeAll(QCryptographicHash::Md5);
        }
    }
    else if (act->text() == "上部")
    {
        dropArea->clear();
    }
    else if (act->text() == "下部")
    {
        plainTextEditOutput->clear();
    }
    else if (act->text() == "全部")
    {
        dropArea->clear();
        plainTextEditOutput->clear();
    }
}
