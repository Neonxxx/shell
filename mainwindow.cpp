#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(QSize(this->width() + 400,this->height()));
    shell = new Shell(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
