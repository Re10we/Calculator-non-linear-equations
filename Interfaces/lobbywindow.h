#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QDialog>
#include "Server/server.h"
#include "WinDark/winDark.h"
#include <QPropertyAnimation>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QFormLayout>
#include <QWidget>
namespace Ui {
class LobbyWindow;
}

class LobbyWindow : public QDialog
{
    Q_OBJECT


public:
    explicit LobbyWindow(QWidget *parent = nullptr,QString login="", QString password="", QString IPAddress="",QMainWindow * main = nullptr);
    ~LobbyWindow();
    QString IpAddress;
private:
    Ui::LobbyWindow *ui;
    QString Login;
    QString Password;
    bool isServer;
    bool isReturnPressed;
    QTcpSocket *socket;
    QByteArray Data;
    quint16 nextBlockSize;
    QVector<QGridLayout*>* GridLayot = new QVector<QGridLayout*>();
    bool isStartClick;
    ///

    QJsonParseError docError;
    ///
    QVector<QVector<QString>>* ArrVectorLine = new  QVector<QVector<QString>>();
    QVector<QVector<QString>>* ArrVectorInitialValue= new  QVector<QVector<QString>>();
    QVector<QString>* ArrVectorAproximation = new QVector<QString>();
    ///
signals:
    void CloseLobby();
    void isSuccesCreate(QString);
    void reject();
public slots:
    void SendToServer(QByteArray);
    void SendToServerSendMessServer(QByteArray);
    void slotReadyRead();
    bool ConnectToServer();
    bool isConnect();
private slots:
    void on_SendMessageButton_clicked();
    void on_MessageLine_returnPressed();
    void on_ChatOpenButton_clicked();
    void on_ListUserOpenButtom_clicked();
    void on_ListStack_currentChanged(int arg1);
    void on_LeftPageList_clicked();
    void on_RightPageList_clicked();
    void on_InstallPackButtom_clicked();
    void on_BackWidgetButt_clicked();
    void on_NextWidgetButt_clicked();
    void on_StartStopTest_clicked();
    void on_AnswerLine_returnPressed();
    void on_AnswerButtom_clicked();
};

#endif // LOBBYWINDOW_H
