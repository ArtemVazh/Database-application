#include "mainwindow.h"
#include <QApplication>
#include <QBrush>
#include <QPalette>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    SetConsoleCP(CP_UTF8);
//    SetConsoleOutputCP(CP_UTF8);
    MainWindow wgt;
    wgt.setWindowTitle("DataBase Application");
    QBrush br(Qt::TexturePattern);
    br.setTextureImage(QImage("987.jpg"));
    QPalette plt = wgt.palette();
    plt.setBrush(QPalette::Background, br);
    wgt.setPalette(plt);
    wgt.show();
    return a.exec();
}
