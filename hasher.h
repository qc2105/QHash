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
     void doWork(QString fileUrl, QCryptographicHash::Algorithm hashAlgorithm);
     QString getResult();

private:
     QString result;
     int id;
};

#endif // HASHER_H
