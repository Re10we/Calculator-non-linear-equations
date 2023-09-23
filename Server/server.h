#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QPair>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonArray>
#include <QtCore/QFile>
#include "../Methods/HFiles/MNewtona.h"
#include "../Methods/HFiles/Methods.h"

class Server: public QTcpServer{

    Q_OBJECT
public:
    Server();
    ~Server();
    QTcpSocket* socket;
    void ServerStart();
private:
    enum CheckUser{
        unFounden,
        Foundet,
        undeclare
    };
    QVector<QTcpSocket*> sockets;
    QByteArray Data;
    QByteArray Password;
    quint16 nextBlockSize;
    QVector<QPair<quint16,QByteArray>> logins;
    QVector<QPair<QByteArray,double>> ResultUser;
    ///JSON
    QJsonDocument Jsondoc;
    QJsonArray JsondocArr;
    QJsonParseError JsondocError;
    ///
    QVector<QVector<QString>>* ArrVectorLine = new  QVector<QVector<QString>>();
    QVector<QVector<QString>>* ArrVectorInitialValue= new  QVector<QVector<QString>>();
    QVector<QString>* ArrVectorAproximation = new QVector<QString>();
    ///
    void SendToClient(QByteArray);
    void SendToOneClient(QByteArray,QTcpSocket*);


public slots:
    void incomingConnection(qintptr socketDescriptor);
    void SlotReadyRead();
    void DeleteSpaceInStr(QString);
};


#endif // SERVER_H
