#include "packcreate.h"
#include "ui_packcreate.h"

PackCreate::PackCreate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PackCreate)
{
    winDark::setDark_Titlebar(reinterpret_cast<HWND>(winId()));
    ui->setupUi(this);

    QLocale lo(QLocale::C);

    lo.setNumberOptions(QLocale::RejectGroupSeparator);
    amountValidator = new QDoubleValidator(parent);
    amountValidator->setDecimals(6);
    amountValidator->setLocale(lo);
    ui->ApproximateLine->setValidator(amountValidator);
    ChangeEquationLayot();
}

PackCreate::~PackCreate()
{
    delete ui;
}

void PackCreate::on_spinUknownBox_valueChanged(int arg1)
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
}

void PackCreate::ChangeEquationLayot()
{
    auto NumericStrEquation = ui->spinUknownBox->value();
    for(auto i=0;i<NumericStrEquation;i++){
        auto *Equationline = new QLineEdit();
        auto *InitialValueLine = new QLineEdit();
        InitialValueLine->setMaximumWidth(80);
        InitialValueLine->setMaxLength(8);
        Equationline->setMaxLength(20);
        InitialValueLine->setValidator(amountValidator);
        ArrLineEdit->push_back(Equationline);
        ArrApproximationEdit->push_back(InitialValueLine);
    }
    for(auto i=0;i<NumericStrEquation;i++){
        auto * label = new   QLabel();
        auto* ApproxLabel = new QLabel();
        label->setText("x"+QString().number(i+1)+" " +"=");
        ApproxLabel->setText("x"+QString().number(i+1)+"_0 " +"=");
        ui->EquationLayout->addRow(label,ArrLineEdit->at(i));
        ui->ApproximationLayout->addRow(ApproxLabel,ArrApproximationEdit->at(i));
    }
}


void PackCreate::on_AddButton_clicked()
{
    bool AllEquationFill = true;
    bool AllinitialValuesFill = true;
    foreach(auto& temp, *ArrLineEdit){
        if(temp->text()=="")  AllEquationFill = false;
    }
    foreach(auto& temp, *ArrApproximationEdit){
        if(temp->text()=="")  AllinitialValuesFill = false;
    }
    if(!AllEquationFill || !AllinitialValuesFill || ui->ApproximateLine->text()==""){
        QMessageBox(QMessageBox::Critical, tr("Error"), tr("Fill in all the fields"), QMessageBox::Ok).exec();
        return;
    }
    else{
        std::vector<std::string> Equation;
        double* StartValue =  new double[ui->spinUknownBox->value()];

        foreach(auto& value, *ArrLineEdit){
            Equation.push_back(value->text().toStdString());
        }
        for(uint8_t i=0;i<ArrApproximationEdit->size();i++){
            StartValue[i] = ArrApproximationEdit->at(i)->text().toDouble();
        }
        method = new MNewtona(Equation,ui->ApproximateLine->text().toDouble(), StartValue);
        if(method->solve()!=NULL){
            QVector<QString>* tempVector = new QVector<QString>();
            foreach(auto& value, *ArrLineEdit){
                tempVector->push_back(value->text());
            }
            ArrVectorLine->push_back(*tempVector);
            tempVector->clear();
            foreach(auto& value, *ArrApproximationEdit){
                tempVector->push_back(value->text());
            }
            ArrVectorInitialValue->push_back(*tempVector);
            tempVector->clear();
            delete tempVector;
            ArrVectorAproximation->push_back(ui->ApproximateLine->text());
        }
        else{
            QMessageBox(QMessageBox::Critical, tr("Error"), tr("Uncorrect data"), QMessageBox::Ok).exec();
            return;
        }

    }
    on_BackButton_clicked();
}


void PackCreate::on_SaveButton_clicked()
{
    if(ArrVectorLine->empty()) {
        QMessageBox(QMessageBox::Critical, tr("Error"), tr("Add at least  equation!"), QMessageBox::Ok).exec();
        return;
    }
     QString filePath=QFileDialog::getSaveFileName(this,tr("Save file"),"C:\\Users\\User\\Desktop","JSON(*.json)" );
     if(!filePath.isEmpty())
     {
         QJsonObject objetofinal;
         for(auto i=0;i<ArrVectorLine->size();i++){
             QJsonArray jsonarrayEquationLine, jsonarrayInitialValue;
             foreach(auto& value, ArrVectorLine->at(i)){
                 QJsonValue Val(value);
                 jsonarrayEquationLine.append(Val);
             }
              objetofinal[QString::fromStdString("Equation"+std::to_string(i+1))]=jsonarrayEquationLine;
              foreach(auto& value, ArrVectorInitialValue->at(i)){
                  QJsonValue Val(value);
                  jsonarrayInitialValue.append(Val);
              }
              objetofinal[QString::fromStdString("InitialValue"+std::to_string(i+1))]=jsonarrayInitialValue;
              objetofinal[QString::fromStdString("ApproximationValue"+std::to_string(i+1))]=ArrVectorAproximation->at(i);
         }
         QJsonDocument jasonsavedoc(objetofinal);
         QFile fileJson(filePath);
         fileJson.open(QIODevice::WriteOnly | QIODevice::NewOnly);
         fileJson.write(jasonsavedoc.toJson());
         fileJson.close();
     }
     else{
         QMessageBox(QMessageBox::Critical, tr("Error"), tr("You must specify a directory"), QMessageBox::Ok).exec();
         return;
     }
}


void PackCreate::on_BackButton_clicked()
{
    for(auto i=0;i<ArrLineEdit->size();i++){
       ArrLineEdit->at(i)->clear();
    }
    for(auto i=0;i<ArrApproximationEdit->size();i++){
       ArrApproximationEdit->at(i)->clear();
    }
    ui->ApproximateLine->clear();
}


void PackCreate::on_ExitButton_clicked()
{
    reject();
}

