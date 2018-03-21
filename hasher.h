#ifndef HASHER_H
#define HASHER_H

#include <QObject>
#include <QCryptographicHash>

class Hasher : public QObject
{
    Q_OBJECT
public:
    explicit Hasher(QObject *parent = NULL);

signals:
    void resultReady(const QString &result);

public slots:
     void doWork(int fileID, QString fileUrl, QCryptographicHash::Algorithm hashAlgorithm);
     QString getResult();

private:
     QString result;
};

#endif // HASHER_H
