#include "server.h"

Server::Server(){
   nextBlockSize =0;
}

Server::~Server(){
     qDebug()<<"Stop Server";
     close();
}

void Server::ServerStart(){
    if(this->listen(QHostAddress::Any,3333)){
        qDebug()<<"Start Server";
    }
    else{
          qDebug()<<"Server error";
    }

}



void Server::incomingConnection(qintptr socketDescriptor){
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,&QTcpSocket::readyRead, this, &Server::SlotReadyRead);
    connect(socket,&QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    sockets.push_back(socket);
    qDebug()<<"Clients connected, him descriptor: "<<socketDescriptor<<"\n";
}


void Server::SlotReadyRead(){
    CheckUser check = CheckUser::undeclare;
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    QByteArray ByteArr;
    bool LoginsPushBack = false;
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        for(;;){
            if(nextBlockSize==0){
                if(socket->bytesAvailable()<2){
                    qDebug()<<"Data <2 break;";
                    break;
                }
                in>>nextBlockSize;
            }
          if(socket->bytesAvailable()<nextBlockSize){
                qDebug()<<"Data not full";
              break;
          }
           in>>ByteArr;
           nextBlockSize =0;
           break;
        }
        if(ByteArr.contains("It`sMyLogin:")){
            QString str = "It`sMyLogin:";
            auto index = ByteArr.indexOf(str.toStdString());
            QByteArray login;
            for(uint8_t i = index+str.size();i<ByteArr.size() && ByteArr[i]!=' ';i++){
                login.push_back(ByteArr[i]);
            }
            QVector<QPair<quint16,QByteArray>>::iterator it  = std::find_if(logins.begin(),logins.end(),[login](QPair<quint16,QByteArray>& temp ){
               return temp.second.contains(login);
            });
            if(it!=logins.end()){
                 check = CheckUser::Foundet;
            }
            else {
                if(check == CheckUser::undeclare){
                      logins.push_back(QPair<quint16,QByteArray>(socket->socketDescriptor(),login));
                       check = CheckUser::unFounden;
                       LoginsPushBack = true;
                }
            }
        }
        if(ByteArr.contains("It`sMyPass:")){
            QString str = "It`sMyPass:";
            auto index = ByteArr.indexOf(str.toStdString());
            QByteArray pass;
            for(uint8_t i = index+str.size();i<ByteArr.size();i++){
                pass.push_back(ByteArr[i]);
            }
            if(pass!=Password){
                 check = CheckUser::Foundet;
            }
            else {
                if(check==CheckUser::undeclare)  check = CheckUser::unFounden;
            }
        }
        if(ByteArr.contains("PassServer:")){
            QString str = "PassServer:";
            auto index = ByteArr.indexOf(str.toStdString());
            for(uint8_t i = index+str.size();i<ByteArr.size() && ByteArr[i]!=' ';i++){
                Password.push_back(ByteArr[i]);
            }
        }
        if(check == CheckUser::Foundet){
            if(LoginsPushBack) logins.pop_back();
            SendToOneClient("ServerMess:Bad",socket);
        }
        else if(check == CheckUser::unFounden){
            SendToOneClient("ServerMess:Good",socket);
        }
        else if(ByteArr.contains("ListUserRequest")){
            QJsonArray jsonarray;
             foreach(auto& temp, logins){
                   QJsonValue Val(QString::fromUtf8(temp.second));
                   jsonarray.append(Val);
               }
             QJsonObject objetofinal;
             objetofinal["Users"]=jsonarray;
             QJsonDocument jasonsavedoc(objetofinal);
             SendToClient("It`sJSONFiles:"+jasonsavedoc.toJson());
        }
        else if(ByteArr.contains("EquationRequest:")){
            if(!ArrVectorLine->isEmpty()){
                for(auto start = ArrVectorLine->begin();start!=ArrVectorLine->end();start++){
                    start->clear();
                }
               ArrVectorLine->clear();
            }
            if(!ArrVectorInitialValue->isEmpty()){
                for(auto start = ArrVectorInitialValue->begin();start!=ArrVectorInitialValue->end();start++){
                    start->clear();
                }
                ArrVectorInitialValue->clear();
            }
            if(! ArrVectorAproximation->isEmpty()){
                 ArrVectorAproximation->clear();
            }
            QString str = "EquationRequest:";
            auto index = ByteArr.indexOf(str.toStdString());
            ByteArr.remove(index,str.size());
            QJsonDocument jasonsavedoc = QJsonDocument::fromJson(ByteArr);
            foreach(auto& tempArr, jasonsavedoc.object().keys()){
                 QJsonArray tempArrJson;
                 QVector<QString> StrArr;
                if(tempArr.contains("Equation")){
                    tempArrJson =jasonsavedoc.object().value(tempArr).toArray();
                    foreach(auto& value, tempArrJson){
                        StrArr.push_back(value.toString());
                    }
                    ArrVectorLine->push_back(StrArr);
                     StrArr.clear();
                }
                if(tempArr.contains("ApproximationValue")){
                    QString tempStr;
                    tempStr = jasonsavedoc.object().value(tempArr).toString();
                    ArrVectorAproximation->push_back(tempStr);
                }
                if(tempArr.contains("InitialValue")){
                    tempArrJson =jasonsavedoc.object().value(tempArr).toArray();
                    foreach(auto& value, tempArrJson){
                        StrArr.push_back(value.toString());
                    }
                    ArrVectorInitialValue->push_back(StrArr);
                    StrArr.clear();
                }
            }
            SendToClient("EquationRequest:"+jasonsavedoc.toJson());
        }
        else if(ByteArr.contains("!Resultat!")){
            QString str = "!Resultat!";
            double ScoreUser;
            auto SubScore =0;
            QString IndexEquationStr = QString::fromUtf8(ByteArr.mid(0+str.size(), (ByteArr.indexOf(" ")-str.size())));
            QString ResultUsers = QString::fromUtf8(ByteArr.mid(ByteArr.indexOf(" "),  (ByteArr.size() - ByteArr.indexOf(" "))));
            auto IndexEquation = IndexEquationStr.toInt();
            std::vector<std::string> Equation;
            double* StartValue =  new double[ArrVectorLine->at(IndexEquation).size()];
            Methods* method;
            double* result;
            foreach(auto& value, ArrVectorLine->at(IndexEquation)){
                DeleteSpaceInStr(value);
                Equation.push_back(value.toStdString());
            }
            for(auto j=0;j<ArrVectorInitialValue->at(IndexEquation).size();j++){
                StartValue[j] = ArrVectorInitialValue->at(IndexEquation).at(j).toDouble();
            }
           method = new MNewtona(Equation,ArrVectorAproximation->at(IndexEquation).toDouble(),StartValue);
           result = method->solve();
           DeleteSpaceInStr(ResultUsers);
           for(auto i=0;i<ArrVectorInitialValue->at(IndexEquation).size();i++){
               QString findStr = "x"+QString::number(i+1);
               if(i+1<=ArrVectorInitialValue->at(IndexEquation).size())
               {
                   QString NextfindStr = "x"+QString::number(i+2);
                   auto indexX = ResultUsers.indexOf(findStr);
                   auto indexNextX = ResultUsers.indexOf(NextfindStr);
                   auto resultatUser = ResultUsers.mid(indexX+findStr.size()+1,(indexNextX-(indexX+findStr.size()+1)));
                   auto res = std::abs(result[i]-resultatUser.toDouble());
                   if(std::abs(result[i]-resultatUser.toDouble())<=0.001 || std::abs(result[i]-resultatUser.toDouble())==0){
                       SubScore+=5;
                   }
                   else  if(std::abs(result[i]-resultatUser.toDouble())<=0.01){
                       SubScore+=4;
                   }
                   else  if(std::abs(result[i]-resultatUser.toDouble())<=0.1){
                       SubScore+=3;
                   }
                   else{
                         SubScore+=2;
                   }
               }
               else{
                    auto indexX = ResultUsers.indexOf(findStr);
                    auto resultatUser = ResultUsers.mid(indexX+findStr.size()+1,(ResultUsers.size() - (indexX+findStr.size()+1)));
                    if(std::abs(result[i]-resultatUser.toDouble())<=0.001 || std::abs(result[i]-resultatUser.toDouble())==0){
                        SubScore+=5;
                    }
                    else  if(std::abs(result[i]-resultatUser.toDouble())<=0.01){
                        SubScore+=4;
                    }
                    else  if(std::abs(result[i]-resultatUser.toDouble())<=0.1){
                        SubScore+=3;
                    }
                    else{
                          SubScore+=2;
                    }
               }
           }
           ScoreUser = (double)SubScore/ ArrVectorLine->at(IndexEquation).size();
            auto desk = socket->socketDescriptor();
           QVector<QPair<quint16,QByteArray>>::iterator iterLogins  = std::find_if(logins.begin(),logins.end(),[desk](QPair<quint16,QByteArray>& temp ){
              return temp.first==desk;
           });
           auto login = iterLogins->second;;
           QVector<QPair<QByteArray,double>>::iterator it  = std::find_if(ResultUser.begin(),ResultUser.end(),[login](QPair<QByteArray,double>& temp ){
              return temp.first.contains(login);
           });
           if(it!=ResultUser.end()){
               it->second +=ScoreUser;
           }
           else{
               QPair<QByteArray,double> temp(login,ScoreUser);
               ResultUser.push_back(temp);
           }
        }
        else if(ByteArr.contains("!StopTests!")){
            QJsonArray jsonarray;
            QJsonObject objetofinal;
            for(auto i=0;i<ResultUser.size();i++){
                ResultUser[i].second = ResultUser[i].second/ArrVectorLine->size();
                QJsonValue Val(ResultUser[i].second);
                objetofinal[ResultUser[i].first]=Val;
            }
             QJsonDocument jasonsavedoc(objetofinal);
             ResultUser.clear();
             SendToClient("Rating:"+jasonsavedoc.toJson());
        }
        else{
            SendToClient(ByteArr);
        }
    }
  else{
         qDebug()<<"Erorr server send";
    }
}

void Server::DeleteSpaceInStr(QString input)
{
    input.erase(std::remove(input.begin(),input.end(),' '),input.end());
}

void Server::SendToClient(QByteArray ByteArr){
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint16(0)<<ByteArr;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));

    for(int i=0;i<sockets.size();i++){
        sockets[i]->write(Data);
    }
}

void Server::SendToOneClient(QByteArray ByteArr, QTcpSocket *socet)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint16(0)<<ByteArr;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof(quint16));
    socet->write(Data);
}




















