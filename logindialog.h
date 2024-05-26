#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString getUsername() const;
    QString getPassword() const;

signals:
    void loginButtonClicked();

private slots:
    void onButtonBoxAccepted();

private:
    Ui::LoginDialog *ui;
    QString username;
    QString password;
};

#endif // LOGINDIALOG_H
