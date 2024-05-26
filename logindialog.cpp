#include "loginDialog.h"
#include "ui_loginDialog.h"

#include <QPushButton>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->startpushButton, &QPushButton::clicked, this, &LoginDialog::onButtonBoxAccepted);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::getUsername() const {
    return username;
}

QString LoginDialog::getPassword() const {
    return password;
}

void LoginDialog::onButtonBoxAccepted()
{
    username = ui->usernameLineEdit->text();
    password = ui->passwordLineEdit->text();
    emit loginButtonClicked();
    close();
}
