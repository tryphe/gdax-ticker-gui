#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    void onShouldPollTicker();
    void onNamReply( QNetworkReply *reply );

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *nam;
};

#endif // MAINWINDOW_H
