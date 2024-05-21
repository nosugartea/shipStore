#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("1234");
    db.setDatabaseName("CarServiceBD");

    if (db.open()) {
        QMessageBox::information(this, "Connection", "Database Connected");
    } else {
        QMessageBox::information(this, "Not Connected", "Databasw Is Not Connected");
    }

    QPalette palette;
    QColor bgColor(41, 41, 70, 255);
    QBrush bgBrush(bgColor);
    palette.setBrush(QPalette::Window, bgBrush);
    ui->centralwidget->setAutoFillBackground(true);
    ui->centralwidget->setPalette(palette);
    ui->tabWidget->setAutoFillBackground(true);
    ui->statusBar->setStyleSheet("background-color: rgba(35, 35, 51, 255); color: white;");
    
    // connect(ui->comboBox_EI, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxSearchCurrentTextChanged);
    // connect(ui->comboBox_add, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxAddCurrentTextChanged);

    // setComboBoxIssue();
    // connect(ui->comboBox_4_issue, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxSearchCurrentIssue4TextChanged);
    // connect(ui->comboBox_6_issue, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxSearchCurrentIssue6TextChanged);

    // setComboBoxCarNum();
    // connect(ui->comboBox_12_carNum, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxCurrentTextChangedCar12);

    // setComboBoxEmployee();
    // connect(ui->comboBox_13_carNum, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxCurrentTextChangedCar13);
    // connect(ui->comboBox_13_employee, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxCurrentTextChangedEmployee13);
    // connect(ui->comboBox_13_issue, &QComboBox::currentTextChanged,
    //         this, &MainWindow::onComboBoxCurrentTextChangedIssue13);

    // connect(ui->pushButton_7_clear, &QPushButton::clicked, this, &MainWindow::onMyButtonClickClear7);
    // connect(ui->pushButton_8_clear, &QPushButton::clicked, this, &MainWindow::onMyButtonClickClear8);
    // connect(ui->pushButton_12_clear, &QPushButton::clicked, this, &MainWindow::onMyButtonClickClear12);
    // connect(ui->pushButton_13_clear, &QPushButton::clicked, this, &MainWindow::onMyButtonClickClear13);
    // connect(ui->pushButton_14_clear, &QPushButton::clicked, this, &MainWindow::onMyButtonClickClear14);

    // connect(ui->pushButton_1_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_OwnerByCarId);
    // connect(ui->pushButton_2_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_CarByOwner);
    // connect(ui->pushButton_3_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_IsseuByOwner);
    // connect(ui->pushButton_4_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_EmployeeByOwnerIssue);
    // connect(ui->pushButton_5_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_CarByEmployee);
    // connect(ui->pushButton_6_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_OwnerByIssue);

    // connect(ui->pushButton_7_change, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAddClient);
    // connect(ui->pushButton_8_change, &QPushButton::clicked, this, &MainWindow::onMyButtonClickRemoveEmployee);
    // connect(ui->pushButton_9_change, &QPushButton::clicked, this, &MainWindow::onMyButtonClickChangeCarCode);

    // connect(ui->pushButton_12_change, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAddCar);
    // connect(ui->pushButton_13_change, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAddRepair);
    // connect(ui->pushButton_14_change, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAddEmployee);

    // connect(ui->pushButton_10_report, &QPushButton::clicked, this, &MainWindow::onMyButtonClickMakeReportIssue);
    // connect(ui->pushButton_11_report, &QPushButton::clicked, this, &MainWindow::onMyButtonClickMakeReportStation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::setComboBoxIssue()
// {
//     QSqlQuery query;
//     query.prepare("SELECT issue_type "
//                   "FROM issues;");
//     if (query.exec()) {
//         while (query.next()) {
//             QString value = query.value(0).toString();
//             ui->comboBox_4_issue->addItem(value);
//             ui->comboBox_6_issue->addItem(value);
//             ui->comboBox_13_issue->addItem(value);
//         }
//     }
// }

