#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMimeData>
#include <QCryptographicHash>
#include <QThread>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>


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

    QMenu* menu[10];
    QAction* act[10];
    QMenuBar* menuBar;

signals:
    void operate(int fileID, const QString &fileUrl, const QCryptographicHash::Algorithm hashAlgorithm);

public slots:
    void getFileHash(const QMimeData *mimeData);
    void handleResults(const QString &);
    void trigerMenu(QAction* act);


private:
    DropArea *dropArea;
    Hasher *hasher;
    QCryptographicHash::Algorithm defaultHashAlgorithm;
    QList<QCryptographicHash::Algorithm> hashAlgorithmList;
    QPlainTextEdit *plainTextEditOutput;
};

#endif // MAINWINDOW_H
