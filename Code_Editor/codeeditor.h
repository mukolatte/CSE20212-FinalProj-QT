#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include "ui_codeeditor.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

//![codeeditordefinition]
class CodeEditor : public QPlainTextEdit{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void openFile(QString filePath);
    void saveFile(QString fileName);
    void ifstate();
    void ifelsestate();
    void forstate();
    void whilestate();
    void dowhilestate();
    void findString();
    void checkParen();
    QString getLineIndent();
    void findReplace(QString find, QString replace, Qt::CaseSensitivity cs);
    void findString(QString text, Qt::CaseSensitivity cs);
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE; //resize option given by example
    virtual void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE; //use for parenthesis completion
private:
    QWidget *lineNumberArea;
    bool placedDoubleCharacter;
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

};

//![codeeditordefinition]
//![extraarea]

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumberAreaPaintEvent(event);
    }
private:
    CodeEditor *codeEditor;
};

//![extraarea]

#endif
