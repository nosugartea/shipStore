#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "loginDialog.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QComboBox>

#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    LoginDialog *loginDialog;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin();
    void setTables();

    void onComboBoxEI(const QString &text);
    void onComboBoxClass(const QString &text);
    void onComboBoxProd(const QString &text);
    void onComboBoxEnum(const QString &text);
    void onComboBoxParam(const QString &text);

    void setComboBoxEI();
    void setComboBoxClass();
    void setComboBoxProd();
    void setComboBoxEnum();
    void setComboBoxVal();
    void setComboBoxParam();

    // EI
    void onMyButtonClickAdd_1();
    void onMyButtonClickDelete_2();
    void onMyButtonClickSearch_3();

    // CHEM_CLASS
    void onMyButtonClickAdd_4();
    void onMyButtonClickDelete_5();
    void onMyButtonClickChange_6();
    void onMyButtonClickClass_7();
    void onMyButtonClickMainClass_7();

    void onComboBox4EI(const QString &text);
    void onComboBox4MainClass(const QString &text);
    void onComboBox5ClassId(const QString &text);
    void onComboBox6NameClass(const QString &text);
    void onComboBox6MainClass(const QString &text);
    void onComboBox7Class(const QString &text);
    void onComboBox7MainClass(const QString &text);

    // PROD
    void onMyButtonClickAdd_8();
    void onMyButtonClickDelete_9();
    void onMyButtonClickSearchClass_12();
    void onMyButtonClickSearchAll_12();

    void onComboBox8ClassId(const QString &text);
    void onComboBox9ProdId(const QString &text);
    void onComboBox12ClassName(const QString &text);

    // ENUM
    void onMyButtonClickAdd_13();
    void onMyButtonClickDeleteEnum_14();
    void onMyButtonClickDeleteVal_14();
    void onMyButtonClickDown_17();
    void onMyButtonClickUp_17();
    void onMyButtonClickSearch_18();

    void onComboBox13EnumId(const QString &text);
    void onComboBox14EnumId(const QString &text);
    void onComboBox14ValId(const QString &text);
    void onComboBox17Down(const QString &text);
    void onComboBox17Up(const QString &text);
    void onComboBox18EnumId(const QString &text);

    // PARAM
    void onMyButtonClickAdd_16();
    void onMyButtonClickAdd_19();
    void onMyButtonClickAdd_23();
    void onMyButtonClickCopy_25();
    void onMyButtonClickProd_26();
    void onMyButtonClickSearch_26();
    void onMyButtonClickSearch_28();

    void onComboBox16ClassId(const QString &text);
    void onComboBox16ParamId(const QString &text);
    void onComboBox19EI(const QString &text);
    void onComboBox23EnumId(const QString &text);
    void onComboBox23ParamId(const QString &text);
    void onComboBox23ProdId(const QString &text);
    void onComboBox25MainClass(const QString &text);
    void onComboBox25ClassId(const QString &text);
    void onComboBox26Prod(const QString &text);
    void onComboBox26Class(const QString &text);
    void onComboBox28Param(const QString &text);

    // clear buttons
    void onClear4();
    void onClear8();

private:
    QString warnEmpty = "Обязательное поле!!";
    QString idEI_4;
    QString mainClass_4;
    QString classId_5;
    QString classId_6;
    QString mainClass_6;
    QString classId_7;
    QString mainClass_7;
    QString classId_8;
    QString prodId_9;
    QString classId_12;
    QString idEnum_13;
    QString idEnum_14;
    QString idVal_14;
    QString idDown_17;
    QString idUp_17;
    QString idEnum_18;

    QString classId_16;
    QString paramId_16;
    QString idEI_19;
    QString idEnum_23;
    QString prodId_23;
    QString paramId_23;
    QString classId_25;
    QString mainClass_25;
    QString classId_26;
    QString prodId_26;
    QString paramId_28;
};
#endif // MAINWINDOW_H
