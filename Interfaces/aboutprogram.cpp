#include "aboutprogram.h"
#include "ui_aboutprogram.h"

AboutProgram::AboutProgram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutProgram)
{
    winDark::setDark_Titlebar(reinterpret_cast<HWND>(winId()));
    ui->setupUi(this);
}

AboutProgram::~AboutProgram()
{
    delete ui;
}
