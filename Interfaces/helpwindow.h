#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include <QPropertyAnimation>
#include "WinDark/winDark.h"

namespace Ui {
class helpWindow;
}

class helpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit helpWindow(QWidget *parent = nullptr);
    ~helpWindow();

private slots:
    void on_OpneIntroductionButton_clicked();

    void on_OpenCalculatorButton_clicked();

    void on_OpenTheoryButton_clicked();

    void on_OpenLobbyButton_clicked();

private:
    Ui::helpWindow *ui;
};

#endif // HELPWINDOW_H
