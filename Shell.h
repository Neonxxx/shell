#ifndef SHELL_H
#define SHELL_H
#pragma once

#include <QLineEdit>
#include <QTextEdit>
#include <QKeyEvent>
#include <QDir>
#include <QScrollBar>
#include <QProcess>
#include <QStringList>
#include <QEventLoop>
#include <QTimer>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <windows.h>

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabBar>

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    QTabWidget *tabWidget;
    QPushButton *addButton;
    QStringList *_tabContent;
    std::vector<QDir> *_currentDir;

    TabWidget(QStringList *tabContent, std::vector<QDir> *currentDir, QWidget *parent = nullptr) : QWidget(parent), _tabContent(tabContent), _currentDir(currentDir) {
        // Set up the main layout for the widget
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        tabWidget = new QTabWidget(this);

        // Create a horizontal layout for the tabs and the "Add Tab" button
        QHBoxLayout *tabLayout = new QHBoxLayout();

        // Create the "Add Tab" button
        addButton = new QPushButton("+", this);
        addButton->setFixedSize(40, 30);  // Size of the add button
        connect(addButton, &QPushButton::clicked, this, &TabWidget::onAddTabClicked);

        // Add the tab widget and button layout to the main layout
        tabLayout->addWidget(tabWidget);
        tabLayout->addWidget(addButton);

        // Add the horizontal layout to the main layout
        mainLayout->addLayout(tabLayout);

        // Set the initial tab position with the button always on the right
        tabWidget->setTabsClosable(true);  // Optionally allow tab closing
        connect(tabWidget, &QTabWidget::tabCloseRequested, this, &TabWidget::closeTab);
    }

public slots:
    void onAddTabClicked() {
        // Add a new tab when the "Add Tab" button is clicked
        QWidget *newTab = new QWidget();
        tabWidget->addTab(newTab, "Terminal");
        _tabContent->push_back(QString());
        _currentDir->push_back(QDir::currentPath());
        // Optionally, you can also set the tab's content, layout, or other properties
    }

    void closeTab(int index) {
        if (tabWidget->count() <= 1)
            return;
        // Close the tab at the given index
        _tabContent->erase(_tabContent->begin() + index);
        _currentDir->erase(_currentDir->begin() + index);
        tabWidget->removeTab(index);
    }

private:
};


class InputField : public QLineEdit
{
public:
    int currentLineIdx = 0;
    QStringList &_fileLines;
    QTextEdit *_space;
    InputField(bool &nano, QStringList &fileLines, QTextEdit *space, QWidget *parent = nullptr)
        : QLineEdit(parent), _nano(nano), _fileLines(fileLines), _space(space) {}

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if (!_nano)
        {
            if (event->key() == Qt::Key_Up)
                undo();
            else if (event->key() == Qt::Key_Down)
                redo();
            else
                QLineEdit::keyPressEvent(event);
        }
        else
        {
            if (event->key() == Qt::Key_Down)
            {
                if (currentLineIdx < _fileLines.size() -1 )
                {
                    _fileLines[currentLineIdx] = this->text();
                    _space->setPlainText(_fileLines.join("\n"));
                    _space->setHtml(_space->toHtml());
                    currentLineIdx++;
                    this->setText(_fileLines[currentLineIdx]);
                }
            }
            else if (event->key() == Qt::Key_Up)
            {
                if (currentLineIdx > 0)
                {
                    _fileLines[currentLineIdx] = this->text();
                    _space->setPlainText(_fileLines.join("\n"));
                    _space->setHtml(_space->toHtml());
                    currentLineIdx--;
                    this->setText(_fileLines[currentLineIdx]);
                }
            }
            else
                QLineEdit::keyPressEvent(event);
        }
    }
private:
    bool &_nano;
};