// void MainWindow::setComboBoxCarNum()
// {
//     QSqlQuery query;
//     query.prepare("SELECT registration_number "
//                   "FROM owners;");
//     if (query.exec()) {
//         while (query.next()) {
//             QString value = query.value(0).toString();
//             ui->comboBox_12_carNum->addItem(value);
//             ui->comboBox_13_carNum->addItem(value);
//         }
//     }
// }

// void MainWindow::setComboBoxEmployee()
// {
//     QSqlQuery query;
//     query.prepare("SELECT full_name "
//                   "FROM employee;");
//     if (query.exec()) {
//         while (query.next()) {
//             QString value = query.value(0).toString();
//             ui->comboBox_13_employee->addItem(value);
//         }
//     }
// }

// void MainWindow::onComboBoxSearchCurrentIssue4TextChanged(const QString &text)
// {
//     int index = ui->comboBox_4_issue->findText(text);
//     if (index != -1) {
//         issue_4 = ui->comboBox_4_issue->itemText(index);
//     }
// }

// void MainWindow::onComboBoxSearchCurrentIssue6TextChanged(const QString &text)
// {
//     int index = ui->comboBox_6_issue->findText(text);
//     if (index != -1) {
//         issue_6 = ui->comboBox_6_issue->itemText(index);
//     }
// }

// void MainWindow::onComboBoxCurrentTextChangedCar12(const QString &text)
// {
//     int index = ui->comboBox_12_carNum->findText(text);
//     if (index != -1) {
//         carNum_12 = ui->comboBox_12_carNum->itemText(index);
//         qDebug() << carNum_12;
//     }
// }

// void MainWindow::onComboBoxCurrentTextChangedCar13(const QString &text)
// {
//     int index = ui->comboBox_13_carNum->findText(text);
//     if (index != -1) {
//         carNum_13 = ui->comboBox_13_carNum->itemText(index);
//         qDebug() << carNum_13;
//     }
// }

// void MainWindow::onComboBoxCurrentTextChangedEmployee13(const QString &text)
// {
//     int index = ui->comboBox_13_employee->findText(text);
//     if (index != -1) {
//         employee_13 = ui->comboBox_13_employee->itemText(index);
//         qDebug() << employee_13;
//     }
// }

// void MainWindow::onComboBoxCurrentTextChangedIssue13(const QString &text)
// {
//     int index = ui->comboBox_13_issue->findText(text);
//     if (index != -1) {
//         issue_13 = ui->comboBox_13_issue->itemText(index);
//         qDebug() << issue_13;
//     }
// }

// void MainWindow::onMyButtonClickClear7()
// {
//     ui->lineEdit_7_name->clear();
//     ui->lineEdit_7_address->clear();
//     ui->lineEdit_7_number->clear();
//     ui->lineEdit_7_model->clear();
//     ui->lineEdit_7_carNum->clear();
// }

// void MainWindow::onMyButtonClickClear8()
// {
//     ui->lineEdit_8_name->clear();
//     ui->lineEdit_8_surName->clear();
//     ui->lineEdit_8_fatherName->clear();;
// }

// void MainWindow::onMyButtonClickClear12()
// {
//     ui->lineEdit_12_manufacturer->clear();
//     ui->lineEdit_12_model->clear();
//     ui->lineEdit_12_year->clear();;
// }

// void MainWindow::onMyButtonClickClear13()
// {
//     ui->lineEdit_13_description->clear();
//     ui->lineEdit_13_endTime->clear();
//     ui->lineEdit_13_startTime->clear();;
// }

// void MainWindow::onMyButtonClickClear14()
// {
//     ui->lineEdit_14_address->clear();
//     ui->lineEdit_14_name->clear();
//     ui->lineEdit_14_number->clear();;
// }

// void MainWindow::onComboBoxSearchCurrentTextChanged(const QString &text)
// {
//     int index = ui->comboBox_EI->findText(text);
//     if (index != -1) {
//         ui->stackedWidget_EI->setCurrentIndex(index);
//     }
// }

