#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::ifstream Settings("SettingsDB.txt");
    filename = "Bufer.txt";
    ui->lineEdit->setText(filename.c_str());
    LoadBuffer();
    std::vector<std::string> set(6);
    for(int i = 0; i < 6; ++i){
        Settings >> set[i];
    }
    if(set[0] == "QSQLITE") {
        db = QSqlDatabase::addDatabase("QSQLITE");
        ui->lineEdit_DB->setText(set[0].c_str());
        ui->lineEdit_DN->setText(set[2].c_str());
        db.setDatabaseName(set[2].c_str());
        ui->lineEdit_HN->setDisabled(true);
        ui->lineEdit_Pas->setDisabled(true);
        ui->lineEdit_Port->setDisabled(true);
        ui->lineEdit_UN->setDisabled(true);
    }
    else {
        db = QSqlDatabase::addDatabase(set[0].c_str());
        db.setHostName(set[1].c_str());
        db.setDatabaseName(set[2].c_str());
        db.setUserName(set[3].c_str());
        db.setPassword(set[4].c_str());
        db.setPort(atoi(set[5].c_str()));
        ui->lineEdit_DB->setText(set[0].c_str());
        ui->lineEdit_HN->setText(set[1].c_str());
        ui->lineEdit_DN->setText(set[2].c_str());
        ui->lineEdit_UN->setText(set[3].c_str());
        ui->lineEdit_Pas->setText(set[4].c_str());
        ui->lineEdit_Port->setText(set[5].c_str());
    }
    db.open();
    if (db.open()) {
        ui->textEdit_2->moveCursor(QTextCursor::Start);
        ui->textEdit_2->insertPlainText("Соединение установлено!\n");
        OutputAll();
    }
    else {
        ui->textEdit_2->moveCursor(QTextCursor::Start);
        ui->textEdit_2->insertPlainText("Соединение не установлено!\n");
        ui->textEdit_2->insertPlainText(db.lastError().text() + "\n");
    }
    QShortcut *Ctrl_UP;
    Ctrl_UP = new QShortcut(Qt::CTRL + Qt::Key_Up, this);
    connect(Ctrl_UP,SIGNAL(activated()),this,SLOT(Output()));
    connect(Ctrl_UP,SIGNAL(activated()),this,SLOT(OutputAll()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Output()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(LoadBuffer()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OutputAll()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Connection()));
    connect(ui->textEdit, SIGNAL(returnPressed()), SLOT(Output()));
    connect(ui->textEdit, SIGNAL(returnPressed()), SLOT(OutputAll()));
    connect(ui->tableView_2,SIGNAL(clicked(QModelIndex)),this,SLOT(ChooseTable(QModelIndex)));
    connect(ui->lineEdit_DB, SIGNAL(returnPressed()), SLOT(Connect1()));
    connect(ui->lineEdit_HN, SIGNAL(returnPressed()), SLOT(Connect2()));
    connect(ui->lineEdit_DN, SIGNAL(returnPressed()), SLOT(Connect3()));
    connect(ui->lineEdit_UN, SIGNAL(returnPressed()), SLOT(Connect4()));
    connect(ui->lineEdit_Pas, SIGNAL(returnPressed()), SLOT(Connect5()));
    connect(ui->lineEdit_Port, SIGNAL(returnPressed()), SLOT(Connect6()));
    OutputAll();
    ui->centralWidget->setLayout(ui->horizontalLayout_12);
    ui->centralWidget->setContentsMargins(0,10,10,0);
    ui->pushButton->setStyleSheet(
                "QPushButton{"
                "background-color: rgb(245, 255, 250);"
                "border-radius: 5px;"
                "border: 2px solid navy;}"
                "QPushButton:hover{"
                "background-color: rgb(242,242,242);} "
                "QPushButton:pressed  {"
                "background-color: rgb(222,222,222); } "
                );
    ui->pushButton_2->setStyleSheet(
                "QPushButton{"
                "background-color: rgb(245, 255, 250);"
                "border-radius: 5px;"
                "border: 2px solid navy;}"
                "QPushButton:hover{"
                "background-color: rgb(242,242,242);} "
                "QPushButton:pressed  {"
                "background-color: rgb(222,222,222); } "
                );
    ui->pushButton_3->setStyleSheet(
                "QPushButton{"
                "background-color: rgb(245, 255, 250);"
                "border-radius: 5px;"
                "border: 2px solid navy;}"
                "QPushButton:hover{"
                "background-color: rgb(242,242,242);} "
                "QPushButton:pressed  {"
                "background-color: rgb(222,222,222); } "
                );
    ui->textEdit->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadBuffer() {
    ui->textEdit_3->clear();
    QString buffer = ui->lineEdit->text();
    filename = buffer.toStdString();
    std::ifstream file(filename);
    if(!file) {
        ui->textEdit_2->moveCursor(QTextCursor::Start);
        ui->textEdit_2->insertPlainText("Файл не открыт! (файл с таким именем будет создан при вводе данных)\n");
        return;
    }
    std::string data;
    while(std::getline(file, data)) {
        ui->textEdit_3->moveCursor(QTextCursor::End);
        ui->textEdit_3->insertPlainText(data.c_str());
        ui->textEdit_3->insertPlainText("\n");
    }
    ui->lineEdit->setText(filename.c_str());
}

void MainWindow::Output()
{
    QSqlQuery queryCore;
    QString query = ui->textEdit->toPlainText();
    std::ofstream file(filename, std::ios::app);
    ui->textEdit->clear();
    queryCore.exec(query.toStdString().c_str());
    file.seekp(0);
    if (queryCore.isActive()){
        ui->textEdit_2->moveCursor(QTextCursor::Start);
        ui->textEdit_2->insertPlainText("Ok\n");
        ui->textEdit_3->moveCursor(QTextCursor::End);
        ui->textEdit_3->insertPlainText(query.toStdString().c_str());
        ui->textEdit_3->insertPlainText("\n\n\n");
        file << query.toStdString() << "\n\n";
    }
    else{
        if(queryCore.lastError().text().toStdString().size() < 3){
            ui->textEdit_2->moveCursor(QTextCursor::Start);
            ui->textEdit_2->insertPlainText("empty query\n");
            ui->textEdit_3->moveCursor(QTextCursor::End);
            ui->textEdit_3->insertPlainText(query.toStdString().c_str());
            ui->textEdit_3->insertPlainText("\n\n\n");
            file << query.toStdString() << "\n\n";
        }
        else {
            ui->textEdit_2->moveCursor(QTextCursor::Start);
            ui->textEdit_2->insertPlainText((queryCore.lastError().text().toStdString() + "\n").c_str());
            ui->textEdit_3->moveCursor(QTextCursor::End);
            ui->textEdit_3->insertPlainText(query.toStdString().c_str());
            ui->textEdit_3->insertPlainText("\n\n\n");
            file << query.toStdString() << "\n\n";
        }
    }
    ui->textEdit_3->moveCursor(QTextCursor::End);
    if (queryCore.isSelect()){
        QSqlQueryModel *table = new  QSqlQueryModel;
        table->setQuery(queryCore);
        ui->tableView->setModel(table);
        ui->tableView->resizeRowsToContents();
        ui->tableView->resizeColumnsToContents();
        }
}

void MainWindow::OutputAll() {
    QStandardItem *item;
    QStandardItemModel *model = new QStandardItemModel;
    QStringList horizontalHeader;
    horizontalHeader.append("Названия:");
    model->setHorizontalHeaderLabels(horizontalHeader);
    for(int i=0; i < db.tables().size(); ++i) {
        item = new QStandardItem;
        item->setText(db.tables()[i].toStdString().c_str());
        model->setItem(i, 0, item);
    }
    ui->tableView_2->setModel(model);
    ui->tableView_2->resizeRowsToContents();
    ui->tableView_2->resizeColumnsToContents();
}

void MainWindow::ChooseTable(QModelIndex newItem) {
    QSqlTableModel *tab1;
    tab1 = new QSqlTableModel();
    tab1->setTable(newItem.data().toString());
    tab1->select();
    ui->tableView->setModel(tab1);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::Connection(){
    std::vector<std::string> settings(5);
    settings[0] = ui->lineEdit_DB->text().toStdString();
    settings[1] = ui->lineEdit_HN->text().toStdString();
    settings[2] = ui->lineEdit_DN->text().toStdString();
    settings[3] = ui->lineEdit_UN->text().toStdString();
    settings[4] = ui->lineEdit_Pas->text().toStdString();
    settings[5] = ui->lineEdit_Port->text().toStdString();
    std::ofstream Set("SettingsDB.txt", std::ios::trunc);
    if(settings[0] == "QSQLITE") {
        db.close();
        db.removeDatabase(db.connectionName().toStdString().c_str());
        db = QSqlDatabase::addDatabase("QSQLITE");
        Set << "QSQLITE" << "\n";
        ui->lineEdit_DB->setText(settings[0].c_str());
        ui->lineEdit_DN->setText(settings[2].c_str());
        db.setDatabaseName(settings[2].c_str());
        Set << settings[1] << "\n";
        ui->lineEdit_HN->setDisabled(true);
        Set << "-" << "\n";
        ui->lineEdit_Pas->setDisabled(true);
        Set << "-" << "\n";
        ui->lineEdit_Port->setDisabled(true);
        Set << "-" << "\n";
        ui->lineEdit_UN->setDisabled(true);
        Set << "-" << "\n";
    }
    else {
        db.close();
        db.removeDatabase(db.connectionName());
        db = QSqlDatabase::addDatabase(settings[0].c_str());
        Set << settings[0] << "\n";
        db.setHostName(settings[1].c_str());
        Set << settings[1] << "\n";
        db.setDatabaseName(settings[2].c_str());
        Set << settings[2] << "\n";
        db.setUserName(settings[3].c_str());
        Set << settings[3] << "\n";
        db.setPassword(settings[4].c_str());
        Set << settings[4] << "\n";
        db.setPort(atoi(settings[5].c_str()));
        Set << settings[5] << "\n";
        ui->lineEdit_DB->setText(settings[0].c_str());
        ui->lineEdit_HN->setText(settings[1].c_str());
        ui->lineEdit_DN->setText(settings[2].c_str());
        ui->lineEdit_UN->setText(settings[3].c_str());
        ui->lineEdit_Pas->setText(settings[4].c_str());
        ui->lineEdit_Port->setText(settings[5].c_str());
    }
    db.open();
    if (db.open()) {
        ui->textEdit_2->moveCursor(QTextCursor::Start);
        ui->textEdit_2->insertPlainText("Соединение установлено!\n");
        OutputAll();
        ui->textEdit->setFocus();
    }
    else {
        ui->textEdit_2->moveCursor(QTextCursor::Start);
        ui->textEdit_2->insertPlainText("Соединение не установлено!\n");
        ui->lineEdit_DB->setFocus();
    }
}

void MainWindow::Connect1(){
    std::string DB = ui->lineEdit_DB->text().toStdString();
    ui->lineEdit_DB->setText(DB.c_str());
      if(DB == "QSQLITE") {
          ui->lineEdit_HN->setDisabled(true);
          ui->lineEdit_Pas->setDisabled(true);
          ui->lineEdit_Port->setDisabled(true);
          ui->lineEdit_UN->setDisabled(true);
          ui->lineEdit_DN->setFocus();
      }
      else {
          ui->lineEdit_HN->setDisabled(false);
          ui->lineEdit_Pas->setDisabled(false);
          ui->lineEdit_Port->setDisabled(false);
          ui->lineEdit_UN->setDisabled(false);
      }
     ui->lineEdit_HN->setFocus();
}

void MainWindow::Connect2(){
     ui->lineEdit_DN->setFocus();
}

void MainWindow::Connect3(){
    std::string DB = ui->lineEdit_DB->text().toStdString();
    ui->lineEdit_DB->setText(DB.c_str());
    if(DB == "QSQLITE") {
        ui->pushButton_2->click();
    }
    else
        ui->lineEdit_UN->setFocus();
}

void MainWindow::Connect4(){
     ui->lineEdit_Pas->setFocus();
}

void MainWindow::Connect5(){
     ui->lineEdit_Port->setFocus();
}

void MainWindow::Connect6(){
    ui->pushButton_2->click();
}
