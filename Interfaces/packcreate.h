#ifndef PACKCREATE_H
#define PACKCREATE_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include <QtGui/QDoubleValidator>
#include "../Methods/HFiles/Methods.h"
#include "../Methods/HFiles/MNewtona.h"

namespace Ui {
class PackCreate;
}

class PackCreate : public QDialog
{
//   Q_OBJECT

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