// void MainWindow::onComboBoxAddCurrentTextChanged(const QString &text)
// {
//     int index = ui->comboBox_add->findText(text);
//     if (index != -1) {
//         ui->stackedWidget_add->setCurrentIndex(index);
//     }
// }

// void MainWindow::onMyButtonClickSearch_OwnerByCarId()
// {
//     ui->textEdit_1_searchResult->clear();
//     QString code = ui->lineEdit_1_code->text();
//     QString region = ui->lineEdit_1_region->text();
//     QString country = ui->lineEdit_1_country->text();
//     if (code.isEmpty() || code == warnEmpty) {
//         ui->lineEdit_1_code->setText(warnEmpty);
//     }
//     if (region.isEmpty() || region == warnEmpty) {
//         ui->lineEdit_1_region->setText(warnEmpty);
//     }
//     if (country.isEmpty() || country == warnEmpty) {
//         ui->lineEdit_1_country->setText(warnEmpty);
//     }
//     if ((!code.isEmpty() && code != warnEmpty) &&
//         (!region.isEmpty() && region != warnEmpty) &&
//         (!country.isEmpty() && country != warnEmpty)){
//         QSqlQuery query;
//         code = code.toUpper() + " " + region.toUpper() + " " + country.toUpper();
//         query.prepare("SELECT full_name, address "
//                       "FROM Owners "
//                       "WHERE registration_number = :registration_number;");
//         query.bindValue(":registration_number", code);

//         if (query.exec()) {
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 QString result_out = "";
//                 for (int i = 0; i < record.count(); ++i) {
//                     result_out = record.value(i).toString();
//                 }
//                 ui->textEdit_1_searchResult->insertPlainText(result_out + "\n");
//             }
//         }
//     }
// }

// void MainWindow::onMyButtonClickSearch_CarByOwner()
// {
//     ui->textEdit_2_searchResult->clear();
//     QString surname = ui->lineEdit_2_surname->text();
//     QString name = ui->lineEdit_2_name->text();
//     QString fatherName = ui->lineEdit_2_fatherName->text();
//     if (surname.isEmpty() || surname == warnEmpty) {
//         ui->lineEdit_2_surname->setText(warnEmpty);
//     }
//     if (name.isEmpty() || name == warnEmpty) {
//         ui->lineEdit_2_name->setText(warnEmpty);
//     }

//     if ((!surname.isEmpty() && surname != warnEmpty) &&
//         (!name.isEmpty() && name != warnEmpty)){
//         QSqlQuery query;
//         if (!fatherName.isEmpty()) {
//             name = surname + " " + name + " " + fatherName;
//         } else {
//             name = surname + " " + name;
//         }
//         qDebug() << name;
//         query.prepare("SELECT Owners.full_name, Car.registration_number, Car.manufacturer, Car.model, Car.year_manufactured "
//                       "FROM Owners JOIN Car "
//                       "On Owners.registration_number = Car.registration_number "
//                       "WHERE Owners.full_name = :name;");
//         query.bindValue(":name", name);

//         if (query.exec()) {
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 QString result_out = "";
//                 for (int i = 0; i < record.count(); ++i) {
//                     result_out += record.value(i).toString() + " ";
//                 }
//                 ui->textEdit_2_searchResult->insertPlainText(result_out + "\n" + separator);
//             }
//         }
//     }
// }

// void MainWindow::onMyButtonClickSearch_IsseuByOwner()
// {
//     ui->textEdit_3_searchResult->clear();
//     QString surname = ui->lineEdit_3_surname->text();
//     QString name = ui->lineEdit_3_name->text();
//     QString fatherName = ui->lineEdit_3_fatherName->text();
//     if (surname.isEmpty() || surname == warnEmpty) {
//         ui->lineEdit_3_surname->setText(warnEmpty);
//     }
//     if (name.isEmpty() || name == warnEmpty) {
//         ui->lineEdit_3_name->setText(warnEmpty);
//     }

