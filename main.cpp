#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Dark Mode
    QApplication::setStyle("fusion");
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53)); // Dark background for windows
    darkPalette.setColor(QPalette::WindowText, Qt::white); // White text color
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42)); // Dark base background for text input
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66)); // Alternating background for selection
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white); // ToolTip base color
    darkPalette.setColor(QPalette::ToolTipText, Qt::black); // ToolTip text color
    darkPalette.setColor(QPalette::Text, Qt::white); // Text color
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53)); // Button background color
    darkPalette.setColor(QPalette::ButtonText, Qt::white); // Button text color
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218)); // Highlight color (blue)
    darkPalette.setColor(QPalette::HighlightedText, Qt::black); // Highlighted text color
    a.setPalette(darkPalette);

    a.setWindowIcon(QIcon(":/resources/icon.ico"));
    w.setWindowIcon(QIcon(":/icons/resources/icon.ico"));
    w.setWindowTitle("Win Command Line");
    w.show();
    return a.exec();
}
