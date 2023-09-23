#include "helpwindow.h"
#include "../ui_helpwindow.h"

helpWindow::helpWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::helpWindow) {
  ui->setupUi(this);
  ui->scrollArea->setMaximumHeight(0);
  ui->scrollArea_2->setMaximumHeight(0);
  ui->scrollArea_3->setMaximumHeight(0);
  ui->scrollArea_4->setMaximumHeight(0);
}

helpWindow::~helpWindow() { delete ui; }

void helpWindow::on_OpneIntroductionButton_clicked() {
  QPropertyAnimation *Animation =
      new QPropertyAnimation(ui->scrollArea, "maximumHeight");
  Animation->setDuration(1500);
  if (ui->scrollArea->maximumHeight() > 0) {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(0);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpneIntroductionButton->setStyleSheet(
        "#OpneIntroductionButton{ icon: "
        "url(:/Icons/Icons/OpenCloseImage.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

  } else {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(500);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpneIntroductionButton->setStyleSheet(
        "#OpneIntroductionButton{ icon: "
        "url(:/Icons/Icons/OpenCloseImageOn.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
  }
}

void helpWindow::on_OpenCalculatorButton_clicked() {
  QPropertyAnimation *Animation =
      new QPropertyAnimation(ui->scrollArea_2, "maximumHeight");
  Animation->setDuration(1500);
  if (ui->scrollArea_2->maximumHeight() > 0) {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(0);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpenCalculatorButton->setStyleSheet(
        "#OpenCalculatorButton{ icon: url(:/Icons/Icons/OpenCloseImage.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

  } else {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(500);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpenCalculatorButton->setStyleSheet(
        "#OpenCalculatorButton{ icon: "
        "url(:/Icons/Icons/OpenCloseImageOn.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
  }
}

void helpWindow::on_OpenTheoryButton_clicked() {
  QPropertyAnimation *Animation =
      new QPropertyAnimation(ui->scrollArea_3, "maximumHeight");
  Animation->setDuration(1500);
  if (ui->scrollArea_3->maximumHeight() > 0) {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(0);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpenTheoryButton->setStyleSheet(
        "#OpenTheoryButton{ icon: url(:/Icons/Icons/OpenCloseImage.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

  } else {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(500);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpenTheoryButton->setStyleSheet(
        "#OpenTheoryButton{ icon: url(:/Icons/Icons/OpenCloseImageOn.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
  }
}

void helpWindow::on_OpenLobbyButton_clicked() {
  QPropertyAnimation *Animation =
      new QPropertyAnimation(ui->scrollArea_4, "maximumHeight");
  Animation->setDuration(1500);
  if (ui->scrollArea_4->maximumHeight() > 0) {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(0);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpenLobbyButton->setStyleSheet(
        "#OpenLobbyButton{ icon: url(:/Icons/Icons/OpenCloseImage.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImage_Hover.svg);}");

  } else {
    Animation->setEasingCurve(QEasingCurve::OutExpo);
    Animation->setEndValue(500);
    Animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->OpenLobbyButton->setStyleSheet(
        "#OpenLobbyButton{ icon: url(:/Icons/Icons/OpenCloseImageOn.svg);} "
        "#OpenClsButtonMNewtona:hover{icon: "
        "url(:/Icons/Icons/OpenCloseImageOn_Hover.svg);}");
  }
}
