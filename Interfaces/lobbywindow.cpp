#include "Interfaces/lobbywindow.h"
#include "ui_lobbywindow.h"


LobbyWindow::LobbyWindow(QWidget *parent,QString login, QString pass, QString IPAddress, QMainWindow* main) :
    QDialog(parent),
    ui(new Ui::LobbyWindow)
{

    isServer = false;
    isStartClick = false;
    isReturnPressed = false;
    ui->setupUi(this);
    winDark::setDark_Titlebar(reinterpret_cast<HWND>(winId()));
    ui->ChatMenuFrame->setMaximumWidth(0);
    ui->ListUserFrame->setMaximumWidth(0);
    socket = new QTcpSocket(this);
    ui->InstallPackButtom->setVisible(false);
    if(IPAddress.size()==0){
        isServer = true;
    }
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(socket,&QTcpSocket::readyRead, this, &LobbyWindow::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(this,SIGNAL(isSuccesCreate(QString)),main,SLOT(SucessfullLobbyData(QString)));
    connect(this,SIGNAL(reject()),main,SLOT(CloseLobby()));
    Login = login;
    Password = pass;
    IpAddress = IPAddress;
    nextBlockSize =0;
    ui->stackedWidget->setVisible(false);
    ui->StartStopTest->setVisible(false);
}

LobbyWindow::~LobbyWindow()
{
    socket->disconnect();
    emit CloseLobby();
    delete ui;
}


void LobbyWindow::SendToServer(QByteArray ByteArr){
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    ByteArr.push_front(']');
   for(int i=Login.size()-1;i>=0;i--){
      ByteArr.push_front(Login[i].toLatin1());
   }
    ByteArr.push_front('[');
    out<<quint16(0)<<ByteArr;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));
    socket->write(Data);
    ui->MessageLine->clear();
}

void LobbyWindow::SendToServerSendMessServer(QByteArray ByteArr)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint16(0)<<ByteArr;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));
    socket->write(Data);
}