//     if ((!surname.isEmpty() && surname != warnEmpty) &&
//         (!name.isEmpty() && name != warnEmpty)){
//         QSqlQuery query;
//         if (!fatherName.isEmpty()) {
//             name = surname + " " + name + " " + fatherName;
//         } else {
//             name = surname + " " + name;
//         }
//         qDebug() << name;
//         query.prepare("SELECT Repairs.registration_number, Repairs.issue_type, Repairs.description "
//                       "FROM Owners JOIN Repairs "
//                       "On Owners.registration_number = Repairs.registration_number "
//                       "WHERE Owners.full_name = :name AND Repairs.status_id = 'ГОТОВО';");
//         query.bindValue(":name", name);

//         if (query.exec()) {
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 QString result_out = "";
//                 for (int i = 0; i < record.count(); ++i) {
//                     result_out += record.value(i).toString() + " ";
//                 }
//                 ui->textEdit_3_searchResult->insertPlainText(result_out + "\n" + separator);
//             }
//         }
//     }
// }

// void MainWindow::onMyButtonClickSearch_EmployeeByOwnerIssue()
// {
//     ui->textEdit_4_searchResult->clear();
//     QString surname = ui->lineEdit_4_surname->text();
//     QString name = ui->lineEdit_4_name->text();
//     QString fatherName = ui->lineEdit_4_fatherName->text();
//     if (surname.isEmpty() || surname == warnEmpty) {
//         ui->lineEdit_4_surname->setText(warnEmpty);
//     }
//     if (name.isEmpty() || name == warnEmpty) {
//         ui->lineEdit_4_name->setText(warnEmpty);
//     }

//     if ((!surname.isEmpty() && surname != warnEmpty) &&
//         (!name.isEmpty() && name != warnEmpty)){
//         QSqlQuery query;
//         if (!fatherName.isEmpty()) {
//             name = surname + " " + name + " " + fatherName;
//         } else {
//             name = surname + " " + name;
//         }
//         query.prepare("SELECT Repairs.registration_number, Repairs.issue_type, Repairs.employee_name, Repairs.repair_time "
//                       "FROM Owners JOIN Repairs "
//                       "On Owners.registration_number = Repairs.registration_number "
//                       "WHERE Owners.full_name = :name AND Repairs.status_id = 'ГОТОВО' AND  Repairs.issue_type = :type;");
//         query.bindValue(":name", name);
//         query.bindValue(":type", issue_4);

//         if (query.exec()) {
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 QString result_out = "";
//                 for (int i = 0; i < record.count(); ++i) {
//                     result_out += record.value(i).toString() + " ";
//                 }
//                 ui->textEdit_4_searchResult->insertPlainText(result_out + "\n" + separator);
//             }
//         }
//     }
// }

// void MainWindow::onMyButtonClickSearch_CarByEmployee()
// {
//     ui->textEdit_5_searchResult->clear();
//     QString surname = ui->lineEdit_5_surname->text();
//     QString name = ui->lineEdit_5_name->text();
//     QString fatherName = ui->lineEdit_5_fatherName->text();
//     if (surname.isEmpty() || surname == warnEmpty) {
//         ui->lineEdit_5_surname->setText(warnEmpty);
//     }
//     if (name.isEmpty() || name == warnEmpty) {
//         ui->lineEdit_5_name->setText(warnEmpty);
//     }

//     if ((!surname.isEmpty() && surname != warnEmpty) &&
//         (!name.isEmpty() && name != warnEmpty)){
//         if (!fatherName.isEmpty()) {
//             name = surname + " " + name + " " + fatherName;
//         } else {
//             name = surname + " " + name;
//         }
//         qDebug() << name;
//         QSqlQuery query;
//         query.prepare("SELECT Car.registration_number, Car.manufacturer, Car.model "
//                       "FROM Repairs JOIN Car "
//                       "On Repairs.registration_number = Car.registration_number "
//                       "WHERE Repairs.employee_name = :name;");
//         query.bindValue(":name", name);

//         if (query.exec()) {
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 QString result_out = "";
//                 for (int i = 0; i < record.count(); ++i) {
//                     result_out += record.value(i).toString() + " ";
//                 }
//                 ui->textEdit_5_searchResult->insertPlainText(result_out + "\n" + separator);
//             }
//         }
//     }
// }

