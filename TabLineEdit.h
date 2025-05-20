#ifndef TABLINEEDIT_H
#define TABLINEEDIT_H
#pragma once

#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>
#include <QDir>
#include <QRegularExpression>

class ShellLineEdit : public QLineEdit {
public:
    using QLineEdit::QLineEdit;

    int &tab_idx;
    std::vector<QDir> &currentDir;

    ShellLineEdit(std::vector<QDir> &_currentDir, int &_tab_idx, QWidget *parent = nullptr)
        : QLineEdit(parent), tab_idx(_tab_idx), currentDir(_currentDir)
    {
        connect(this, &QLineEdit::textEdited, this, [this]() {
            resetCompletionState();
        });
    }

protected:
    bool event(QEvent *e) override {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
            if (keyEvent->key() == Qt::Key_Tab) {
                handleTabCompletion();
                return true;
            }
        }
        return QLineEdit::event(e);
    }

    void focusInEvent(QFocusEvent *event) override {
        QLineEdit::focusInEvent(event);
        resetCompletionState(); // reset when switching to this line edit
    }

private:
    QStringList matches;
    int matchIndex = -1;
    QString lastPrefix;
    QString baseCompletion;
    int lastTabIdx = -1; // NEW: store the last used tab index

    void resetCompletionState() {
        matchIndex = -1;
        lastPrefix.clear();
        matches.clear();
        baseCompletion.clear();
        lastTabIdx = -1;
    }

    void handleTabCompletion() {
        QString input = this->text().trimmed();

        // Reset if input is empty or tab index changed
        if (input.isEmpty() || lastTabIdx != tab_idx) {
            resetCompletionState();
        }

        if (matches.isEmpty()) {
            QStringList words = input.split(' ');
            if (words.isEmpty())
                return;

            QString pattern = words.last();
            QString commandPrefix = words.mid(0, words.size() - 1).join(' ');
            if (!commandPrefix.isEmpty())
                commandPrefix += " ";

            QString path;
            int lastSlash = pattern.lastIndexOf('/');
            if (lastSlash != -1) {
                path = pattern.left(lastSlash + 1);
                pattern = pattern.mid(lastSlash + 1);
            }

            QDir baseDir = currentDir[tab_idx];
            QDir dir = !path.isEmpty() ? QDir(baseDir.filePath(path)) : baseDir;

            if (!dir.exists())
                return;

            QStringList entries = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
            QRegularExpression re("^" + QRegularExpression::escape(pattern), QRegularExpression::CaseInsensitiveOption);
            matches = entries.filter(re);

            if (matches.isEmpty())
                return;

            matchIndex = -1;
            lastPrefix = pattern;
            baseCompletion = commandPrefix + path;
            lastTabIdx = tab_idx;  // NEW: remember current tab index
        }

        if (!matches.isEmpty()) {
            matchIndex = (matchIndex + 1) % matches.size();
            QString completed = baseCompletion + matches[matchIndex];
            this->setText(completed);
        }

        qDebug() << "Tab completion:"
                 << " base='" << baseCompletion
                 << "' prefix='" << lastPrefix
                 << "' match=" << matchIndex
                 << "' tab=" << tab_idx
                 << " current text=" << this->text();
    }
};

#endif // TABLINEEDIT_H
