#include "Shell.h"

QString Shell::execute(const QString &command)
{
    if (!nano)
    {
        if (command.startsWith("cd "))
        {
            QString path = command.split("cd ")[1];
            if (currentDir[tabs->tabWidget->currentIndex()].cd(path))
                return "<span style='color: lime'>Entered Folder " + currentDir[tabs->tabWidget->currentIndex()].path() + "</span>";
            else
                return "<span style='color: red'>Path not found!</span>";
        }
        else if (command.startsWith("ls"))
        {
            QStringList files = currentDir[tabs->tabWidget->currentIndex()].entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
            if (!files.empty())
            {
                files.push_front("<span style='color: cyan'>");
                return files.join("\t").replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;");
            }
            else
                return "<span style='color: red'>Folder empty!</span>";
        }
        else if (command.startsWith("whoami"))
        {
            QString username = QProcessEnvironment::systemEnvironment().value("USERNAME");
            return "<span style='color: lime'>" + username + "</span>";
        }
        else if (command.startsWith("mkdir "))
        {
            QString folder = command.split("mkdir ")[1];
            if (currentDir[tabs->tabWidget->currentIndex()].mkdir(folder))
                return "<span style='color: lime'>Directory created successfully!</span>";
            else if (currentDir[tabs->tabWidget->currentIndex()].exists(folder))
                return "<span style='color: red'>ERROR: Directory already exists!</span>";
            return "<span style='color: red'>ERROR: Failed to create Directory!</span>";
        }
        else if (command.startsWith("rmdir "))
        {
            if (currentDir[tabs->tabWidget->currentIndex()].rmdir(command.split("rmdir ")[1]))
                return "<span style='color: lime'>Deleted Directory successfully!</span>";
            return "<span style='color: red'>ERROR: Failed to delete Directory!</span>";
        }
        else if (command.startsWith("wlanpwd")) {
            QProcess *process = new QProcess(this);  // Make process persistent

            QStringList args = command.split(" ");
            args.pop_front();  // Remove "wlanpwd" from the arguments

            QStringList netshArgs;
            if (args.isEmpty()) {
                netshArgs << "wlan" << "show" << "profile";
            } else {
                netshArgs << "wlan" << "show" << "profile" << args[0] << "key=clear";
            }

            qDebug() << "Executing command: netsh" << netshArgs;

            process->start("netsh", netshArgs);

            if (!process->waitForStarted())
                space->append("<span style='color: red'>Error: Process failed to start.</span>");

            // Read standard output
            connect(process, &QProcess::readyReadStandardOutput, [this, process]() {
                QString output = QString::fromUtf8(process->readAllStandardOutput());
                space->append("<span style='color: lime'>" + output + "</span>");
            });

            // Read standard error
            connect(process, &QProcess::readyReadStandardError, [this, process]() {
                QString errorOutput = QString::fromUtf8(process->readAllStandardError());
                if (!errorOutput.isEmpty()) {
                    space->append("<span style='color: red'>Error: " + errorOutput + "</span>");
                }
            });
        }
        else if (command == "clear")
            space->clear();
        else if (command.startsWith("nano "))
        {
            QString fileName = command.split("nano ")[1];
            editFileUsingNano(fileName);
        }
        else {
            QProcess *process = new QProcess(this); // Make process persistent

            // Run the command in cmd.exe
            process->setWorkingDirectory(currentDir[tabs->tabWidget->currentIndex()].path());
            process->start("cmd.exe", QStringList() << "/c" << command);

            if (!process->waitForStarted())
                space->append("<span style='color: red'>Error: Process failed to start.</span>");

            // Read standard output
            connect(process, &QProcess::readyReadStandardOutput, [this, process]()
            {
                QString output = QString::fromUtf8(process->readAllStandardOutput());
                space->append("<span style='color: lime'>" + output.replace("\n","<br>") + "</span>");
            });

            // Read standard error
            connect(process, &QProcess::readyReadStandardError, [this, process]()
            {
                QString errorOutput = QString::fromUtf8(process->readAllStandardError());
                if (!errorOutput.isEmpty())
                    space->append("<span style='color: red'>Error: " + errorOutput + "</span>");
            });

            // Do NOT call process.waitForFinished(); Let Qt handle it asynchronously.
        }
    }
    return "";
};

void Shell::editFileUsingNano(const QString &filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadWrite))
    {
        nano = true;
        fileLines = QString(file.readAll()).split('\n');
        space->setPlainText(fileLines.join("\n"));
        space->setHtml("<span style='color: cyan'>" + space->toHtml() + "</span>");
        if (fileLines.size() > 0)
            inputField->setText(fileLines[0]);
    }
}
