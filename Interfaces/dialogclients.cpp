#include "dialogclients.h"
#include "../ui_dialogclients.h"

DialogClients::DialogClients(QWidget *parent,bool isCreateLobby) :
    QDialog(parent),
    ui(new Ui::DialogClients)
{
    ui->setupUi(this);
    if(isCreateLobby){
        ui->label_3->setVisible(false);
        ui->IpAddressLine->setVisible(false);
    }
    else{
        ui->CreateLobby->setText(tr("Sign in lobby"));
    }
}

DialogClients::~DialogClients()
{
    delete ui;
}

void DialogClients::on_CreateLobby_clicked()
{
    if(ui->LoginLine->text().size()<30 && ui->PasswordLine->text().size()<30
            && ui->LoginLine->text().size()!=0 && ui->PasswordLine->text().size()!=0 && !ui->IpAddressLine->isVisible()){
           emit signalCreateLobby(ui->LoginLine->text(),ui->PasswordLine->text(),ui->IpAddressLine->text());
    }
    else if(ui->IpAddressLine->isVisible() && ui->LoginLine->text().size()<30 && ui->PasswordLine->text().size()<30
            && ui->LoginLine->text().size()!=0 && ui->PasswordLine->text().size()!=0
            && ui->IpAddressLine->text().size()!=0 && ui->IpAddressLine->text().size()<30){
           emit signalCreateLobby(ui->LoginLine->text(),ui->PasswordLine->text(),ui->IpAddressLine->text());
    }

}


void DialogClients::on_ExitButton_clicked()
{
    reject();
}