// void MainWindow::onMyButtonClickSearch_OwnerByIssue()
// {
//     ui->textEdit_6_searchResult->clear();
//     if (!issue_6.isEmpty()) {
//         QSqlQuery query;
//         query.prepare("SELECT Owners.full_name, Owners.phone_number, Owners.registration_number, Owners.car_model "
//                       "FROM Repairs JOIN Owners "
//                       "On Repairs.registration_number = Owners.registration_number "
//                       "WHERE Repairs.issue_type = :type;");
//         query.bindValue(":type", issue_6);

//         if (query.exec()) {
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 QString result_out = "";
//                 for (int i = 0; i < record.count(); ++i) {
//                     result_out += record.value(i).toString() + " ";
//                 }
//                 ui->textEdit_6_searchResult->insertPlainText(result_out + "\n" + separator);
//             }
//         }
//     }
// }

// void MainWindow::onMyButtonClickAddClient()
// {
//     QString name = ui->lineEdit_7_name->text();
//     QString address = ui->lineEdit_7_address->text();
//     QString number = ui->lineEdit_7_number->text();
//     QString model = ui->lineEdit_7_model->text();
//     QString carNum = ui->lineEdit_7_carNum->text();

//     int flag = 1;
//     if (name.isEmpty() || name == warnEmpty) {
//          ui->lineEdit_7_name->setText(warnEmpty);
//         flag = 0;
//     }
//     if (address.isEmpty() || address == warnEmpty) {
//         ui->lineEdit_7_address->setText(warnEmpty);
//         flag = 0;
//     }
//     if (number.isEmpty() || number == warnEmpty) {
//         ui->lineEdit_7_number->setText(warnEmpty);
//         flag = 0;
//     }
//     if (model.isEmpty() || model == warnEmpty) {
//         ui->lineEdit_7_model->setText(warnEmpty);
//         flag = 0;
//     }
//     if (carNum.isEmpty() || carNum == warnEmpty) {
//         ui->lineEdit_7_carNum->setText(warnEmpty);
//         flag = 0;
//     }

//     if (flag) {
//         QSqlQuery query;
//         query.prepare("INSERT INTO owners (full_name, address, phone_number, car_model, registration_number) VALUES "
//                       "(:name, :address, :number, :model, :carNum);");
//         query.bindValue(":name", name);
//         query.bindValue(":address", address);
//         query.bindValue(":number", number);
//         query.bindValue(":model", model);
//         query.bindValue(":carNum", carNum);
//         if (query.exec()) {
//             statusBar()->showMessage("Запись успешно добавлена!", 5000);
//         } else {
//             statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//         }
//     }
// }


// void MainWindow::onMyButtonClickRemoveEmployee()
// {
//     QString name = ui->lineEdit_8_name->text();
//     QString surname = ui->lineEdit_8_surName->text();
//     QString fatherName = ui->lineEdit_8_fatherName->text();

//     int flag = 1;
//     if (name.isEmpty() || name == warnEmpty) {
//         ui->lineEdit_8_name->setText(warnEmpty);
//         flag = 0;
//     }
//     if (surname.isEmpty() || surname == warnEmpty) {
//         ui->lineEdit_8_surName->setText(warnEmpty);
//         flag = 0;
//     }

//     if (flag) {
//         if (!fatherName.isEmpty()){
//             name = surname + " " + name + " " + fatherName;
//         } else {
//             name = surname + " " + name;
//         }
//         QSqlQuery query;
//         query.prepare("SELECT COUNT(*) FROM Employee WHERE full_name = :name;");
//         query.bindValue(":name", name);
//         if (query.exec()) {
//             int result_out = 0;
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 result_out = record.value(0).toInt();
//             }
//             if (result_out == 0) {
//                 statusBar()->showMessage("Работник с введенным ФИО не найден", 5000);
//             } else {
//                 query.prepare("DELETE FROM Employee WHERE full_name = :name;");
//                 query.bindValue(":name", name);

