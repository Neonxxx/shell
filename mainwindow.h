#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Shell.h"

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

    void resizeEvent(QResizeEvent *event) override
    {
        QMainWindow::resizeEvent(event); // Call base class implementation (optional)
        shell->space->setGeometry(shell->space->x(),shell->space->y(),this->width(),this->height() - 90);
        shell->inputField->setGeometry(40,shell->space->height() + 40,this->width(),50);
        shell->rightsTextEdit->setGeometry(0,shell->space->height() + 40,50,50);
        shell->tabs->setGeometry(0,0,this->width(),50);
    }

private:
    Ui::MainWindow *ui;

    Shell *shell;
};
#endif // MAINWINDOW_H
