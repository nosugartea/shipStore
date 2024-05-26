#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginDialog(new LoginDialog(this))
{
    ui->setupUi(this);

    connect(loginDialog, &LoginDialog::loginButtonClicked, this, &MainWindow::handleLogin);
    loginDialog->exec();

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("1234");
    db.setDatabaseName("ships2");

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
    
    setTables();

    connect(ui->comboBox_EI, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxEI);
    connect(ui->comboBox_class, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxClass);
    connect(ui->comboBox_prod, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxProd);
    connect(ui->comboBox_enum, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxEnum);
    connect(ui->comboBox_param, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxParam);

    setComboBoxEI();
    setComboBoxClass();
    setComboBoxProd();
    setComboBoxEnum();
    setComboBoxVal();
    setComboBoxParam();

    // EI
    connect(ui->pushButton_1_add, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAdd_1);
    connect(ui->pushButton_2_delete, &QPushButton::clicked, this, &MainWindow::onMyButtonClickDelete_2);
    connect(ui->pushButton_3_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_3);

    // CHEM_CLASS
    connect(ui->pushButton_4_add, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAdd_4);
    connect(ui->comboBox_4_idEI, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox4EI);
    connect(ui->comboBox_4_mainClass, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox4MainClass);

    connect(ui->pushButton_5_delete, &QPushButton::clicked, this, &MainWindow::onMyButtonClickDelete_5);
    connect(ui->comboBox_5_classId, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox5ClassId);

    connect(ui->pushButton_6_change, &QPushButton::clicked, this, &MainWindow::onMyButtonClickChange_6);
    connect(ui->comboBox_6_nameClass, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox6NameClass);
    connect(ui->comboBox_6_nameMainClass, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox6MainClass);

    connect(ui->pushButton_7_class, &QPushButton::clicked, this, &MainWindow::onMyButtonClickClass_7);
    connect(ui->comboBox_7_className, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox7Class);
    connect(ui->pushButton_7_mainClass, &QPushButton::clicked, this, &MainWindow::onMyButtonClickMainClass_7);
    connect(ui->comboBox_7_mainClassName, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox7MainClass);

    // PROD
    connect(ui->pushButton_8_add, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAdd_8);
    connect(ui->comboBox_8_classId, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox8ClassId);

    connect(ui->pushButton_9_delete, &QPushButton::clicked, this, &MainWindow::onMyButtonClickDelete_9);
    connect(ui->comboBox_9_idProd, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox9ProdId);

    connect(ui->pushButton_12_searchClass, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearchClass_12);
    connect(ui->comboBox_12_className, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox12ClassName);
    connect(ui->pushButton_12_searchAll, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearchAll_12);

    // ENUM
    connect(ui->pushButton_13_add, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAdd_13);
    connect(ui->comboBox_13_idEnum, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox13EnumId);

    connect(ui->pushButton_14_delete_enum, &QPushButton::clicked, this, &MainWindow::onMyButtonClickDeleteEnum_14);
    connect(ui->comboBox_14_idEnum, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox14EnumId);
    connect(ui->pushButton_14_deleteVal, &QPushButton::clicked, this, &MainWindow::onMyButtonClickDeleteVal_14);
    connect(ui->comboBox_14_idVal, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox14ValId);

    connect(ui->pushButton_17_down, &QPushButton::clicked, this, &MainWindow::onMyButtonClickDown_17);
    connect(ui->comboBox_17_down, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox17Down);
    connect(ui->pushButton_17_up, &QPushButton::clicked, this, &MainWindow::onMyButtonClickUp_17);
    connect(ui->comboBox_17_up, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox17Up);

    connect(ui->pushButton_18_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_18);
    connect(ui->comboBox_18_idEnum, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox18EnumId);

    // PARAM
    connect(ui->pushButton_16_add, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAdd_16);
    connect(ui->comboBox_16_idClass, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox16ClassId);
    connect(ui->comboBox_16_idParam, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox16ParamId);

    connect(ui->pushButton_19_add, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAdd_19);
    connect(ui->comboBox_19_idEI, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox19EI);

    connect(ui->pushButton_23_add, &QPushButton::clicked, this, &MainWindow::onMyButtonClickAdd_23);
    connect(ui->comboBox_23_idEnum, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox23EnumId);
    connect(ui->comboBox_23_idParam, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox23ParamId);
    connect(ui->comboBox_23_idProd, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox23ProdId);

    connect(ui->pushButton_25_copy, &QPushButton::clicked, this, &MainWindow::onMyButtonClickCopy_25);
    connect(ui->comboBox_25_mainClass, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox25MainClass);
    connect(ui->comboBox_25_class, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox25ClassId);

    connect(ui->pushButton_26_prod, &QPushButton::clicked, this, &MainWindow::onMyButtonClickProd_26);
    connect(ui->pushButton_26_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_26);
    connect(ui->comboBox_26_idProd, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox26Prod);
    connect(ui->comboBox_26_idClass, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox26Class);

    connect(ui->pushButton_28_search, &QPushButton::clicked, this, &MainWindow::onMyButtonClickSearch_28);
    connect(ui->comboBox_28_idParam, &QComboBox::currentTextChanged, this, &MainWindow::onComboBox28Param);

    // CLEAR BUTTON
    connect(ui->pushButton_4_clear, &QPushButton::clicked, this, &MainWindow::onClear4);
    connect(ui->pushButton_8_clear, &QPushButton::clicked, this, &MainWindow::onClear8);
}