//                 if (query.exec()) {
//                     statusBar()->showMessage("Записи успешно удалены", 5000);
//                 } else {
//                     statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//                 }
//             }
//         } else {
//             statusBar()->showMessage("Ошибка записи", 5000);
//         }
//     }
// }

// void MainWindow::onMyButtonClickChangeCarCode()
// {
//     QString oldCode = ui->lineEdit_9_carCodeOld->text();
//     QString newCode = ui->lineEdit_9_carCodeNew->text();

//     int flag = 1;
//     if (oldCode.isEmpty() || oldCode == warnEmpty) {
//         ui->lineEdit_9_carCodeOld->setText(warnEmpty);
//         flag = 0;
//     }
//     if (newCode.isEmpty() || newCode == warnEmpty) {
//         ui->lineEdit_9_carCodeNew->setText(warnEmpty);
//         flag = 0;
//     }

//     if (flag) {
//         QSqlQuery query;
//         query.prepare("SELECT COUNT(*) FROM Owners WHERE registration_number = :oldCode;");
//         query.bindValue(":oldCode", oldCode);
//         if (query.exec()) {
//             int result_out = 0;
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 result_out = record.value(0).toInt();
//             }
//             if (result_out == 0) {
//                 statusBar()->showMessage("Клиент с введенным госномером не найден", 5000);
//             } else {
//                 query.prepare("UPDATE Owners SET registration_number = :newCode WHERE registration_number = :oldCode;");
//                 query.bindValue(":oldCode", oldCode);
//                 query.bindValue(":newCode", newCode);

//                 if (query.exec()) {
//                     statusBar()->showMessage("Успешно изменено!", 5000);
//                 } else {
//                     statusBar()->showMessage("Ошибка записи", 5000);
//                 }
//             }
//         } else {
//             statusBar()->showMessage("Ошибка записи", 5000);
//         }
//     }
// }

// void MainWindow::onMyButtonClickAddCar()
// {
//     QString year = ui->lineEdit_12_year->text();
//     QString manufacturer = ui->lineEdit_12_manufacturer->text();
//     QString model = ui->lineEdit_12_model->text();

//     int flag = 1;
//     if (year.isEmpty() || year == warnEmpty) {
//         ui->lineEdit_12_year->setText(warnEmpty);
//         flag = 0;
//     }
//     if (manufacturer.isEmpty() || manufacturer == warnEmpty) {
//         ui->lineEdit_12_manufacturer->setText(warnEmpty);
//         flag = 0;
//     }
//     if (model.isEmpty() || model == warnEmpty) {
//         ui->lineEdit_12_model->setText(warnEmpty);
//         flag = 0;
//     }

//     if (flag) {
//         QSqlQuery query;
//         query.prepare("INSERT INTO car (registration_number, year_manufactured, manufacturer, model) VALUES "
//                       "(:carNum, :year, :manu, :model);");
//         query.bindValue(":carNum", carNum_12);
//         query.bindValue(":year", year.toInt());
//         query.bindValue(":manu", manufacturer);
//         query.bindValue(":model", model);
//         if (query.exec()) {
//             statusBar()->showMessage("Запись успешно добавлена!", 5000);
//         } else {
//             statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//         }
//     }
// }

// void MainWindow::onMyButtonClickAddRepair()
// {
//     QString start_time = ui->lineEdit_13_startTime->text();
//     QString end_time = ui->lineEdit_13_endTime->text();
//     QString description = ui->lineEdit_13_description->text();

//     int flag = 0;
//     if (!start_time.isEmpty() && end_time.isEmpty()) {
//         flag = 1;
//     }
//     if (start_time.isEmpty() && !end_time.isEmpty()) {
//         flag = 2;
//     }
//     if (!start_time.isEmpty() && !end_time.isEmpty()) {
//         flag = 3;
//     }
//     if (start_time.isEmpty() && end_time.isEmpty()) {
//         flag = 4;
//     }

//     if (carNum_13.isEmpty() || employee_13.isEmpty() || issue_13.isEmpty()) {
//         flag = 0;
//     }

