#include "hasher.h"
#include <QFile>
#include <QFileInfo>
#include <QtGlobal>
#include <QDebug>
#include <QUrl>

Hasher::Hasher(QObject *parent) : QObject(parent)
{
}

void Hasher::doWork(int fileID, QString fileUrl, QCryptographicHash::Algorithm hashAlgorithm)
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
        QString algorithmString;
        if (hashAlgorithm == QCryptographicHash::Sha256) {
            algorithmString = QString("Sha256");
        } else if (hashAlgorithm == QCryptographicHash::Sha512) {
            algorithmString = QString("Sha512");
        } else if (hashAlgorithm == QCryptographicHash::Md5) {
            algorithmString = QString("Md5");
        }
        result = QString::number(fileID) + QString(":") + QString("[") + fileUrl + QString("]") + QString("-") + QString("[") + algorithmString + QString("]") + QString(": ");
        result += hash.result().toHex();
        result += QLatin1Char('\n');     qDebug() << result;
        emit resultReady(result);
    } else {    qDebug() << "Failed to open device!";
    }
}

QString Hasher::getResult()
{
    return result;
}