void LobbyWindow::slotReadyRead()
{
    QDataStream in(socket);
    QByteArray arr;
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        for(;;){
            if(nextBlockSize==0){
                if(socket->bytesAvailable()<2){
                    break;
                }
                in>>nextBlockSize;
            }
          if(socket->bytesAvailable()<nextBlockSize){
              break;
          }
          in>>arr;
          if(!arr.contains("ServerMess:Good") && !arr.contains("ServerMess:Bad")
                  && !arr.contains("It`sJSONFiles:") && !arr.contains("EquationRequest:")
                  && !arr.contains("!StartTests!") && !arr.contains("!StopTests!") && !arr.contains("Rating:")) ui->textBrowser->append(QString::fromUtf8(arr));
          nextBlockSize =0;
          break;
        }
        if(arr.contains("ServerMess:Good")){
             SendToServerSendMessServer("ListUserRequest");
             emit isSuccesCreate("good");
        }
        if(arr.contains("ServerMess:Bad")) {
              emit isSuccesCreate("bad");
        }
        if(arr.contains("It`sJSONFiles:")){
            QString str = "It`sJSONFiles:";
            auto index = arr.indexOf(str.toStdString());
            arr =  arr.remove(index,str.size());
            QJsonDocument doc = QJsonDocument::fromJson(arr,&docError);
            QJsonArray arrJson = doc.object().value("Users").toArray();
            QLayoutItem* item;
            while ( ( item =  ui->ListPageLayout->takeAt( 0 ) ) != NULL )
            {
                  delete item->widget();
                  delete item;
            }
            for(auto i =0;i<arrJson.count();i++){
                QLabel* tempLabel = new QLabel();
                tempLabel->setText(arrJson[i].toString());
                tempLabel->setAlignment(Qt::AlignHCenter);
                ui->ListPageLayout->addWidget(tempLabel);
                //delete tempLabel;
            }
        }
        if(arr.contains("Rating:")){
            QString str = "Rating:";
            QString strRating="";
            auto index = arr.indexOf(str.toStdString());
            arr =  arr.remove(index,str.size());
            QJsonDocument doc = QJsonDocument::fromJson(arr,&docError);
            foreach(auto key, doc.object().keys()){
                QString temp  = QString::number(doc.object().value(key).toDouble());
                strRating+=key+": "+temp+"\n";
            }
            ui->RatingLabel->setText(strRating);
            if( ui->stackedWidget->count()>0){
                for(int i = ui->stackedWidget->count(); i >= 0; i--)
                {
                    QWidget* widget = ui->stackedWidget->widget(i);
                    ui->stackedWidget->removeWidget(widget);
                    widget->deleteLater();
                }
            }
              ui->NextWidgetButt->setEnabled(false);
              ui->BackWidgetButt->setEnabled(false);
        }
        if(arr.contains("EquationRequest:")){
            if(!isServer) ui->stackedWidget->setVisible(false);
            QString str = "EquationRequest:";
            auto index = arr.indexOf(str.toStdString());
            arr =  arr.remove(index,str.size());
            QJsonDocument doc = QJsonDocument::fromJson(arr,&docError);
            if( ui->stackedWidget->count()>0){
                for(int i = ui->stackedWidget->count(); i >= 0; i--)
                {
                    QWidget* widget = ui->stackedWidget->widget(i);
                    ui->stackedWidget->removeWidget(widget);
                    widget->deleteLater();
                }
            }
            foreach(auto& tempArr, doc.object().keys()){
                 QJsonArray tempArrJson;
                 QVector<QString> StrArr;
                 if(tempArr.contains("Equation")){
                     tempArrJson =doc.object().value(tempArr).toArray();
                     foreach(auto& value, tempArrJson){
                         StrArr.push_back(value.toString());
                     }
                     ArrVectorLine->push_back(StrArr);
                      StrArr.clear();
                 }
                 if(tempArr.contains("ApproximationValue")){
                     QString tempStr;
                     tempStr = doc.object().value(tempArr).toString();
                     ArrVectorAproximation->push_back(tempStr);
                 }
                 if(tempArr.contains("InitialValue")){
                     tempArrJson =doc.object().value(tempArr).toArray();
                     foreach(auto& value, tempArrJson){
                         StrArr.push_back(value.toString());
                     }
                     ArrVectorInitialValue->push_back(StrArr);
                     StrArr.clear();
                 }
            }
            for(auto i=0;i<ArrVectorLine->size();i++){
                QLabel* label;
                QGridLayout* gridLayot = new QGridLayout();
                QPushButton* image = new QPushButton();
                QWidget *widget = new QWidget();
                gridLayot->setObjectName(QString::fromStdString("Equation"+std::to_string(i+1)));
                image->setText("");
                image->setObjectName("Image"+QString::number(i+1));
                image->setStyleSheet("#Image"+QString::number(i+1)+"{background-color: rgb(50, 50, 50); border: none; icon:url(:/Lobby/Icons/BigCurlyBrace.svg);}");
                image->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding));
                gridLayot->addWidget(image,1,1,ArrVectorLine->at(i).size(),1);
                image->setIconSize(QSize(70,30+(ArrVectorLine->at(i).size()*30)));
                for(auto j =0;j<ArrVectorLine->at(i).size();j++ ){
                    label = new QLabel();
                    label->setText(ArrVectorLine->at(i).at(j));
                    gridLayot->addWidget(label,(j+1),2);
                }
                for(auto j =0;j<ArrVectorInitialValue->at(i).size();j++ ){
                    label = new QLabel();
                    label->setText("x"+QString::number(i+1)+"_0 = "+ArrVectorInitialValue->at(i).at(j));
                    label->setAlignment(Qt::AlignCenter);
                    gridLayot->addWidget(label,(j+1),4);
                }
                label= new QLabel();
                label->setText("eps < "+ArrVectorAproximation->at(i));
                label->setAlignment(Qt::AlignCenter);
                gridLayot->addWidget(label,1,3);
                label= new QLabel();
                label->setText(tr("Equation №")+QString::number(i+1));
                label->setAlignment(Qt::AlignCenter);
                gridLayot->addWidget(label,0,1,1,2);
                label= new QLabel();
                label->setText(tr("Approximation:"));
                label->setAlignment(Qt::AlignCenter);
                gridLayot->addWidget(label,0,3);
                label= new QLabel();
                label->setText(tr("Initial value:"));
                label->setAlignment(Qt::AlignCenter);
                gridLayot->addWidget(label,0,4);
                gridLayot->setContentsMargins(0,0,0,0);
                widget->setLayout(gridLayot);
                ui->stackedWidget->addWidget(widget);
            }

        }
        if(arr.contains("!StartTests!")){
            ui->stackedWidget->setVisible(true);
            ui->NextWidgetButt->setEnabled(true);
        }
    }
}

bool LobbyWindow::ConnectToServer()
{
    if(isServer){
         socket->connectToHost("127.0.0.1",3333);
         ui->InstallPackButtom->setVisible(true);
         ui->stackedWidget->setVisible(true);
         ui->StartStopTest->setVisible(true);
         return true;
    }
    else{
          socket->connectToHost(IpAddress,3333);
          if(!socket->waitForConnected(300))
          {// Error
              return false;
          }
          else return true;
    }
}

bool LobbyWindow::isConnect()
{
    if(socket->state()==QTcpSocket::ConnectingState || socket->state()==QTcpSocket::ConnectedState){
        return true;
    }
    return false;
}

void LobbyWindow::on_SendMessageButton_clicked()
{
    SendToServer(ui->MessageLine->text().toUtf8());
}


void LobbyWindow::on_MessageLine_returnPressed()
{
     SendToServer(ui->MessageLine->text().toUtf8());
     isReturnPressed = true;
}