//     QSqlQuery query;
//     switch (flag)
//     {
//     case 1:
//         query.prepare("INSERT INTO repairs (registration_number, employee_name, issue_type, "
//                       "description, status_id, start_time) VALUES "
//                       "(:carNum, :employee, :issue, :description, \"В ПРОЦЕССЕ\", :start_time);");
//         query.bindValue(":carNum", carNum_13);
//         query.bindValue(":employee", employee_13);
//         query.bindValue(":issue", issue_13);
//         query.bindValue(":description", description);
//         query.bindValue(":start_time", start_time);
//         if (query.exec()) {
//             statusBar()->showMessage("Запись успешно добавлена! Статус работ: В ПРОЦЕССЕ", 5000);
//         } else {
//             statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//         }
//         break;
//     case 2:
//         query.prepare("SELECT start_time FROM repairs "
//                       "WHERE registration_number = :carNum AND employee_name = :employee AND issue_type = :issue;");
//         query.bindValue(":carNum", carNum_13);
//         query.bindValue(":employee", employee_13);
//         query.bindValue(":issue", issue_13);
//         if (query.exec()) {
//             QString result_out = 0;
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 result_out = record.value(0).toString();
//             }
//             if (result_out.isEmpty()) {
//                 statusBar()->showMessage("Запись с введенными параметрами на найдена", 5000);
//             } else {
//                 query.prepare("UPDATE repairs SET "
//                               "SET end_time = :end_time, status_id = \"ГОТОВО\", repair_time = :end_time - :start_time;");
//                 query.bindValue(":end_time", end_time);
//                 query.bindValue(":start_time", result_out);
//                 if (query.exec()) {
//                     statusBar()->showMessage("Запись успешно добавлена! Статус работ: ГОТОВО", 5000);
//                 } else {
//                     statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//                 }
//             }
//         }
//         break;
//     case 3:
//         query.prepare("INSERT INTO repairs (registration_number, employee_name, issue_type, "
//                       "description, status_id, start_time, end_time, repair_time) VALUES "
//                       "(:carNum, :employee, :issue, :description, \'ГОТОВО\', :start_time, :end_time, AGE(:end_time, :start_time));");
//         query.bindValue(":carNum", carNum_13);
//         query.bindValue(":employee", employee_13);
//         query.bindValue(":issue", issue_13);
//         query.bindValue(":description", description);
//         query.bindValue(":start_time", start_time);
//         query.bindValue(":end_time", end_time);
//         if (query.exec()) {
//             statusBar()->showMessage("Запись успешно добавлена! Статус работ: ГОТОВО", 5000);
//         } else {
//             qDebug() << query.lastError().text();
//             statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//         }
//         break;
//     case 4:
//         query.prepare("INSERT INTO repairs (registration_number, employee_name, issue_type, "
//                       "description, status_id) VALUES "
//                       "(:carNum, :employee, :issue, :description, \"НЕ НАЧАТО\");");
//         query.bindValue(":carNum", carNum_13);
//         query.bindValue(":employee", employee_13);
//         query.bindValue(":issue", issue_13);
//         query.bindValue(":description", description);
//         if (query.exec()) {
//             statusBar()->showMessage("Запись успешно добавлена! Статус работ: НЕ НАЧАТО", 5000);
//         } else {
//             statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//         }
//         break;
//     default:
//         statusBar()->showMessage("Ошибка записи. Проверьте значения выпадающих списков", 5000);
//         break;
//     }
// }

// void MainWindow::onMyButtonClickAddEmployee()
// {
//     QString address = ui->lineEdit_14_address->text();
//     QString name = ui->lineEdit_14_name->text();
//     QString number = ui->lineEdit_14_number->text();

//     int flag = 1;
//     if (address.isEmpty() || address == warnEmpty) {
//         ui->lineEdit_14_address->setText(warnEmpty);
//         flag = 0;
//     }
//     if (name.isEmpty() || name == warnEmpty) {
//         ui->lineEdit_14_name->setText(warnEmpty);
//         flag = 0;
//     }
//     if (number.isEmpty() || number == warnEmpty) {
//         ui->lineEdit_14_number->setText(warnEmpty);
//         flag = 0;
//     }

