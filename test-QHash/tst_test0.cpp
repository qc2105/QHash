#include <QtTest>
#include <QCoreApplication>
#include <QString>

// add necessary includes here
#include "../hasher.h"

class Hasher;

class test0 : public QObject
{
    Q_OBJECT

public:
    test0();
    ~test0();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_calculateFileHash();
    void test_calculateFileHashEmptyFile();

private:
    Hasher * hasher;

};

test0::test0()
{

}

test0::~test0()
{

}

void test0::initTestCase()
{

}

void test0::cleanupTestCase()
{

}

void test0::test_calculateFileHash()
{
    system("echo 'hack Hack' > test.txt");
    hasher = new Hasher;
    hasher->doWork("test.txt", QCryptographicHash::Sha256);
    QCOMPARE(hasher->getResult(), QString("390d68d988742fe014e1e88f43d256113a898806ed4cf38b84c1d19b69f380b7"));
    system("rm test.txt");
}

void test0::test_calculateFileHashEmptyFile()
{
    system("echo '' > test.txt");
    hasher = new Hasher;
    hasher->doWork("test.txt", QCryptographicHash::Sha256);
    QCOMPARE(hasher->getResult(), QString("01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b"));
    system("rm test.txt");
}

QTEST_MAIN(test0)

#include "tst_test0.moc"