void MainWindow::handleLogin() {
    QString username = loginDialog->getUsername();
    QString password = loginDialog->getPassword();

    if (username == "admin" && password == "1234") {
        ui->stackedWidget_class->setCurrentIndex(0);
        ui->stackedWidget_prod->setCurrentIndex(0);
        ui->stackedWidget_enum->setCurrentIndex(0);
        ui->stackedWidget_param->setCurrentIndex(0);
        ui->stackedWidget_EI->setCurrentIndex(0);

        QString text =
            "<p>Основные функции приложения</p>"
            "<ul>"
            "<li>Единицы измерения"
            "  <ul>"
            "    <li>Вывод списка единиц измерения</li>"
            "  </ul>"
            "</li>"
            "<li>Классы"
            "  <ul>"
            "    <li>Поиск родительского класса</li>"
            "    <li>Поиск классов наследников</li>"
            "  </ul>"
            "</li>"
            "<li>Изделия"
            "  <ul>"
            "    <li>Поиск изделий заданного класса</li>"
            "    <li>Вывод всех изделий</li>"
            "  </ul>"
            "</li>"
            "<li>Перечисления"
            "  <ul>"
            "    <li>Поиск всех значений заданного перечисления</li>"
            "  </ul>"
            "</li>"
            "<li>Параметры"
            "  <ul>"
            "    <li>Поиск значения параметров изделия</li>"
            "    <li>Поиск значения параметров класса</li>"
            "    <li>Поиск изделия по значению параметра</li>"
            "  </ul>"
            "</li>"
            "</ul>"
            "<p>Бригада: Васильцева А. А; Кубрина Е; Макшаков Д. Д.</p>";
        ui->textEdit->setHtml(text);
    } else {
        ui->stackedWidget_EI->setCurrentIndex(0);
        ui->comboBox_EI->removeItem(1);
        ui->comboBox_EI->removeItem(1);

        ui->stackedWidget_class->setCurrentIndex(0);
        ui->comboBox_class->removeItem(1);
        ui->comboBox_class->removeItem(1);
        ui->comboBox_class->removeItem(1);

        ui->stackedWidget_prod->setCurrentIndex(0);
        ui->comboBox_prod->removeItem(1);
        ui->comboBox_prod->removeItem(1);

        ui->stackedWidget_enum->setCurrentIndex(0);
        ui->comboBox_enum->removeItem(1);
        ui->comboBox_enum->removeItem(1);
        ui->comboBox_enum->removeItem(1);

        ui->stackedWidget_param->setCurrentIndex(0);
        ui->comboBox_param->removeItem(3);
        ui->comboBox_param->removeItem(3);
        ui->comboBox_param->removeItem(3);
        ui->comboBox_param->removeItem(3);
        ui->comboBox_param->removeItem(3);

        QString text =
            "<p>Основные функции приложения</p>"
            "<ul>"
            "<li>Единицы измерения"
            "  <ul>"
            "    <li>Вывод списка единиц измерения</li>"
            "    <li>Добавление новой единицы измерения</li>"
            "    <li>Удаление существующей единицы измерения</li>"
            "  </ul>"
            "</li>"
            "<li>Классы"
            "  <ul>"
            "    <li>Поиск родительского класса</li>"
            "    <li>Поиск классов наследников</li>"
            "    <li>Добавление нового класса</li>"
            "    <li>Удаление существующего класса</li>"
            "    <li>Изменение родительского класса у класса</li>"
            "  </ul>"
            "</li>"
            "<li>Изделия"
            "  <ul>"
            "    <li>Поиск изделий заданного класса</li>"
            "    <li>Вывод всех изделий</li>"
            "    <li>Добавление изделия в базу данных</li>"
            "    <li>Удаление изделия из базы данных</li>"
            "  </ul>"
            "</li>"
            "<li>Перечисления"
            "  <ul>"
            "    <li>Поиск всех значений заданного перечисления</li>"
            "    <li>Добавление значения перечисления</li>"
            "    <li>Удаление значения перечисления</li>"
            "    <li>Перемещение значения перечисления вверх/вниз</li>"
            "  </ul>"
            "</li>"
            "<li>Параметры"
            "  <ul>"
            "    <li>Поиск значения параметров изделия</li>"
            "    <li>Поиск значения параметров класса</li>"
            "    <li>Поиск изделия по значению параметра</li>"
            "    <li>Добавление значения параметров экземпляра</li>"
            "    <li>Добавление нового параметра</li>"
            "    <li>Копирование параметров родительского класса</li>"
            "  </ul>"
            "</li>"
            "</ul>"
            "<p>Бригада: Васильцева А. А; Кубрина Е; Макшаков Д. Д.</p>";
        ui->textEdit->setHtml(text);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTables()
{
    QStringList headers;

    ui->tableWidget_3_result->setRowCount(0);
    ui->tableWidget_3_result->setColumnCount(4);
    headers = {"ID", "Сокращение", "Название", "Код"};
    ui->tableWidget_3_result->setHorizontalHeaderLabels(headers);
    ui->tableWidget_3_result->setColumnWidth(0, 50);
    ui->tableWidget_3_result->setColumnWidth(1, 100);
    ui->tableWidget_3_result->setColumnWidth(2, 150);
    ui->tableWidget_3_result->setColumnWidth(3, 50);

    ui->tableWidget_7_class->setRowCount(0);
    ui->tableWidget_7_class->setColumnCount(5);
    headers = {"ID род.", "Сокр", "Название", "ЕИ", "Род. класс"};
    ui->tableWidget_7_class->setHorizontalHeaderLabels(headers);
    ui->tableWidget_7_class->setColumnWidth(0, 50);
    ui->tableWidget_7_class->setColumnWidth(1, 80);
    ui->tableWidget_7_class->setColumnWidth(2, 100);
    ui->tableWidget_7_class->setColumnWidth(3, 50);
    ui->tableWidget_7_class->setColumnWidth(4, 50);

    ui->tableWidget_7_mainClass->setRowCount(0);
    ui->tableWidget_7_mainClass->setColumnCount(5);
    headers = {"ID насл.", "Сокращение", "Название", "ЕИ", "Род. класс"};
    ui->tableWidget_7_mainClass->setHorizontalHeaderLabels(headers);
    ui->tableWidget_7_mainClass->setColumnWidth(0, 50);
    ui->tableWidget_7_mainClass->setColumnWidth(1, 80);
    ui->tableWidget_7_mainClass->setColumnWidth(2, 100);
    ui->tableWidget_7_mainClass->setColumnWidth(3, 50);
    ui->tableWidget_7_mainClass->setColumnWidth(4, 50);

    ui->tableWidget_12_resultClass->setRowCount(0);
    ui->tableWidget_12_resultClass->setColumnCount(3);
    headers = {"ID", "Сокращение", "Название"};
    ui->tableWidget_12_resultClass->setHorizontalHeaderLabels(headers);
    ui->tableWidget_12_resultClass->setColumnWidth(0, 50);
    ui->tableWidget_12_resultClass->setColumnWidth(1, 100);
    ui->tableWidget_12_resultClass->setColumnWidth(2, 200);

    ui->tableWidget_12_result_all->setRowCount(0);
    ui->tableWidget_12_result_all->setColumnCount(4);
    headers = {"ID", "Сокращение", "Название", "Класс"};
    ui->tableWidget_12_result_all->setHorizontalHeaderLabels(headers);
    ui->tableWidget_12_result_all->setColumnWidth(0, 50);
    ui->tableWidget_12_result_all->setColumnWidth(1, 100);
    ui->tableWidget_12_result_all->setColumnWidth(2, 150);
    ui->tableWidget_12_result_all->setColumnWidth(3, 50);


    ui->tableWidget_18_result->setRowCount(0);
    ui->tableWidget_18_result->setColumnCount(5);
    headers = {"ID", "Номер", "Сокращение", "Название", "Значение"};
    ui->tableWidget_18_result->setHorizontalHeaderLabels(headers);
    ui->tableWidget_18_result->setColumnWidth(0, 50);
    ui->tableWidget_18_result->setColumnWidth(1, 50);
    ui->tableWidget_18_result->setColumnWidth(2, 100);
    ui->tableWidget_18_result->setColumnWidth(3, 150);
    ui->tableWidget_18_result->setColumnWidth(4, 450);

    ui->tableWidget_26_prod->setRowCount(0);
    ui->tableWidget_26_prod->setColumnCount(8);
    headers = {"ID пары", "Сокращ", "Название", "Числовое зн.", "ЕИ", "Строк. знач.", "Значение", "OBLOB"};
    ui->tableWidget_26_prod->setHorizontalHeaderLabels(headers);
    ui->tableWidget_26_prod->setColumnWidth(0, 50);
    ui->tableWidget_26_prod->setColumnWidth(1, 50);
    ui->tableWidget_26_prod->setColumnWidth(2, 240);
    ui->tableWidget_26_prod->setColumnWidth(3, 150);
    ui->tableWidget_26_prod->setColumnWidth(4, 50);
    ui->tableWidget_26_prod->setColumnWidth(5, 80);
    ui->tableWidget_26_prod->setColumnWidth(7, 80);
    ui->tableWidget_26_prod->setColumnWidth(8, 130);

    ui->tableWidget_26_search->setRowCount(0);
    ui->tableWidget_26_search->setColumnCount(7);
    headers = {"ID пары", "Сокращ", "Название", "Мин. зн.", "Макс. зн.", "ЕИ", "Название ЕИ"};
    ui->tableWidget_26_search->setHorizontalHeaderLabels(headers);
    ui->tableWidget_26_search->setColumnWidth(0, 50);
    ui->tableWidget_26_search->setColumnWidth(1, 100);
    ui->tableWidget_26_search->setColumnWidth(2, 330);
    ui->tableWidget_26_search->setColumnWidth(3, 100);
    ui->tableWidget_26_search->setColumnWidth(4, 100);
    ui->tableWidget_26_search->setColumnWidth(5, 50);
    ui->tableWidget_26_search->setColumnWidth(6, 120);

    ui->tableWidget_28_search->setRowCount(0);
    ui->tableWidget_28_search->setColumnCount(4);
    headers = {"ID", "Название", "Мин. зн.", "Макс. зн."};
    ui->tableWidget_28_search->setHorizontalHeaderLabels(headers);
    ui->tableWidget_28_search->setColumnWidth(0, 100);
    ui->tableWidget_28_search->setColumnWidth(1, 430);
    ui->tableWidget_28_search->setColumnWidth(2, 150);
    ui->tableWidget_28_search->setColumnWidth(2, 150);
}

void MainWindow::onComboBoxEI(const QString &text)
{
    int index = ui->comboBox_EI->findText(text);
    if (index != -1) {
        ui->stackedWidget_EI->setCurrentIndex(index);
    }
}

void MainWindow::onComboBoxClass(const QString &text)
{
    int index = ui->comboBox_class->findText(text);
    if (index != -1) {
        ui->stackedWidget_class->setCurrentIndex(index);
    }
    setComboBoxEI();
    setComboBoxClass();
}

void MainWindow::onComboBoxProd(const QString &text)
{
    int index = ui->comboBox_prod->findText(text);
    if (index != -1) {
        ui->stackedWidget_prod->setCurrentIndex(index);
    }
    setComboBoxClass();
    setComboBoxProd();
}

void MainWindow::onComboBoxEnum(const QString &text)
{
    int index = ui->comboBox_enum->findText(text);
    if (index != -1) {
        ui->stackedWidget_enum->setCurrentIndex(index);
    }
    setComboBoxEnum();
}

void MainWindow::onComboBoxParam(const QString &text)
{
    int index = ui->comboBox_param->findText(text);
    if (index != -1) {
        ui->stackedWidget_param->setCurrentIndex(index);
    }
    setComboBoxParam();
}

void MainWindow::setComboBoxEI()
{
    ui->comboBox_4_idEI->clear();
    ui->comboBox_19_idEI->clear();

    QSqlQuery query;
    query.prepare("SELECT ID_EI, NAME FROM EI ORDER BY ID_EI;");
    if (query.exec()) {
        while (query.next()) {
            QString id_ei = query.value(0).toString();
            QString name = query.value(1).toString();
            QString combined = "ID = " + id_ei + " " + ": " + name;
            ui->comboBox_4_idEI->addItem(combined);
            ui->comboBox_19_idEI->addItem(combined);
        }
        idEI_4 = ui->comboBox_4_idEI->itemText(0);
        static const QRegularExpression re1("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re1.match(idEI_4);
        if (match.hasMatch()) {
            idEI_4 = match.captured(1);
        }
        idEI_19 = ui->comboBox_19_idEI->itemText(0);
        static const QRegularExpression re2("ID\\s*=\\s*(\\d+)");
        match = re2.match(idEI_19);
        if (match.hasMatch()) {
            idEI_19 = match.captured(1);
        }
    }
}

void MainWindow::setComboBoxClass()
{
    ui->comboBox_4_mainClass->clear();
    ui->comboBox_6_nameMainClass->clear();
    ui->comboBox_7_mainClassName->clear();
    ui->comboBox_5_classId->clear();
    ui->comboBox_6_nameClass->clear();
    ui->comboBox_7_className->clear();
    ui->comboBox_8_classId->clear();
    ui->comboBox_12_className->clear();
    ui->comboBox_16_idClass->clear();
    ui->comboBox_25_class->clear();
    ui->comboBox_25_mainClass->clear();
    ui->comboBox_26_idClass->clear();

    ui->comboBox_4_mainClass->addItem("Нет родительского класса");
    ui->comboBox_6_nameMainClass->addItem("Нет родительского класса");
    // ui->comboBox_7_mainClassName->addItem("Нет родительского класса");
    QSqlQuery query;
    query.prepare("SELECT ID_CLASS, NAME FROM CHEM_CLASS ORDER BY ID_CLASS;");
    if (query.exec()) {
        while (query.next()) {
            QString id_ei = query.value(0).toString();
            QString name = query.value(1).toString();
            QString combined = "ID = " + id_ei + " " + ": " + name;

            ui->comboBox_4_mainClass->addItem(combined);
            ui->comboBox_6_nameMainClass->addItem(combined);
            ui->comboBox_7_mainClassName->addItem(combined);
            ui->comboBox_5_classId->addItem(combined);
            ui->comboBox_6_nameClass->addItem(combined);
            ui->comboBox_7_className->addItem(combined);
            ui->comboBox_8_classId->addItem(combined);
            ui->comboBox_12_className->addItem(combined);
            ui->comboBox_16_idClass->addItem(combined);
            ui->comboBox_25_class->addItem(combined);
            ui->comboBox_25_mainClass->addItem(combined);
            ui->comboBox_26_idClass->addItem(combined);
        }
        mainClass_4 = ui->comboBox_4_mainClass->itemText(0);
        static const QRegularExpression re1("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re1.match(mainClass_4);
        if (match.hasMatch()) {
            mainClass_4 = match.captured(1);
        }
        classId_5 = ui->comboBox_5_classId->itemText(0);
        static const QRegularExpression re2("ID\\s*=\\s*(\\d+)");
        match = re2.match(classId_5);
        if (match.hasMatch()) {
            classId_5 = match.captured(1);
        }
        classId_6 = ui->comboBox_6_nameClass->itemText(0);
        static const QRegularExpression re3("ID\\s*=\\s*(\\d+)");
        match = re3.match(classId_6);
        if (match.hasMatch()) {
            classId_6 = match.captured(1);
        }
        mainClass_6 = ui->comboBox_6_nameMainClass->itemText(0);
        static const QRegularExpression re4("ID\\s*=\\s*(\\d+)");
        match = re4.match(mainClass_6);
        if (match.hasMatch()) {
            mainClass_6 = match.captured(1);
        }
        classId_7 = ui->comboBox_7_className->itemText(0);
        static const QRegularExpression re5("ID\\s*=\\s*(\\d+)");
        match = re5.match(classId_7);
        if (match.hasMatch()) {
            classId_7 = match.captured(1);
        }
        mainClass_7 = ui->comboBox_7_mainClassName->itemText(0);
        static const QRegularExpression re6("ID\\s*=\\s*(\\d+)");
        match = re6.match(mainClass_7);
        if (match.hasMatch()) {
            mainClass_7 = match.captured(1);
        }
        classId_8 = ui->comboBox_8_classId->itemText(0);
        static const QRegularExpression re7("ID\\s*=\\s*(\\d+)");
        match = re7.match(classId_8);
        if (match.hasMatch()) {
            classId_8 = match.captured(1);
        }
        classId_12 = ui->comboBox_12_className->itemText(0);
        static const QRegularExpression re8("ID\\s*=\\s*(\\d+)");
        match = re8.match(classId_12);
        if (match.hasMatch()) {
            classId_12 = match.captured(1);
        }
        classId_16 = ui->comboBox_16_idClass->itemText(0);
        static const QRegularExpression re9("ID\\s*=\\s*(\\d+)");
        match = re9.match(classId_16);
        if (match.hasMatch()) {
            classId_16 = match.captured(1);
        }
        classId_25 = ui->comboBox_25_class->itemText(0);
        static const QRegularExpression re11("ID\\s*=\\s*(\\d+)");
        match = re11.match(classId_25);
        if (match.hasMatch()) {
            classId_25 = match.captured(1);
        }
        mainClass_25 = ui->comboBox_25_mainClass->itemText(0);
        static const QRegularExpression re12("ID\\s*=\\s*(\\d+)");
        match = re12.match(mainClass_25);
        if (match.hasMatch()) {
            mainClass_25 = match.captured(1);
        }
        classId_26 = ui->comboBox_26_idClass->itemText(0);
        static const QRegularExpression re13("ID\\s*=\\s*(\\d+)");
        match = re13.match(classId_26);
        if (match.hasMatch()) {
            classId_26 = match.captured(1);
        }
    }
}

void MainWindow::setComboBoxProd()
{
    ui->comboBox_9_idProd->clear();
    ui->comboBox_23_idProd->clear();
    ui->comboBox_26_idProd->clear();

    QSqlQuery query;
    query.prepare("SELECT ID_PROD, NAME FROM PROD ORDER BY ID_PROD;");
    if (query.exec()) {
        while (query.next()) {
            QString id_prod = query.value(0).toString();
            QString name = query.value(1).toString();
            QString combined = "ID = " + id_prod + " " + ": " + name;
            ui->comboBox_9_idProd->addItem(combined);
            ui->comboBox_23_idProd->addItem(combined);
            ui->comboBox_26_idProd->addItem(combined);
        }
        prodId_9 = ui->comboBox_9_idProd->itemText(0);
        static const QRegularExpression re1("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re1.match(prodId_9);
        if (match.hasMatch()) {
            prodId_9 = match.captured(1);
        }
        prodId_23 = ui->comboBox_23_idProd->itemText(0);
        static const QRegularExpression re3("ID\\s*=\\s*(\\d+)");
        match = re3.match(prodId_23);
        if (match.hasMatch()) {
            prodId_23 = match.captured(1);
        }
        prodId_26 = ui->comboBox_26_idProd->itemText(0);
        static const QRegularExpression re5("ID\\s*=\\s*(\\d+)");
        match = re5.match(prodId_26);
        if (match.hasMatch()) {
            prodId_26 = match.captured(1);
        }
    }
}

void MainWindow::setComboBoxEnum()
{
    ui->comboBox_13_idEnum->clear();
    ui->comboBox_14_idEnum->clear();
    ui->comboBox_18_idEnum->clear();

    QSqlQuery query;
    query.prepare("SELECT ID_CLASS, NAME FROM CHEM_CLASS ORDER BY ID_CLASS;");
    if (query.exec()) {
        while (query.next()) {
            QString id_ei = query.value(0).toString();
            QString name = query.value(1).toString();
            QString combined = "ID = " + id_ei + " " + ": " + name;
            if (id_ei.toInt() > 19 && id_ei.toInt() < 28) {
                ui->comboBox_13_idEnum->addItem(combined);
                ui->comboBox_14_idEnum->addItem(combined);
                ui->comboBox_18_idEnum->addItem(combined);
            }
        }
        idEnum_13 = ui->comboBox_13_idEnum->itemText(0);
        static const QRegularExpression re1("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re1.match(idEnum_13);
        if (match.hasMatch()) {
            idEnum_13 = match.captured(1);
        }
        idEnum_14 = ui->comboBox_14_idEnum->itemText(0);
        static const QRegularExpression re2("ID\\s*=\\s*(\\d+)");
        match = re2.match(idEnum_14);
        if (match.hasMatch()) {
            idEnum_14 = match.captured(1);
        }
        idEnum_18 = ui->comboBox_18_idEnum->itemText(0);
        static const QRegularExpression re3("ID\\s*=\\s*(\\d+)");
        match = re3.match(idEnum_18);
        if (match.hasMatch()) {
            idEnum_18 = match.captured(1);
        }
    }
}

void MainWindow::setComboBoxVal()
{
    ui->comboBox_14_idVal->clear();
    ui->comboBox_17_down->clear();
    ui->comboBox_17_up->clear();
    ui->comboBox_23_idEnum->clear();
    QSqlQuery query;
    query.prepare("SELECT ID_POS, E_NAME "
                  "FROM POS_ENUM ORDER BY ID_POS, ID_ENUM;");
    if (query.exec()) {
        while (query.next()) {
            QString id_pos = query.value(0).toString();
            QString name = query.value(1).toString();
            QString combined = "ID = " + id_pos + ": " + name;
            ui->comboBox_14_idVal->addItem(combined);
            ui->comboBox_17_down->addItem(combined);
            ui->comboBox_17_up->addItem(combined);
            ui->comboBox_23_idEnum->addItem(combined);
        }
        idVal_14 = ui->comboBox_14_idVal->itemText(0);
        static const QRegularExpression re1("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re1.match(idVal_14);
        if (match.hasMatch()) {
            idVal_14 = match.captured(1);
        }
        idDown_17 = ui->comboBox_17_down->itemText(0);
        static const QRegularExpression re2("ID\\s*=\\s*(\\d+)");
        match = re2.match(idDown_17);
        if (match.hasMatch()) {
            idDown_17 = match.captured(1);
        }
        idUp_17 = ui->comboBox_17_up->itemText(0);
        static const QRegularExpression re3("ID\\s*=\\s*(\\d+)");
        match = re3.match(idUp_17);
        if (match.hasMatch()) {
            idUp_17 = match.captured(1);
        }
        idEnum_23 = ui->comboBox_23_idEnum->itemText(0);
        static const QRegularExpression re4("ID\\s*=\\s*(\\d+)");
        match = re4.match(idEnum_23);
        if (match.hasMatch()) {
            idEnum_23 = match.captured(1);
        }
    }
}

void MainWindow::setComboBoxParam()
{
    ui->comboBox_16_idParam->clear();
    ui->comboBox_23_idParam->clear();
    ui->comboBox_28_idParam->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM PARAMETR1 ORDER BY ID_PAR;");
    if (query.exec()) {
        while (query.next()) {
            QString id_pos = query.value(0).toString();
            QString name = query.value(2).toString();
            QString combined = "ID = " + id_pos + ": " + name;
            ui->comboBox_16_idParam->addItem(combined);
            ui->comboBox_23_idParam->addItem(combined);
            ui->comboBox_28_idParam->addItem(combined);
        }
        paramId_16 = ui->comboBox_16_idParam->itemText(0);
        static const QRegularExpression re1("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re1.match(paramId_16);
        if (match.hasMatch()) {
            paramId_16 = match.captured(1);
        }
        paramId_23 = ui->comboBox_23_idParam->itemText(0);
        static const QRegularExpression re3("ID\\s*=\\s*(\\d+)");
        match = re3.match(paramId_23);
        if (match.hasMatch()) {
            paramId_23 = match.captured(1);
        }
        paramId_28 = ui->comboBox_28_idParam->itemText(0);
        static const QRegularExpression re4("ID\\s*=\\s*(\\d+)");
        match = re4.match(paramId_28);
        if (match.hasMatch()) {
            paramId_28 = match.captured(1);
        }
    }
}


void MainWindow::onComboBox4EI(const QString &text)
{
    int index = ui->comboBox_4_idEI->findText(text);
    if (index != -1) {
        idEI_4 = ui->comboBox_4_idEI->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idEI_4);
        if (match.hasMatch()) {
            idEI_4 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox4MainClass(const QString &text)
{
    int index = ui->comboBox_4_mainClass->findText(text);
    if (index != -1) {
        mainClass_4 = ui->comboBox_4_mainClass->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(mainClass_4);
        if (match.hasMatch()) {
            mainClass_4 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox5ClassId(const QString &text)
{
    int index = ui->comboBox_5_classId->findText(text);
    if (index != -1) {
        classId_5 = ui->comboBox_5_classId->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_5);
        if (match.hasMatch()) {
            classId_5 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox6NameClass(const QString &text)
{
    int index = ui->comboBox_6_nameClass->findText(text);
    if (index != -1) {
        classId_6 = ui->comboBox_6_nameClass->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_6);
        if (match.hasMatch()) {
            classId_6 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox6MainClass(const QString &text)
{
    int index = ui->comboBox_6_nameMainClass->findText(text);
    if (index != -1) {
        mainClass_6 = ui->comboBox_6_nameMainClass->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(mainClass_6);
        if (match.hasMatch()) {
            mainClass_6 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox7Class(const QString &text)
{
    int index = ui->comboBox_7_className->findText(text);
    if (index != -1) {
        classId_7 = ui->comboBox_7_className->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_7);
        if (match.hasMatch()) {
            classId_7 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox7MainClass(const QString &text)
{
    int index = ui->comboBox_7_mainClassName->findText(text);
    if (index != -1) {
        mainClass_7 = ui->comboBox_7_mainClassName->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(mainClass_7);
        if (match.hasMatch()) {
            mainClass_7 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox8ClassId(const QString &text)
{
    int index = ui->comboBox_8_classId->findText(text);
    if (index != -1) {
        classId_8 = ui->comboBox_8_classId->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_8);
        if (match.hasMatch()) {
            classId_8 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox9ProdId(const QString &text)
{
    int index = ui->comboBox_9_idProd->findText(text);
    if (index != -1) {
        prodId_9 = ui->comboBox_9_idProd->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(prodId_9);
        if (match.hasMatch()) {
            prodId_9 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox12ClassName(const QString &text)
{
    int index = ui->comboBox_12_className->findText(text);
    if (index != -1) {
        classId_12 = ui->comboBox_12_className->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_12);
        if (match.hasMatch()) {
            classId_12 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox13EnumId(const QString &text)
{
    int index = ui->comboBox_13_idEnum->findText(text);
    if (index != -1) {
        idEnum_13 = ui->comboBox_13_idEnum->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idEnum_13);
        if (match.hasMatch()) {
            idEnum_13 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox14EnumId(const QString &text)
{
    int index = ui->comboBox_14_idEnum->findText(text);
    if (index != -1) {
        idEnum_14 = ui->comboBox_14_idEnum->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idEnum_14);
        if (match.hasMatch()) {
            idEnum_14 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox14ValId(const QString &text)
{
    int index = ui->comboBox_14_idVal->findText(text);
    if (index != -1) {
        idVal_14 = ui->comboBox_14_idVal->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idVal_14);
        if (match.hasMatch()) {
            idVal_14 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox17Down(const QString &text)
{
    int index = ui->comboBox_17_down->findText(text);
    if (index != -1) {
        idDown_17 = ui->comboBox_17_down->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idDown_17);
        if (match.hasMatch()) {
            idDown_17 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox17Up(const QString &text)
{
    int index = ui->comboBox_17_up->findText(text);
    if (index != -1) {
        idUp_17 = ui->comboBox_17_up->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idUp_17);
        if (match.hasMatch()) {
            idUp_17 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox18EnumId(const QString &text)
{
    int index = ui->comboBox_18_idEnum->findText(text);
    if (index != -1) {
        idEnum_18 = ui->comboBox_18_idEnum->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idEnum_18);
        if (match.hasMatch()) {
            idEnum_18 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox16ClassId(const QString &text)
{
    int index = ui->comboBox_16_idClass->findText(text);
    if (index != -1) {
        classId_16 = ui->comboBox_16_idClass->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_16);
        if (match.hasMatch()) {
            classId_16 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox16ParamId(const QString &text)
{
    int index = ui->comboBox_16_idParam->findText(text);
    if (index != -1) {
        paramId_16 = ui->comboBox_16_idParam->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(paramId_16);
        if (match.hasMatch()) {
            paramId_16 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox19EI(const QString &text)
{
    int index = ui->comboBox_19_idEI->findText(text);
    if (index != -1) {
        idEI_19 = ui->comboBox_19_idEI->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idEI_19);
        if (match.hasMatch()) {
            idEI_19 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox23EnumId(const QString &text)
{
    int index = ui->comboBox_23_idEnum->findText(text);
    if (index != -1) {
        idEnum_23 = ui->comboBox_23_idEnum->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(idEnum_23);
        if (match.hasMatch()) {
            idEnum_23 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox23ProdId(const QString &text)
{
    int index = ui->comboBox_23_idProd->findText(text);
    if (index != -1) {
        prodId_23 = ui->comboBox_23_idProd->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(prodId_23);
        if (match.hasMatch()) {
            prodId_23 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox23ParamId(const QString &text)
{
    int index = ui->comboBox_23_idParam->findText(text);
    if (index != -1) {
        paramId_23 = ui->comboBox_23_idParam->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(paramId_23);
        if (match.hasMatch()) {
            paramId_23 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox25MainClass(const QString &text)
{
    int index = ui->comboBox_25_mainClass->findText(text);
    if (index != -1) {
        mainClass_25 = ui->comboBox_25_mainClass->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(mainClass_25);
        if (match.hasMatch()) {
            mainClass_25 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox25ClassId(const QString &text)
{
    int index = ui->comboBox_25_class->findText(text);
    if (index != -1) {
        classId_25 = ui->comboBox_25_class->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_25);
        if (match.hasMatch()) {
            classId_25 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox26Prod(const QString &text)
{
    int index = ui->comboBox_26_idProd->findText(text);
    if (index != -1) {
        prodId_26 = ui->comboBox_26_idProd->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(prodId_26);
        if (match.hasMatch()) {
            prodId_26 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox26Class(const QString &text)
{
    int index = ui->comboBox_26_idClass->findText(text);
    if (index != -1) {
        classId_26 = ui->comboBox_26_idClass->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(classId_26);
        if (match.hasMatch()) {
            classId_26 = match.captured(1);
        }
    }
}

void MainWindow::onComboBox28Param(const QString &text)
{
    int index = ui->comboBox_28_idParam->findText(text);
    if (index != -1) {
        paramId_28 = ui->comboBox_28_idParam->itemText(index);
        static const QRegularExpression re("ID\\s*=\\s*(\\d+)");
        QRegularExpressionMatch match = re.match(paramId_28);
        if (match.hasMatch()) {
            paramId_28 = match.captured(1);
        }
    }
}

void MainWindow::onMyButtonClickAdd_1()
{
    ui->textEdit_1_result->clear();
    QString code = ui->lineEdit_1_code->text();
    QString name = ui->lineEdit_1_name->text();
    QString shortName = ui->lineEdit_1_shortName->text();
    if (code.isEmpty() || code == warnEmpty) {
        ui->lineEdit_1_code->setText(warnEmpty);
    }
    if (name.isEmpty() || name == warnEmpty) {
        ui->lineEdit_1_name->setText(warnEmpty);
    }
    if (shortName.isEmpty()) {
        shortName = "NULL";
    }
    if ((!code.isEmpty() && code != warnEmpty) &&
        (!name.isEmpty() && name != warnEmpty)){
        QSqlQuery query;
        query.prepare("SELECT * FROM INS_EI(:pCode, :pShortName, :pName);");
        query.bindValue(":pCode", code);
        query.bindValue(":pShortName", shortName);
        query.bindValue(":pName", name);

        if (query.exec()) {
            while (query.next()) {
                QString id_ei = query.value(0).toString();
                QString result_out = query.value(1).toString();
                if (result_out == "1")
                {
                    ui->textEdit_1_result->insertPlainText("Запись успешно создана. Новый ID = " + id_ei + "\n");
                } else if (result_out == "0") {
                    ui->textEdit_1_result->insertPlainText("Ошибка добавления.\n");
                }
            }
        }
    }
}

void MainWindow::onMyButtonClickDelete_2()
{
    ui->textEdit_2_result->clear();
    QString code = ui->lineEdit_2_id->text();
    if (code.isEmpty() || code == warnEmpty) {
        ui->lineEdit_1_code->setText(warnEmpty);
    }
    if (!code.isEmpty() && code != warnEmpty) {
        QSqlQuery query;
        query.prepare("SELECT * FROM DEL_EI(:pCode);");
        query.bindValue(":pCode", code);

        if (query.exec()) {
            while (query.next()) {
                QString result_out = query.value(0).toString();
                if (result_out == "1")
                {
                    ui->textEdit_2_result->insertPlainText("Запись успешно удалена.\n");
                } else if (result_out == "0") {
                    ui->textEdit_2_result->insertPlainText("Ошибка удаления.\n");
                }
            }
        }
    }
}

void MainWindow::onMyButtonClickSearch_3()
{
    ui->tableWidget_3_result->setRowCount(0);
    int rowCount = 0;

    QSqlQuery query;
    query.prepare("SELECT * FROM EI ORDER BY ID_EI;");
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            ui->tableWidget_3_result->insertRow(rowCount);
            ui->tableWidget_3_result->setRowHeight(rowCount, 30);
            for (int i = 0; i < record.count(); ++i) {
                qDebug() << i;
                QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                ui->tableWidget_3_result->setItem(rowCount, i, item);
            }
            ++rowCount;
        }
    }
}

void MainWindow::onMyButtonClickAdd_4()
{
    ui->textEdit_4_result->clear();
    QString name = ui->lineEdit_4_nameClass->text();
    QString shortName = ui->lineEdit_4_shortName->text();
    if (name.isEmpty() || name == warnEmpty) {
        ui->lineEdit_4_nameClass->setText(warnEmpty);
    }
    if (shortName.isEmpty()) {
        shortName = "NULL";
    }
    if ((!name.isEmpty() && name != warnEmpty)){
        QSqlQuery query;
        query.prepare("SELECT * FROM ADD_CHEM_CLASS(:pShortName, :pName, :pBaseEI, :pMainClass);");
        query.bindValue(":pShortName", shortName);
        query.bindValue(":pName", name);
        query.bindValue(":pBaseEI", idEI_4);
        query.bindValue(":pMainClass", mainClass_4);

        if (query.exec()) {
            while (query.next()) {
                QString id_class = query.value(0).toString();
                QString result_out = query.value(1).toString();
                if (result_out == "1")
                {
                    ui->textEdit_4_result->insertPlainText("Запись успешно создана. Новый ID = " +id_class + "\n");
                } else if (result_out == "0") {
                    ui->textEdit_4_result->insertPlainText("Ошибка создания.\n");
                }
            }
        }
    }
}

void MainWindow::onMyButtonClickDelete_5()
{
    ui->textEdit_5_result->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM DELETE_CHEM_CLASS(:pName);");
    query.bindValue(":pName", classId_5);
    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_5_result->insertPlainText("Запись успешно удалена.\n");
            } else if (result_out == "0") {
                ui->textEdit_5_result->insertPlainText("Ошибка удаления.\n");
            }
        }
    }
}

void MainWindow::onMyButtonClickChange_6()
{
    ui->textEdit_6_result->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM CHANGE_PARENT_CHEM_CLASS(:pIdClass, :pNewMainClass);");
    query.bindValue(":pIdClass", classId_6);
    query.bindValue(":pNewMainClass", mainClass_6);
    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_6_result->insertPlainText("Родительский класс успешно изменен.\n");
            } else if (result_out == "0") {
                ui->textEdit_6_result->insertPlainText("Ошибка внесения изменений в БД.\n");
            } else if (result_out == "2") {
                ui->textEdit_6_result->insertPlainText("Ошибка внесения изменений в БД. Вы хотите сделать цикл.\n");
            }
        }
    }
}

void MainWindow::onMyButtonClickClass_7()
{
    ui->tableWidget_7_class->setRowCount(0);
    QSqlQuery query;
    query.prepare("SELECT * FROM FIND_PARENT(:pIdClass);");
    query.bindValue(":pIdClass", classId_7);
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_7_class->insertRow(0);
            ui->tableWidget_7_class->setRowHeight(0, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                ui->tableWidget_7_class->setItem(0, i, item);
            }
        }
    }
}

void MainWindow::onMyButtonClickMainClass_7()
{
    ui->tableWidget_7_mainClass->setRowCount(0);
    int rowCount = 0;

    if (mainClass_7 == "Нет родительского класса") {
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM FIND_CHILD(:pIdClass) ORDER BY CHILD_ID;");
    query.bindValue(":pIdClass", mainClass_7);
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_7_mainClass->insertRow(rowCount);
            ui->tableWidget_7_mainClass->setRowHeight(rowCount, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                ui->tableWidget_7_mainClass->setItem(rowCount, i, item);
            }
             ++rowCount;
        }
    }
}

void MainWindow::onMyButtonClickAdd_8()
{
    ui->textEdit_8_result->clear();
    QString name = ui->lineEdit_8_name->text();
    QString shortName = ui->lineEdit_8_shortName->text();
    if (name.isEmpty() || name == warnEmpty) {
        ui->lineEdit_1_name->setText(warnEmpty);
    }
    if (shortName.isEmpty()) {
        shortName = "NULL";
    }
    if ((!name.isEmpty() && name != warnEmpty)){
        QSqlQuery query;
        query.prepare("SELECT * FROM ADD_PROD(:pShortName, :pName, :pIdClass);");
        query.bindValue(":pIdClass", classId_8);
        query.bindValue(":pShortName", shortName);
        query.bindValue(":pName", name);

        if (query.exec()) {
            while (query.next()) {
                QString newID = query.value(0).toString();
                QString result_out = query.value(1).toString();
                if (result_out == "1")
                {
                    ui->textEdit_8_result->insertPlainText("Запись успешно создана. ID нового изделия: " + newID + "\n");
                } else if (result_out == "0") {
                    ui->textEdit_8_result->insertPlainText("Ошибка добавления.\n");
                }
            }
        }
    }
}

void MainWindow::onMyButtonClickDelete_9()
{
    qDebug() << "hjcjds";
    ui->textEdit_9_result->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM DELETE_PROD(:pCode);");
    query.bindValue(":pCode", prodId_9);

    if (query.exec()) {
        while (query.next()) {
            qDebug() << "hk";
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_9_result->insertPlainText("Запись успешно удалена.\n");
            } else if (result_out == "0") {
                ui->textEdit_9_result->insertPlainText("Ошибка удаления." + query.lastError().text() + "\n");
            }
        }
    } else {
        ui->textEdit_9_result->insertPlainText("Ошибка удаления." + query.lastError().text() + "\n");
    }
}

void MainWindow::onMyButtonClickSearchClass_12()
{
    ui->tableWidget_12_resultClass->setRowCount(0);
    int rowCount = 0;

    QSqlQuery query;
    query.prepare("SELECT * FROM FIND_PROD_BY_CLASS(:pIdClass) ORDER BY ID_PROD;");
    query.bindValue(":pIdClass", classId_12);
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_12_resultClass->insertRow(rowCount);
            ui->tableWidget_12_resultClass->setRowHeight(rowCount, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                ui->tableWidget_12_resultClass->setItem(rowCount, i, item);
            }
            ++rowCount;
        }
    }
}

void MainWindow::onMyButtonClickSearchAll_12()
{
    ui->tableWidget_12_result_all->setRowCount(0);
    int rowCount = 0;

    QSqlQuery query;
    query.prepare("SELECT * FROM PROD ORDER BY ID_PROD;");
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_12_result_all->insertRow(rowCount);
            ui->tableWidget_12_result_all->setRowHeight(rowCount, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                ui->tableWidget_12_result_all->setItem(rowCount, i, item);
            }
            ++rowCount;
        }
    }
}

void MainWindow::onMyButtonClickAdd_13()
{
    ui->textEdit_13_result->clear();
    QString iVal = ui->lineEdit_13_iVal->text();
    QString rVal = ui->lineEdit_13_rVal->text();
    QString name = ui->lineEdit_13_name->text();
    QString shortName = ui->lineEdit_13_shortName->text();
    QString image = ui->lineEdit_13_image->text();

    if (name.isEmpty() || name == warnEmpty) {
        ui->lineEdit_13_name->setText(warnEmpty);
    }
    if (shortName.isEmpty()) {
        shortName = "NULL";
    }
    if (iVal.isEmpty()) {
        iVal = "NULL";
    }
    if (rVal.isEmpty()) {
        rVal = "NULL";
    }
    if (image.isEmpty()) {
        image = "NULL";
    }
    if ((!name.isEmpty() && name != warnEmpty)){
        QSqlQuery query;
        query.prepare("SELECT * FROM INS_VAL_ENUM(:pIdEnum, :pShortName, :pName, :pRval, :pIntVal, :pPicVal);");
        qDebug() << idEnum_13 << " " << shortName <<  " " << name << " " << rVal << " " <<  iVal << " " << image;
        query.bindValue(":pIdEnum", idEnum_13);
        query.bindValue(":pShortName", shortName);
        query.bindValue(":pName", name);
        query.bindValue(":pRval", rVal);
        query.bindValue(":pIntVal", iVal);
        query.bindValue(":pPicVal", image);

        if (query.exec()) {
            while (query.next()) {
                QString newId = query.value(0).toString();
                QString result_out = query.value(1).toString();
                if (result_out == "1")
                {
                    ui->textEdit_13_result->insertPlainText("Запись успешно создана. ИД новой записи : " + newId + "\n");
                } else if (result_out == "0") {
                    ui->textEdit_13_result->insertPlainText("Ошибка добавления.\n");
                }
            }
        } else {
            ui->textEdit_13_result->insertPlainText("Запрос не выполнен." + query.lastError().text() + "\n");
        }
    }
}

void MainWindow::onMyButtonClickDeleteEnum_14()
{
    ui->textEdit_14_requestEnum->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM DEL_ENUM(:pCode);");
    query.bindValue(":pCode", idEnum_14);

    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_14_requestEnum->insertPlainText("Записи успешно удалены.\n");
            } else if (result_out == "0") {
                ui->textEdit_14_requestEnum->insertPlainText("Ошибка удаления.\n");
            }
        }
    }
}

void MainWindow::onMyButtonClickDeleteVal_14()
{
    ui->textEdit_14_requestVal->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM DEL_ENUM_VAL(:pCode);");
    query.bindValue(":pCode", idVal_14);

    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_14_requestVal->insertPlainText("Запись успешно удалена.\n");
            } else if (result_out == "0") {
                ui->textEdit_14_requestVal->insertPlainText("Ошибка удаления.\n");
            }
        }
    }
}

void MainWindow::onMyButtonClickDown_17()
{
    ui->textEdit_17_result_down->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM DOWN_VAL(:pCode);");
    query.bindValue(":pCode", idDown_17);
    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_17_result_down->insertPlainText("Запись успешно перемещена вниз.\n");
            } else if (result_out == "0") {
                ui->textEdit_17_result_down->insertPlainText("Ошибка перемещения.\n");
            }
        }
    } else {
        ui->textEdit_17_result_down->insertPlainText("Запрос не выполнен.\n");
    }
}

void MainWindow::onMyButtonClickUp_17()
{
    ui->textEdit_17_result_up->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM UP_VAL(:pCode);");
    query.bindValue(":pCode", idUp_17);

    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_17_result_up->insertPlainText("Запись успешно перемещена вверх.\n");
            } else if (result_out == "0") {
                ui->textEdit_17_result_up->insertPlainText("Ошибка перемещения.\n");
            }
        }
    } else {
        ui->textEdit_17_result_up->insertPlainText("Запрос не выполнен." + query.lastError().text() + "\n");
    }
}

void MainWindow::onMyButtonClickSearch_18()
{
    ui->tableWidget_18_result->setRowCount(0);
    int rowCount = 0;
    QSqlQuery query;
    query.prepare("SELECT * FROM FIND_LIST_ENUM(:pIdEnum) ORDER BY oIdPos;");
    query.bindValue(":pIdEnum", idEnum_18);
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_18_result->insertRow(rowCount);
            ui->tableWidget_18_result->setRowHeight(rowCount, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                if (i > 3 && record.value(i).toString() != "NULL") {
                    QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                    ui->tableWidget_18_result->setItem(rowCount, 4, item);
                } else if (i <= 3) {
                    QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                    ui->tableWidget_18_result->setItem(rowCount, i, item);
                }
            }
            ++rowCount;
        }
    }
}

void MainWindow::onMyButtonClickAdd_16()
{
    ui->textEdit_16_result->clear();
    QString maxVal = ui->lineEdit_16_maxVal->text();
    QString minVal = ui->lineEdit_16_minVal->text();
    if (maxVal.isEmpty()) {
        maxVal = "NULL";
    }
    if (minVal.isEmpty()) {
        minVal = "NULL";
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM ADD_PARAMETR_CLASS(:pIdPar, :pIdClass, :pMinVal, :pMaxVal);");
    query.bindValue(":pIdPar", paramId_16);
    query.bindValue(":pIdClass", classId_16);
    query.bindValue(":pMinVal", minVal);
    query.bindValue(":pMaxVal", maxVal);

    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(1).toString();
            if (result_out == "1")
            {
                ui->textEdit_16_result->insertPlainText("Запись успешно создана.\n");
            } else if (result_out == "0") {
                ui->textEdit_16_result->insertPlainText("Ошибка добавления.\n");
            }
        }
    }
}

void MainWindow::onMyButtonClickAdd_19()
{
    ui->textEdit_19_result->clear();
    QString type = ui->lineEdit_19_type->text();
    QString name = ui->lineEdit_19_name->text();
    QString shortName = ui->lineEdit_19_shortName->text();
    if (type.isEmpty() || type == warnEmpty) {
        ui->lineEdit_19_type->setText(warnEmpty);
    }
    if (name.isEmpty() || name == warnEmpty) {
        ui->lineEdit_19_name->setText(warnEmpty);
    }
    if (shortName.isEmpty()) {
        shortName = "NULL";
    }
    if ((!type.isEmpty() && type != warnEmpty) &&
        (!name.isEmpty() && name != warnEmpty)){
        QSqlQuery query;
        query.prepare("SELECT * FROM INS_PARAMETR(:pShName, :pName, :pIdEI, :pTypePar);");
        query.bindValue(":pIdEI", idEI_19);
        query.bindValue(":pShortName", shortName);
        query.bindValue(":pName", name);
        query.bindValue(":pTypePar", type);

        if (query.exec()) {
            while (query.next()) {
                QString idPar = query.value(0).toString();
                QString result_out = query.value(1).toString();
                if (result_out == "1")
                {
                    ui->textEdit_19_result->insertPlainText("Запись успешно создана. ID нового параметра: " + idPar + "\n");
                } else if (result_out == "0") {
                    ui->textEdit_19_result->insertPlainText("Ошибка добавления.\n");
                }
            }
        }
    }
}

void MainWindow::onMyButtonClickAdd_23()
{
    ui->textEdit_23_result->clear();
    QString val = ui->lineEdit_23_val->text();
    QString str = ui->lineEdit_23_str->text();
    QString note = ui->lineEdit_23_note->text();
    if (val.isEmpty() && str == warnEmpty) {
        return;
    }
    if (val.isEmpty()) {
        val = "NULL";
    }
    if (str.isEmpty()) {
        str = "NULL";
    }
    if (note.isEmpty()) {
        note = "NULL";
    }
        QSqlQuery query;
        if (val == "NULL") {
            query.prepare("SELECT * FROM WRITE_PAR_PROD(:pIdProd, :pIdPar, NULL, :pStr, :pEnumVal, :pNote);");
            query.bindValue(":pIdProd", prodId_23);
            query.bindValue(":pIdPar", paramId_23);
            query.bindValue(":pStr", str);
            query.bindValue(":pEnumVal", idEnum_23);
            query.bindValue(":pNote", note);
        } else {
            query.prepare("SELECT * FROM WRITE_PAR_PROD(:pIdProd, :pIdPar, :pVal, :pStr, :pEnumVal, :pNote);");
            query.bindValue(":pIdProd", prodId_23);
            query.bindValue(":pIdPar", paramId_23);
            query.bindValue(":pVal", val);
            query.bindValue(":pStr", str);
            query.bindValue(":pEnumVal", idEnum_23);
            query.bindValue(":pNote", note);
        }

        if (query.exec()) {
            while (query.next()) {
                QString result_out = query.value(0).toString();
                if (result_out == "1")
                {
                    ui->textEdit_23_result->insertPlainText("Значение успешно добавлено.\n");
                } else if (result_out == "0") {
                    ui->textEdit_23_result->insertPlainText("Ошибка добавления." + query.lastError().text() + "\n");
                }
            }
        } else {
            qDebug() << query.lastError().text();
        }
}

void MainWindow::onMyButtonClickCopy_25()
{
    ui->textEdit_25_result->clear();
    QSqlQuery query;
    query.prepare("SELECT * FROM COPY_PAR(:pIdMain, :pIdClass);");
    query.bindValue(":pIdMain", mainClass_25);
    query.bindValue(":pIdClass", classId_25);

    if (query.exec()) {
        while (query.next()) {
            QString result_out = query.value(0).toString();
            if (result_out == "1")
            {
                ui->textEdit_25_result->insertPlainText("Параметры успешно скопированы.\n");
            } else if (result_out == "0") {
                ui->textEdit_25_result->insertPlainText("Ошибка копирования.\n");
            }
        }
    }
}

void MainWindow::onMyButtonClickProd_26()
{
    ui->tableWidget_26_prod->setRowCount(0);
    int rowCount = 0;

    QSqlQuery query;
    query.prepare("SELECT * FROM FIND_PAR_PROD(:pIdProd);");
    query.bindValue(":pIdProd", prodId_26);
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_26_prod->insertRow(rowCount);
            ui->tableWidget_26_prod->setRowHeight(rowCount, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                if (i < 6) {
                    QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                    ui->tableWidget_26_prod->setItem(rowCount, i, item);
                }
                if (i > 6) {
                    QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                    ui->tableWidget_26_prod->setItem(rowCount, i - 1, item);
                }
            }
            ++rowCount;
        }
    }
}

void MainWindow::onMyButtonClickSearch_26()
{
    ui->tableWidget_26_search->setRowCount(0);
    int rowCount = 0;

    QSqlQuery query;
    query.prepare("SELECT * FROM FIND_PAR_CLASS(:pIdClass);");
    query.bindValue(":pIdClass", classId_26);
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_26_search->insertRow(rowCount);
            ui->tableWidget_26_search->setRowHeight(rowCount, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                ui->tableWidget_26_search->setItem(rowCount, i, item);
            }
            ++rowCount;
        }
    }
}

void MainWindow::onMyButtonClickSearch_28()
{
    ui->tableWidget_28_search->setRowCount(0);
    int rowCount = 0;
    QString maxVal = ui->lineEdit_28_maxVal->text();
    QString minVal = ui->lineEdit_28_minVal->text();
    if (maxVal.isEmpty()) {
        maxVal = "NULL";
    }
    if (minVal.isEmpty()) {
        minVal = "NULL";
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM SEARCH_PRODUCTS_BY_PARAMETER(:pIdParam, :pMinVal, :pMaxVal);");
    query.bindValue(":pIdParam", paramId_28);
    query.bindValue(":pMinVal", minVal);
    query.bindValue(":pMaxVal", maxVal);
    if (query.exec()) {
        while (query.next()) {
            ui->tableWidget_28_search->insertRow(rowCount);
            ui->tableWidget_28_search->setRowHeight(rowCount, 30);
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                QTableWidgetItem *item = new QTableWidgetItem(record.value(i).toString());
                qDebug() <<record.value(i).toString();
                ui->tableWidget_28_search->setItem(rowCount, i, item);
            }
            ++rowCount;
        }
    } else {
        qDebug() << query.lastError().text();
    }
}

void MainWindow::onClear4()
{
    ui->lineEdit_4_nameClass->clear();
    ui->lineEdit_4_shortName->clear();
}

void MainWindow::onClear8()
{
    ui->lineEdit_8_name->clear();
    ui->lineEdit_8_shortName->clear();
}
