#ifndef DIALOGCLIENTS_H
#define DIALOGCLIENTS_H

#include <QtWidgets/QDialog>

namespace Ui {
class DialogClients;
}

class DialogClients : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClients(QWidget *parent = nullptr, bool isCreateLobby =true);
    ~DialogClients();

private slots:
    void on_CreateLobby_clicked();

    void on_ExitButton_clicked();

signals:
    void signalCreateLobby(QString,QString,QString);

private:
    Ui::DialogClients *ui;
};

#endif // DIALOGCLIENTS_H
