#ifndef PACKCREATE_H
#define PACKCREATE_H

#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QDoubleValidator>
#include "Methods/HFiles/Methods.h"
#include "Methods/HFiles/MNewtona.h"
#include "WinDark/winDark.h"

namespace Ui {
class PackCreate;
}

class PackCreate : public QDialog
{
    Q_OBJECT

public:
    explicit PackCreate(QWidget *parent = nullptr);
    ~PackCreate();

private slots:
    void ChangeEquationLayot();

    void on_AddButton_clicked();

    void on_SaveButton_clicked();

    void on_BackButton_clicked();

    void on_ExitButton_clicked();

    void on_spinUknownBox_valueChanged(int arg1);


private:
    Ui::PackCreate *ui;
    QVector<QLineEdit*>* ArrLineEdit = new QVector<QLineEdit*>();
    QVector<QLineEdit*>* ArrApproximationEdit = new QVector<QLineEdit*>();
    QVector<QVector<QString>>* ArrVectorLine = new  QVector<QVector<QString>>();
    QVector<QVector<QString>>* ArrVectorInitialValue= new  QVector<QVector<QString>>();
    QVector<QString>* ArrVectorAproximation = new QVector<QString>();
    QDoubleValidator* amountValidator;
    Methods* method;
};

#endif // PACKCREATE_H
