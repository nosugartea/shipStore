#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// private slots:
//     void setComboBoxIssue();
//     void setComboBoxCarNum();
//     void setComboBoxEmployee();

//     void onComboBoxSearchCurrentIssue4TextChanged(const QString &text);
//     void onComboBoxSearchCurrentIssue6TextChanged(const QString &text);
//     void onComboBoxSearchCurrentTextChanged(const QString &text);
//     void onComboBoxAddCurrentTextChanged(const QString &text);

//     void onComboBoxCurrentTextChangedCar12(const QString &text);
//     void onComboBoxCurrentTextChangedCar13(const QString &text);
//     void onComboBoxCurrentTextChangedEmployee13(const QString &text);
//     void onComboBoxCurrentTextChangedIssue13(const QString &text);

//     void onMyButtonClickClear7();
//     void onMyButtonClickClear8();
//     void onMyButtonClickClear12();
//     void onMyButtonClickClear13();
//     void onMyButtonClickClear14();

//     // search requests
//     void onMyButtonClickSearch_OwnerByCarId();
//     void onMyButtonClickSearch_CarByOwner();
//     void onMyButtonClickSearch_IsseuByOwner();
//     void onMyButtonClickSearch_EmployeeByOwnerIssue();
//     void onMyButtonClickSearch_CarByEmployee();
//     void onMyButtonClickSearch_OwnerByIssue();

//     // change request
//     void onMyButtonClickAddClient();
//     void onMyButtonClickRemoveEmployee();
//     void onMyButtonClickChangeCarCode();

//     void onMyButtonClickAddCar();
//     void onMyButtonClickAddRepair();
//     void onMyButtonClickAddEmployee();

//     // reports
//     void onMyButtonClickMakeReportIssue();
//     void onMyButtonClickMakeReportStation();

private:
     Ui::MainWindow *ui;

//     QString warnEmpty = "Обязательное поле!!";
//     QString separator = "___________________________________________________________________________________________________________________________________________________";
//     QString issue_4 = "";
//     QString issue_6 = "";
//     QString carNum_12 = "";
//     QString issue_13 = "";
//     QString employee_13 = "";
//     QString carNum_13 = "";
};
#endif // MAINWINDOW_H
