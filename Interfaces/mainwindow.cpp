#include "mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->LeftMenu->setMaximumWidth(0);
    ClickPanel = false;

    QLocale lo(QLocale::C);

    lo.setNumberOptions(QLocale::RejectGroupSeparator);
    amountValidator = new QDoubleValidator(parent);
    amountValidator->setDecimals(6);
    amountValidator->setLocale(lo);
    ui->AccuracyLine->setValidator(amountValidator);

    ui->GraphicsWidget->hide();
    ui->NameGraphLabel->hide();


    ui->BroydenArea->setMaximumHeight(0);
    ui->IterationArea->setMaximumHeight(0);
    ui->NwetonArea->setMaximumHeight(0);
    ui->SeidelArea->setMaximumHeight(0);


    labelTime = new QLabel();
    labelLang = new QLabel();
    labelLang->setText(tr("Current lang: ")+"Eng");
    time = new QTimer();
    connect(time, SIGNAL(timeout()), this, SLOT(updateTime()));
    time->setInterval(1000);
    statusBar()->setStyleSheet("background-color: rgb(50,50,50);  color: white; font-family: 'Georgia'; font-weight: bold;font-size: 14px; ");
    statusBar()->addWidget(labelTime);
    statusBar()->addWidget(labelLang);
    time->start();
    ChangeEquationLayot();
}




MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_MainMenuButton_clicked()
{
    QPropertyAnimation *PanelAnimation = new QPropertyAnimation(ui->LeftMenu,"maximumWidth");
    PanelAnimation->setDuration(1500);
    if(!ClickPanel){
        //StartWidthPanel = ui->LeftMenu->width();
        PanelAnimation->setEasingCurve(QEasingCurve::OutExpo);
        PanelAnimation->setEndValue(201);
        PanelAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        ui->MainMenuButton->setStyleSheet("image: url(:/Icons/Icons/MenuOpened.svg);");
        ClickPanel = true;
    }
    else{
         PanelAnimation->setEasingCurve(QEasingCurve::OutQuint);
         PanelAnimation->setEndValue(0);
        PanelAnimation->start(QAbstractAnimation::DeleteWhenStopped);
          ui->MainMenuButton->setStyleSheet("image: url(:/Icons/Icons/MenuClose.svg);");

        ClickPanel = false;
    }

}

