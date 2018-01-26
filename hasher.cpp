#include "hasher.h"
#include <QFile>
#include <QFileInfo>
#include <QtGlobal>
#include <QDebug>
#include <QUrl>

Hasher::Hasher(QObject *parent) : QObject(parent)
{

}

void Hasher::doWork(QString fileUrl, QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile in(fileUrl);    qDebug() << fileUrl;
    QFileInfo fileInfo(fileUrl);
    qint64 fileSize = fileInfo.size();    qDebug() << "filesize: " << fileSize;
    const int bufferSize = 10000;
    QCryptographicHash hash(hashAlgorithm);
    if (in.open(QIODevice::ReadOnly)) {
        char buf[bufferSize];
        int bytesRead;
        int readSize = qMin<qint64>(fileSize, bufferSize);
        while (readSize > 0 && (bytesRead = in.read(buf, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buf, bytesRead);
            readSize = qMin<qint64>(fileSize, bufferSize);
        }
        in.close();
        result = hash.result().toHex();        qDebug() << result;
        emit resultReady(result);
    } else {    qDebug() << "Failed to open device!";
    }
}

QString Hasher::getResult()
{
    return result;
}
