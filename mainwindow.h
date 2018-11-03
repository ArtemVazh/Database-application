#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstring>
#include <QApplication>
#include <iostream>
#include <QtSql/QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <string>
#include <QDebug>
#include "QStandardItemModel"
#include "QStandardItem"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <iterator>
#include <QSizePolicy>
#include <QGridLayout>
#include <QShortcut>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSqlDatabase db;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void Connection();
    void ChooseTable(QModelIndex);
    void Output();
    void OutputAll();
    void Connect1();
    void Connect2();
    void Connect3();
    void Connect4();
    void Connect5();
    void Connect6();
    void LoadBuffer();
private:
    std::string filename;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