void MainWindow::on_CalculatorButton_clicked()
{
    //Fade in
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->CalculatorButton->setGraphicsEffect(eff);
    ui->CalculatorButton->setStyleSheet("#CalculatorButton{ background-color: rgb(198, 66, 0);} #CalculatorButton:hover{background-color:rgb(198, 66, 0);}");
    ui->LobbyButton->setStyleSheet("#LobbyButton{ background-color: rgb(50, 50, 50);} #LobbyButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->SettingsButton->setStyleSheet("#SettingsButton{ background-color: rgb(50, 50, 50);} #SettingsButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->TheoryButton->setStyleSheet("#TheoryButton{ background-color: rgb(50, 50, 50);} #TheoryButton:hover{background-color: rgba(255, 85, 0,70);}");
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(250);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_TheoryButton_clicked()
{
    //Fade in
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->TheoryButton->setGraphicsEffect(eff);
    ui->CalculatorButton->setStyleSheet("#CalculatorButton{ background-color: rgb(50, 50, 50);} #CalculatorButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->LobbyButton->setStyleSheet("#LobbyButton{ background-color: rgb(50, 50, 50);} #LobbyButton:hover{background-color: rgba(255, 85, 0,70);;}");
    ui->SettingsButton->setStyleSheet("#SettingsButton{ background-color: rgb(50, 50, 50);} #SettingsButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->TheoryButton->setStyleSheet("#TheoryButton{ background-color: rgb(198, 66, 0);} #TheoryButton:hover{background-color: rgb(198, 66, 0);}");
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(250);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    ui->stackedWidget->setCurrentIndex(1);

    QFile TextMNewtona(".\\Files\\TextMNewtona.txt"),  TextMIteration(".\\Files\\TextMIteration.txt"),
            TextMSeidel(".\\Files\\TextMSeidel.txt"), TextMBroyden(".\\Files\\TextMBroyden.txt");
    QString TextsMNewtona="", TextsMIteration="",TextsMSeidel="", TextsMBroyden="";
    QTextStream in;
    if (!TextMNewtona.open(QIODevice::ReadOnly) || !TextMIteration.open(QIODevice::ReadOnly) ||
            !TextMSeidel.open(QIODevice::ReadOnly) || !TextMBroyden.open(QIODevice::ReadOnly)) {
        QMessageBox(QMessageBox::Critical, tr("Error"), tr("Error open file"), QMessageBox::Yes).exec();
        return;
    }
    else{
        in.setDevice(&TextMNewtona);
        while (!in.atEnd()) {
          QString line = in.readLine();
          TextsMNewtona+=line;
        }
         in.setDevice(&TextMIteration);
         while (!in.atEnd()) {
           QString line = in.readLine();
           TextsMIteration+=line;
         }
         in.setDevice(&TextMSeidel);
         while (!in.atEnd()) {
           QString line = in.readLine();
           TextsMSeidel+=line;
         }
         in.setDevice(&TextMBroyden);
         while (!in.atEnd()) {
           QString line = in.readLine();
           TextsMBroyden+=line;
         }
        TextMNewtona.close();
        TextMIteration.close();
        TextMSeidel.close();
        TextMBroyden.close();
    }
    ui->TextMNewtona->setText(TextsMNewtona);
    ui->TextMIteration->setText(TextsMIteration);
    ui->TextMZeidela->setText(TextsMSeidel);
    ui->TextMBroyden->setText(TextsMBroyden);
}

void MainWindow::on_LobbyButton_clicked()
{
    //Fade in
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->LobbyButton->setGraphicsEffect(eff);
    ui->CalculatorButton->setStyleSheet("#CalculatorButton{ background-color: rgb(50, 50, 50);} #CalculatorButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->LobbyButton->setStyleSheet("#LobbyButton{ background-color: rgb(198, 66, 0);} #LobbyButton:hover{background-color: rgb(198, 66, 0);}");
    ui->SettingsButton->setStyleSheet("#SettingsButton{ background-color: rgb(50, 50, 50);} #SettingsButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->TheoryButton->setStyleSheet("#TheoryButton{ background-color: rgb(50, 50, 50);} #TheoryButton:hover{background-color: rgba(255, 85, 0,70);}");
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(250);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_SettingsButton_clicked()
{
    //Fade in
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    ui->SettingsButton->setGraphicsEffect(eff);
    ui->CalculatorButton->setStyleSheet("#CalculatorButton{ background-color: rgb(50, 50, 50);} #CalculatorButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->LobbyButton->setStyleSheet("#LobbyButton{ background-color: rgb(50, 50, 50);} #LobbyButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->TheoryButton->setStyleSheet("#TheoryButton{ background-color: rgb(50, 50, 50);} #TheoryButton:hover{background-color: rgba(255, 85, 0,70);}");
    ui->SettingsButton->setStyleSheet("#SettingsButton{ background-color:  rgb(198, 66, 0);} #SettingsButton:hover{background-color: rgb(198, 66, 0);}");
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(250);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_OpenClsButtonMNewtona_clicked()
{
    QPropertyAnimation *Animation = new QPropertyAnimation(ui->NwetonArea,"maximumHeight");
    Animation->setDuration(1500);
    if(ui->NwetonArea->maximumHeight()>0){
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(0);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);


        ui->OpenClsButtonMNewtona->setStyleSheet("#OpenClsButtonMNewtona{ icon: url(:/Icons/Icons/OpenCloseImage.svg);} #OpenClsButtonMNewtona:hover{icon: url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

    }
    else {
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(500);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        ui->OpenClsButtonMNewtona->setStyleSheet("#OpenClsButtonMNewtona{ icon: url(:/Icons/Icons/OpenCloseImageOn.svg);} #OpenClsButtonMNewtona:hover{icon: url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
    }
}


void MainWindow::on_OpenClsButtonMZeidela_clicked()
{
    QPropertyAnimation *Animation = new QPropertyAnimation(ui->SeidelArea,"maximumHeight");
    Animation->setDuration(1500);
    if(ui->SeidelArea->maximumHeight()>0){
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(0);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);

        ui->OpenClsButtonMZeidela->setStyleSheet("#OpenClsButtonMZeidela{ icon: url(:/Icons/Icons/OpenCloseImage.svg);} #OpenClsButtonMZeidela:hover{icon: url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

    }
    else {
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(500);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        ui->OpenClsButtonMZeidela->setStyleSheet("#OpenClsButtonMZeidela{ icon: url(:/Icons/Icons/OpenCloseImageOn.svg);} #OpenClsButtonMZeidela:hover{icon: url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
    }
}


void MainWindow::on_OpenClsButtonMItaration_clicked()
{
    QPropertyAnimation *Animation = new QPropertyAnimation(ui->IterationArea,"maximumHeight");
    Animation->setDuration(1500);
    if(ui->IterationArea->maximumHeight()>0){
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(0);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);

        ui->MItarationWidget->setMinimumHeight(0);
        ui->OpenClsButtonMItaration->setStyleSheet("#OpenClsButtonMItaration{ icon: url(:/Icons/Icons/OpenCloseImage.svg);} #OpenClsButtonMItaration:hover{icon: url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

    }
    else {
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(500);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
         ui->MItarationWidget->setMinimumHeight(240);
        ui->OpenClsButtonMItaration->setStyleSheet("#OpenClsButtonMItaration{ icon: url(:/Icons/Icons/OpenCloseImageOn.svg);} #OpenClsButtonMItaration:hover{icon: url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
    }
}

void MainWindow::on_OpenClsButtonMBroyden_clicked()
{
    QPropertyAnimation *Animation = new QPropertyAnimation(ui->BroydenArea,"maximumHeight");
    Animation->setDuration(1500);
    if(ui->BroydenArea->maximumHeight()>0){
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(0);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        ui->BroydenArea->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding));
        ui->OpenClsButtonMBroyden->setStyleSheet("#OpenClsButtonMBroyden{ icon: url(:/Icons/Icons/OpenCloseImage.svg);} #OpenClsButtonMItaration:hover{icon: url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

    }
    else {
        Animation->setEasingCurve(QEasingCurve::OutExpo);
        Animation->setEndValue(500);
        Animation->start(QAbstractAnimation::DeleteWhenStopped);
        ui->BroydenArea->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed));
        ui->OpenClsButtonMBroyden->setStyleSheet("#OpenClsButtonMBroyden{ icon: url(:/Icons/Icons/OpenCloseImageOn.svg);} #OpenClsButtonMItaration:hover{icon: url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
    }
}


void MainWindow::ChangeEquationLayot(){
    int NumericStrEquation = ui->SpinUknownBox->value();
    for(int i=0;i<NumericStrEquation;i++){
        QLineEdit *line = new QLineEdit();
        QLineEdit *Approxline = new QLineEdit();
        Approxline->setMaximumWidth(80);
        Approxline->setMaxLength(8);
        line->setMaxLength(20);
        Approxline->setValidator(amountValidator);
        ArrLineEdit->push_back(line);
        ArrApproximationEdit->push_back(Approxline);
    }
    for(int i=0;i<NumericStrEquation;i++){
        QLabel * label = new   QLabel();
        QLabel* ApproxLabel = new QLabel();
        label->setText("x"+QString().number(i+1)+" " +"=");
        ApproxLabel->setText("x"+QString().number(i+1)+"_0 " +"=");
        ui->EquationLayout->addRow(label,ArrLineEdit->at(i));
        ui->ApproximationLayout->addRow(ApproxLabel,ArrApproximationEdit->at(i));
    }

}

void MainWindow::on_SpinUknownBox_valueChanged()
{
           QLayoutItem* item;
         while ( ( item =  ui->EquationLayout->takeAt( 0 ) ) != NULL )
         {
             delete item->widget();
             delete item;
         }
         while( ( item =  ui->ApproximationLayout->takeAt( 0 ) ) != NULL){
             delete item->widget();
             delete item;
         }
     ArrLineEdit->clear();
     ArrApproximationEdit->clear();
     ChangeEquationLayot();
     ui->ResultStrLabel->clear();
}




void MainWindow::on_ShowResultButton_clicked()
{
   ui->GraphicsWidget->hide();
   ui->NameGraphLabel->hide();
   ui->ResultStrLabel->clear();
    std::string str = "\nResult: ";
    std::vector<std::string> Equation;
    double* StartValue =  new double[ui->SpinUknownBox->value()];
    for(uint8_t i=0;i<ArrLineEdit->size();i++){
        Equation.push_back(ArrLineEdit->at(i)->text().toStdString());
    }
    for(uint8_t i=0;i<ArrApproximationEdit->size();i++){
        StartValue[i] = ArrApproximationEdit->at(i)->text().toDouble();
    }
    Methods* method;
     double* result;

    if(ui->MethodsBox->currentIndex()==0){
        method = new MNewtona(Equation,ui->AccuracyLine->text().toDouble(),StartValue);
        result = method->solve();
    }
    if(ui->MethodsBox->currentIndex()==3){
        method = new MIteration(Equation,ui->AccuracyLine->text().toDouble(),StartValue);
        result = method->solve();
    }
    if(ui->MethodsBox->currentIndex()==1){
        method = new MBroyden(Equation,ui->AccuracyLine->text().toDouble(),StartValue);
        result = method->solve();
    }
    if(ui->MethodsBox->currentIndex()==2){
        method = new MSeidel(Equation,ui->AccuracyLine->text().toDouble(),StartValue);
        result = method->solve();
    }
    if(result==NULL){
        QMessageBox(QMessageBox::Critical, tr("Error"), tr("Invalid enter equation or initial value"), QMessageBox::Yes).exec();
        return;
    }
    for(uint8_t i=0;i<Equation.size();i++){
        str+= "\nx"+std::to_string(i+1)+" = "+std::to_string(result[i])+";";
    }
    if(Equation.size()==2){
        ui->GraphicsWidget->xAxis->setRange(*std::min_element(method->X.constBegin(), method->X.constEnd()),*std::max_element(method->X.constBegin(), method->X.constEnd()));
        ui->GraphicsWidget->yAxis->setRange(*std::min_element(method->Y.constBegin(), method->Y.constEnd()),*std::max_element(method->Y.constBegin(), method->Y.constEnd()));
        ui->GraphicsWidget->addGraph();
        ui->GraphicsWidget->graph(0)->addData(method->X,method->Y);
        ui->GraphicsWidget->replot();
        ui->GraphicsWidget->show();
       ui->NameGraphLabel->show();
    }
    method->StrResult +=str;
    QString strLabel = QString::fromStdString(method->StrResult);
    ui->ResultStrLabel->setText(strLabel);
}







void MainWindow::on_CreateLobbyButton_clicked()
{
    clientsDialog = new DialogClients();
    connect(clientsDialog, &DialogClients::signalCreateLobby,this, &MainWindow::CreateLobby);
    clientsDialog->setWindowTitle(tr("Registration lobby"));
    clientsDialog->show();
}


void MainWindow::on_CreatePackButton_clicked()
{
    packCreate = new PackCreate();
    packCreate->setWindowTitle(tr("Create pack"));
    if(packCreate->exec()==QDialog::Accepted){
        delete packCreate;
    }
}


void MainWindow::on_SignInLobbyButton_clicked()
{
    clientsDialog = new DialogClients(this,false);
    connect(clientsDialog, &DialogClients::signalCreateLobby,this, &MainWindow::CreateLobby);
    clientsDialog->setWindowTitle(tr("Authorization in lobby"));
    clientsDialog->show();
}

void MainWindow::CreateLobby(QString LoginStr, QString PasswordStr,QString IPAddress)
{
    lobby = new LobbyWindow(nullptr,LoginStr,PasswordStr,IPAddress,this);
    if(IPAddress.size()==0){
        serv = new Server();
        serv->ServerStart();
    }

    if(!lobby->ConnectToServer()){
        QMessageBox(QMessageBox::Critical, tr("Error"), tr("No such server exists \nPlease check the server address you entered"), QMessageBox::Yes).exec();
        return;
    }
    if(IPAddress.size()==0){
        lobby->SendToServerSendMessServer(("PassServer:"+PasswordStr+" It`sMyLogin:"+LoginStr).toUtf8());
    }
    else   lobby->SendToServerSendMessServer(("It`sMyLogin:"+LoginStr+" It`sMyPass:"+PasswordStr).toUtf8());
}

void MainWindow::SucessfullLobbyData(QString str)
{
   if(str=="good"){
       this->hide();
       delete clientsDialog;
       lobby->show();
   }
   else{
       QMessageBox(QMessageBox::Critical, tr("Error"), tr("Invalid login or password"), QMessageBox::Yes).exec();
       return;
   }
}

void MainWindow::CloseLobby()
{
    if(lobby->IpAddress.size()==0) delete serv;
     delete lobby;
    this->show();
}

void MainWindow::updateTime()
{

    auto lang = QGuiApplication::inputMethod()->locale();
    labelTime->setText(tr("Current time: ")+QTime::currentTime().toString());
    if(lang.language() == QLocale::Ukrainian){

         labelLang->setText(labelLang->text().remove(labelLang->text().size()-3,3)+"Ukr");
    }
    if(lang.language() == QLocale::Russian){
         labelLang->setText(labelLang->text().remove(labelLang->text().size()-3,3)+"Rus");
    }
    if(lang.language() == QLocale::English){
        labelLang->setText(labelLang->text().remove(labelLang->text().size()-3,3)+"Eng");
    }
}



void MainWindow::on_AboutProgramButt_clicked()
{
    AboutProgram* aboutProgram = new AboutProgram();
    aboutProgram->setWindowTitle(tr("About program"));
    if(aboutProgram->exec()==QDialog::Accepted){
        delete aboutProgram;
    }
}


void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::LanguageChange){
        ui->retranslateUi(this);
    }
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1=="English" || arg1=="Англійська"){
        auto isSuccessfullLoad = translate.load(":/Translate/QtLanguage_en.qm");
        if(isSuccessfullLoad) qApp->installTranslator(&translate);
    }
    else{
         auto isSuccessfullLoad = translate.load(":/Translate/QtLanguage_ukr.qm");
         if(isSuccessfullLoad) qApp->installTranslator(&translate);
    }
}


void MainWindow::on_PrintButton_clicked()
{
     QString filePath=QFileDialog::getSaveFileName(this,tr("Save file"),"C:\\Users\\User\\Desktop","DOC(*.doc)" );
     if(!filePath.isEmpty())
     {
         QFile file(filePath);
         QByteArray text;
         if(!file.open(QFile::WriteOnly | QFile::Text)){
                 QMessageBox::warning(this,"title","Out Not Open");
             }
         else{
             QTextStream out(&file);
              if(ui->TextMBroyden->height()>0){
                  text.push_back(ui->TextMBroyden->text().toUtf8());
              }
              if(ui->TextMZeidela->height()>0){
                  text.push_back(ui->TextMZeidela->text().toUtf8());
              }
              if(ui->TextMNewtona->height()>0){
                  text.push_back(ui->TextMNewtona->text().toUtf8());
              }
              if(ui->TextMIteration->height()>0){
                  text.push_back(ui->TextMIteration->text().toUtf8());
              }
              out<<text;
              file.flush();
              file.close();
         }
     }
     else{
         QMessageBox(QMessageBox::Critical, tr("Error"), tr("You must specify a directory"), QMessageBox::Ok).exec();
         return;
     }
}


void MainWindow::on_HelpButt_clicked()
{
    helpWindow* helpwindow = new helpWindow();
    helpwindow->setWindowTitle(tr("Help window"));
    if(helpwindow->exec()==QDialog::Accepted){
        delete helpwindow;
    }
}