class Shell : public QWidget
{
     Q_OBJECT
public:
    explicit Shell(QWidget *parent = nullptr) : QWidget(parent) {
        rightsTextEdit = new QTextEdit(parent);
        rightsTextEdit->setStyleSheet("font-size: 25px");
        rightsTextEdit->setGeometry(0,parent->height() - 50,50,50);
        rightsTextEdit->setReadOnly(true);
        if (!isUserAnAdmin())
            rightsTextEdit->setHtml("<span style='color: red'>$$</span>");
        else
            rightsTextEdit->setHtml("<span style='color: cyan'>>></span>");

        tabs = new TabWidget(&tabContent,&currentDir,parent);
        tabs->setGeometry(0,0,parent->width(),50);
        tabs->tabWidget->addTab(new QWidget(),"Terminal");
        tabContent.push_back(QString()); // first tab
        connect(tabs->tabWidget,&QTabWidget::currentChanged,this,[=](){
            space->setHtml(tabContent[tabs->tabWidget->currentIndex()]);
        });

        space = new QTextEdit(parent);
        space->setStyleSheet("font-size: 15px");
        space->setGeometry(0,40,parent->width(),parent->height() - 90);
        space->setReadOnly(true);
        space->setHtml("<span style='color: lime'>----------------------------------------------------------------------------<br>"
                       "| Shell booted up succesfully ...<br>| Start by writing down commands in the input field below.<br>"
                       "----------------------------------------------------------------------------</span>");

        inputField = new InputField(nano,fileLines,space,parent);
        inputField->setFocus();
        inputField->setStyleSheet("font-size: 15px");
        inputField->setGeometry(40,space->height(),parent->width() - 40,50);
        connect(inputField, &QLineEdit::returnPressed, this, [=]() {
            if (!nano)
            {
                if (inputField->text().isEmpty())
                    space->append("<span style='color: lime'>" + currentDir[tabs->tabWidget->currentIndex()].path() + "</span><br>");
                    //space->setHtml(space->toHtml() + currentDir.path() + "\n");
                else
                {
                    space->append("<span style='color: white'>" + inputField->text() + "</span>");
                    //space->setHtml(space->toHtml() + inputField->text() + "\n");

                    QString returnMessage = execute(inputField->text()).toUtf8();
                    if (!returnMessage.isEmpty())
                        space->append("<span style='color: lime' charset='UTF-8'>" + QString::fromUtf8(returnMessage.toStdString()) + "</span><br>");
                        //space->setHtml(space->toHtml() + returnMessage + "\n");

                    inputField->clear();
                }
                Sleep(1);
                space->updateGeometry();
                space->update();
                QScrollBar *bar = space->verticalScrollBar();
                bar->setValue(bar->maximum());
                tabContent[tabs->tabWidget->currentIndex()] = space->toHtml();
            }
            else
            {
                if (inputField->text().isEmpty())
                {
                    space->setHtml(space->toHtml() + "<div></div>");
                    fileLines.push_back("\n");
                }
                else
                {
                    space->setHtml(space->toHtml() + "<span style='color: cyan'>" + inputField->text() + "</span>");
                    fileLines.push_back(inputField->text());
                }
                inputField->clear();
            }
        });

        updateTimer = new QTimer(this);
        connect(updateTimer,&QTimer::timeout,this,[=](){
            tabContent[tabs->tabWidget->currentIndex()] = space->toHtml();
        });
        updateTimer->start(1);
    };

    // objects
    InputField *inputField;
    QTextEdit *space;
    QTextEdit *rightsTextEdit;
    TabWidget *tabs;

    //variables
    std::vector<QDir> currentDir = {QDir::currentPath()};
    QTimer *updateTimer;

protected:
    void Sleep(const int &time_to_wait)
    {
        QEventLoop loop;
        QTimer::singleShot(time_to_wait, &loop, SLOT(quit()));
        loop.exec();
    }

    void editFileUsingNano(const QString &filename);

    bool isUserAnAdmin() {
        BOOL isElevated = FALSE;
        HANDLE tokenHandle = NULL;

        // Open the access token associated with the current process
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tokenHandle)) {
            TOKEN_ELEVATION elevation;
            DWORD size;

            // Get the elevation information
            if (GetTokenInformation(tokenHandle, TokenElevation, &elevation, sizeof(elevation), &size)) {
                isElevated = elevation.TokenIsElevated;
            }

            CloseHandle(tokenHandle);
        }

        return isElevated == TRUE;
    }

private:
    QStringList tabContent;
    QStringList fileLines;
    bool nano = false;
    QString execute(const QString &command);
};

#endif // SHELL_H
