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


    menuBar = new QMenuBar(this);
    menuBar->addMenu(menu[0]);
    menuBar->setGeometry(0,0, this->width(),30);

    defaultHashAlgorithm = QCryptographicHash::Sha256;

    connect(menuBar, SIGNAL(triggered(QAction*)), this, SLOT(trigerMenu(QAction*)));



    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(menuBar);
    mainLayout->addWidget(dropArea);
    plainTextEditOutput = new QPlainTextEdit(this);
    mainLayout->addWidget(plainTextEditOutput);

    setWindowTitle(tr("Hasher"));
    setMinimumSize(500,350);
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
                if (hashAlgorithmList.size() <= 0) {
                    emit operate(i, fileUrl, defaultHashAlgorithm);
                } else {
                    QCryptographicHash::Algorithm hashAlgorithm;
                    foreach (hashAlgorithm, hashAlgorithmList) {
                        emit operate(i, fileUrl, hashAlgorithm);
                    }
                }
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

// TODO: 当选择一个action时，uncheck其他的action
void MainWindow::trigerMenu(QAction *act)
{
    if(act->text() == "SHA256")
    {
        if (act->isChecked()) {
            qDebug() << "选择sha256校验算法";
            hashAlgorithmList.append(QCryptographicHash::Sha256);
        } else {
            qDebug() << "去掉sha256校验算法";
            hashAlgorithmList.removeAll(QCryptographicHash::Sha256);
        }
    }
    else if (act->text() == "SHA512")
    {
        if (act->isChecked()) {
            qDebug() << "选择sha512校验算法";
            hashAlgorithmList.append(QCryptographicHash::Sha512);
        } else {
            qDebug() << "去掉sha512校验算法";
            hashAlgorithmList.removeAll(QCryptographicHash::Sha512);
        }
    }
    else if (act->text() == "MD5")
    {
        if (act->isChecked()) {
            qDebug() << "选择md5校验算法";
            hashAlgorithmList.append(QCryptographicHash::Md5);
        } else {
            qDebug() << "去掉md5校验算法";
            hashAlgorithmList.removeAll(QCryptographicHash::Md5);
        }
    }
}
