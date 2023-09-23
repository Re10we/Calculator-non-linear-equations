#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QPropertyAnimation>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtCore/QFile>
#include <QtCore/QVector>
#include <QtWidgets/QLineEdit>
#include "../Parser/SimpleParser.h"
#include "../Methods/HFiles/Methods.h"
#include "../Methods/HFiles/MNewtona.h"
#include "../Methods/HFiles/MIteration.h"
#include "../Methods/HFiles/MSeidel.h"
#include "../Methods/HFiles/MBroyden.h"
#include <vector>
#include <string.h>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QScrollBar>
#include "dialogclients.h"
#include "lobbywindow.h"
#include "packcreate.h"
#include "aboutprogram.h"
#include "helpwindow.h"
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTranslator>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QMenu>
#include <QtGui/QInputMethod>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_MainMenuButton_clicked();

    void on_CalculatorButton_clicked();

    void on_TheoryButton_clicked();

    void on_LobbyButton_clicked();

    void on_OpenClsButtonMNewtona_clicked();

    void on_OpenClsButtonMZeidela_clicked();

    void on_OpenClsButtonMItaration_clicked();

    void ChangeEquationLayot();

    void on_SpinUknownBox_valueChanged();

    void on_ShowResultButton_clicked();


    void on_SettingsButton_clicked();

    void on_CreateLobbyButton_clicked();

    void on_CreatePackButton_clicked();

    void on_SignInLobbyButton_clicked();
    void on_AboutProgramButt_clicked();
    void changeEvent(QEvent*) override;

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_PrintButton_clicked();

    void on_HelpButt_clicked();

    void on_OpenClsButtonMBroyden_clicked();

public slots:
    void CreateLobby(QString,QString,QString);
    void SucessfullLobbyData(QString);
    void CloseLobby();
    void updateTime();
private:
    Ui::MainWindow *ui;
    QTranslator translate;
    DialogClients* clientsDialog;
    bool ClickPanel;
    int StartWidthPanel;
    LobbyWindow* lobby;
    Server* serv;
    PackCreate* packCreate;
    QVector<QLineEdit*>* ArrLineEdit = new QVector<QLineEdit*>();
    QVector<QLineEdit*>* ArrApproximationEdit = new QVector<QLineEdit*>();
    QTimer* time;
    QLabel* labelTime;
    QLabel* labelLang;
    QDoubleValidator *amountValidator;
};
#endif // MAINWINDOW_H
