#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMimeData>
#include <QCryptographicHash>
#include <QThread>

class DropArea;
class Hasher;
class QPlainTextEdit;
class QFile;

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    QThread workerThread;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void operate(const QString &fileUrl, const QCryptographicHash::Algorithm hashAlgorithm);

public slots:
    void getFileHash(const QMimeData *mimeData);
    void handleResults(const QString &);

private:
    DropArea *dropArea;
    Hasher *hasher;
    QPlainTextEdit *plainTextEditOutput;
};

#endif // MAINWINDOW_H