void LobbyWindow::on_ChatOpenButton_clicked()
{
    if(!isReturnPressed){
        QPropertyAnimation *PanelAnimation = new QPropertyAnimation(ui->ChatMenuFrame,"maximumWidth");
        PanelAnimation->setDuration(1500);
        if(ui->ChatMenuFrame->width()==0){
            PanelAnimation->setEasingCurve(QEasingCurve::OutExpo);
            PanelAnimation->setEndValue(200);
            PanelAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else{
             PanelAnimation->setEasingCurve(QEasingCurve::OutQuint);
             PanelAnimation->setEndValue(0);
            PanelAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else isReturnPressed = false;
}


void LobbyWindow::on_ListUserOpenButtom_clicked()
{
    if(!isReturnPressed){
        QPropertyAnimation *PanelAnimation = new QPropertyAnimation(ui->ListUserFrame,"maximumWidth");
        PanelAnimation->setDuration(1500);
        if(ui->ListUserFrame->width()==0){
            PanelAnimation->setEasingCurve(QEasingCurve::OutExpo);
            PanelAnimation->setEndValue(250);
            PanelAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else{
             PanelAnimation->setEasingCurve(QEasingCurve::OutQuint);
             PanelAnimation->setEndValue(0);
            PanelAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else isReturnPressed = false;
}




void LobbyWindow::on_ListStack_currentChanged(int arg1)
{
    if(arg1==0){
        ui->NameList->setText(tr("List users"));
        ui->ListStack->setCurrentIndex(0);
    }
    else{
        ui->NameList->setText(tr("Rating"));
         ui->ListStack->setCurrentIndex(1);
    }
}


void LobbyWindow::on_LeftPageList_clicked()
{
    if(ui->ListStack->currentIndex()==1){
        ui->RightPageList->setEnabled(true);
        on_ListStack_currentChanged(0);
        ui->LeftPageList->setEnabled(false);
    }
}


void LobbyWindow::on_RightPageList_clicked()
{
    if(ui->ListStack->currentIndex()==0){
           on_ListStack_currentChanged(1);
           ui->LeftPageList->setEnabled(true);
           ui->RightPageList->setEnabled(false);
    }
}


void LobbyWindow::on_InstallPackButtom_clicked()
{
    QString filter = "*.json";
    QString pathJson;
    QFile file;
    QJsonDocument docJson;
    pathJson  = QFileDialog::getOpenFileName(this, tr("Select pack"), QDir::homePath(), filter);
    file.setFileName(pathJson);
    if(file.open(QIODevice::ReadOnly|QFile::Text)){
        docJson = QJsonDocument::fromJson(file.readAll(),&docError);
    }
    ui->RatingLabel->clear();
    SendToServerSendMessServer("EquationRequest:"+docJson.toJson());
}




void LobbyWindow::on_BackWidgetButt_clicked()
{
    if(ui->stackedWidget->currentIndex()>=1 && ui->stackedWidget->currentIndex()<= ui->stackedWidget->currentIndex()<=ui->stackedWidget->count()-1){
       if(ui->stackedWidget->currentIndex()==1)   ui->BackWidgetButt->setEnabled(false);
       if(ui->stackedWidget->currentIndex()==ui->stackedWidget->count()-1)  ui->NextWidgetButt->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
    }
}


void LobbyWindow::on_NextWidgetButt_clicked()
{

    if(ui->stackedWidget->currentIndex()>=0 && ui->stackedWidget->currentIndex()<=ui->stackedWidget->count()-2){
           if(ui->stackedWidget->currentIndex()==0) ui->BackWidgetButt->setEnabled(true);
           if(ui->stackedWidget->currentIndex()==ui->stackedWidget->count()-2) ui->NextWidgetButt->setEnabled(false);
           ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
    }
}


void LobbyWindow::on_StartStopTest_clicked()
{
    if(ui->stackedWidget->count()==0){
        QMessageBox(QMessageBox::Critical, tr("Error"), "Download the pack", QMessageBox::Yes).exec();
        return;
    }
    if(!isStartClick){
        ui->StartStopTest->setStyleSheet("#StartStopTest{icon:url(:/Lobby/Icons/StopServer.svg); } #StartStopTest:hover{icon:url(:/Lobby/Icons/StopServer_Hover.svg); }  ");
        isStartClick = true;
        SendToServerSendMessServer("!StartTests!");
    }
    else{
         ui->StartStopTest->setStyleSheet("#StartStopTest{icon:url(:/Lobby/Icons/StartTest.svg); } #StartStopTest:hover{icon:url(:/Lobby/Icons/StartTest_Hover.svg); }  ");
        isStartClick = false;
        SendToServerSendMessServer("!StopTests!");
    }
}


void LobbyWindow::on_AnswerLine_returnPressed()
{
    if(ui->AnswerLine->text().size()!=0){
        QString  str = "!Resultat!"+QString::number(ui->stackedWidget->currentIndex())+" "+ui->AnswerLine->text();
        ui->AnswerLine->clear();
        SendToServerSendMessServer(str.toUtf8());
    }
     isReturnPressed = true;
}


void LobbyWindow::on_AnswerButtom_clicked()
{
    if(ui->AnswerLine->text().size()!=0){
        QString  str = "!Resultat!"+QString::number(ui->stackedWidget->currentIndex())+" "+ui->AnswerLine->text();
        ui->AnswerLine->clear();
        SendToServerSendMessServer(str.toUtf8());
    }
}