//     if (flag) {
//         QSqlQuery query;
//         query.prepare("INSERT INTO employee (full_name, address, phone_number) VALUES "
//                       "(:name, :address, :number);");
//         query.bindValue(":address", address);
//         query.bindValue(":name", name);
//         query.bindValue(":number", number);
//         if (query.exec()) {
//             statusBar()->showMessage("Запись успешно добавлена!", 5000);
//         } else {
//             statusBar()->showMessage("Ошибка записи. Проверьте корректность данных", 5000);
//         }
//     }
// }

// void MainWindow::onMyButtonClickMakeReportIssue()
// {
//     ui->textEdit_10_result->clear();
//     QString surname = ui->lineEdit_10_surName->text();
//     QString name = ui->lineEdit_10_name->text();
//     QString fatherName = ui->lineEdit_10_fatherName->text();

//     int flag = 1;
//     if (surname.isEmpty() || surname == warnEmpty) {
//         ui->lineEdit_10_surName->setText(warnEmpty);
//         flag = 0;
//     }
//     if (name.isEmpty() || name == warnEmpty) {
//         ui->lineEdit_10_name->setText(warnEmpty);
//         flag = 0;
//     }

//     if (flag) {
//         if (!fatherName.isEmpty()) {
//             name = surname + " " + name + " " + fatherName;
//         } else {
//             name = surname + " " + name;
//         }
//         QSqlQuery query;
//         query.prepare("SELECT Owners.full_name, Repairs.issue_type, Repairs.description "
//                       "FROM Owners JOIN Repairs "
//                       "On Owners.registration_number = Repairs.registration_number "
//                       "WHERE Owners.full_name = :name AND Repairs.status_id != 'ГОТОВО';");
//         query.bindValue(":name", name);
//         if (query.exec()) {
//             while (query.next()) {
//                 QSqlRecord record = query.record();
//                 QString result_out = "";
//                 for (int i = 0; i < record.count(); ++i) {
//                     result_out += record.value(i).toString() + " ";
//                 }
//                 ui->textEdit_10_result->insertPlainText(result_out + "\n");
//             }
//         }
//     }
// }

// void MainWindow::onMyButtonClickMakeReportStation()
// {
//     ui->lineEdit_11_amount->clear();
//     ui->textEdit_11_repairs->clear();
//     ui->textEdit_11_issues->clear();

//     QSqlQuery query;
//     query.prepare("SELECT COUNT(*) "
//                   "FROM Repairs "
//                   "WHERE status_id != 'ГОТОВО';");
//     if (query.exec()) {
//         while (query.next()) {
//             QSqlRecord record = query.record();
//             QString result_out = record.value(0).toString();
//             ui->lineEdit_11_amount->setAlignment(Qt::AlignCenter);
//             ui->lineEdit_11_amount->setText(result_out);
//         }
//     }

//     query.prepare("SELECT employee_name, repair_time "
//                   "FROM Repairs "
//                   "WHERE status_id = 'ГОТОВО';");
//     if (query.exec()) {
//         while (query.next()) {
//             QSqlRecord record = query.record();
//             QString result_out = "";
//             for (int i = 0; i < record.count(); ++i) {
//                 result_out += record.value(i).toString() + " ";
//             }
//             ui->textEdit_11_repairs->insertPlainText(result_out + "\n");
//         }
//     }

//     query.prepare("SELECT Car.manufacturer, Car.model, Repairs.issue_type "
//                   "FROM Car JOIN Repairs "
//                   "On Car.registration_number = Repairs.registration_number;");
//     if (query.exec()) {
//         while (query.next()) {
//             QSqlRecord record = query.record();
//             QString result_out = "";
//             for (int i = 0; i < record.count(); ++i) {
//                 result_out += record.value(i).toString() + " ";
//             }
//             ui->textEdit_11_issues->insertPlainText(result_out + "\n");
//         }
//     }
// }
