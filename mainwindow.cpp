#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui( new Ui::MainWindow ),
    nam( Q_NULLPTR )
{
    ui->setupUi( this );

    // initialize nam
    nam = new QNetworkAccessManager();
    connect( nam, &QNetworkAccessManager::finished, this, &MainWindow::onNamReply );

    // polling timer
    QTimer *ticker_timer = new QTimer( this );
    connect( ticker_timer, &QTimer::timeout, this, &MainWindow::onShouldPollTicker );
    ticker_timer->setTimerType( Qt::CoarseTimer );
    ticker_timer->start( 60000 );
    onShouldPollTicker();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete nam;
}

void MainWindow::onShouldPollTicker()
{
    nam->get( QNetworkRequest( QUrl( "https://api.gdax.com/products/BTC-USD/ticker" ) ) );
}

void MainWindow::onNamReply( QNetworkReply *reply )
{
    QJsonDocument json_data = QJsonDocument::fromJson( reply->readAll() );
    bool is_object = json_data.isObject();

    if ( !is_object ) // ensure result is object
    {
        qDebug() << "local error: http response was not jsonable";
        reply->deleteLater(); // cleanup
        return;
    }

    // parse document into object
    QJsonObject json_obj = json_data.object();

    // parse string->double so we can format the number
    double bid = json_obj["bid"].toString().toDouble();
    double ask = json_obj["ask"].toString().toDouble();

    // print formatted numbers
    ui->labelBid->setText( "bid: " + QString::number( bid, 'f', 2 ) );
    ui->labelAsk->setText( "ask: " + QString::number( ask, 'f', 2 ) );

    // cleanup
    reply->deleteLater();
}
