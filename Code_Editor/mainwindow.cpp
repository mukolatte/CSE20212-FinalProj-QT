#include <QtWidgets/QWidget>
#include "mainwindow.h"
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QTextStream>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QString>
#include <QMessageBox>
#include <QStatusBar>
#include <QFileDialog>
#include <QCloseEvent>
#include <QTextStream>
#include <QFile>
#include <QApplication>
#include <QTextCursor>
#include <QSettings>
#include <QTextCharFormat>
#include <QDockWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QTreeWidget>
#include <QSplitter>
#include <iostream>
#include <QAbstractItemView>
#include <QPalette>
#include <QColor>
#include <QtDebug>
#include <QDir>
#include <QIcon>
#include <QInputDialog>

using namespace std;

MainWindow::MainWindow(): QMainWindow(){
    ui = new Ui::codeeditor;
    ui -> setupUi(this);
    editor = new CodeEditor(this);
    highlighter = new SyntaxHighlighter(editor->document());

    fileIsOpened = false;
    editorName = QString("Galeanthropy");

    ui->actionQuit     -> connect(ui->actionQuit,    SIGNAL(triggered()), this,   SLOT(close()));
    ui->actionCopy     -> connect(ui->actionCopy,    SIGNAL(triggered()), editor, SLOT(copy()));
    ui->actionCut      -> connect(ui->actionCut,     SIGNAL(triggered()), editor, SLOT(cut()));
    ui->actionPaste    -> connect(ui->actionPaste,   SIGNAL(triggered()), editor, SLOT(paste()));
    ui->actionQuit     -> connect(ui->actionQuit,    SIGNAL(triggered()), editor, SLOT(selectAll()));
    ui->actionUndo     -> connect(ui->actionUndo,    SIGNAL(triggered()), editor, SLOT(undo()));
    ui->actionRedo     -> connect(ui->actionRedo,    SIGNAL(triggered()), editor, SLOT(redo()));
    ui->actionOpen     -> connect(ui->actionOpen,    SIGNAL(triggered()), this,   SLOT(openDialog()));
    ui->actionSave_As  -> connect(ui->actionSave_As, SIGNAL(triggered()), this,   SLOT(saveAsDialog()));
    ui->actionSave     -> connect(ui->actionSave,    SIGNAL(triggered()), this,   SLOT(saveDialog()));
    ui->actionIf       -> connect(ui->actionIf,      SIGNAL(triggered()), this,   SLOT(ifstate()));
    ui->actionIf_Else  -> connect(ui->actionIf_Else, SIGNAL(triggered()), this,   SLOT(ifelsestate()));
    ui->actionFor      -> connect(ui->actionFor,     SIGNAL(triggered()), this,   SLOT(forstate()));
    ui->actionWhile    -> connect(ui->actionWhile,   SIGNAL(triggered()), this,   SLOT(whilestate()));
    ui->actionDo_While -> connect(ui->actionDo_While,SIGNAL(triggered()), this,   SLOT(dowhilestate()));

    setCentralWidget(editor);
    //setWindowTitle(tr(editorName) + QString(" | ") + tr("untitled"));
    show();

//    dialog = new FindDialog(this);

//    ui-> actionFind_and_Replace -> connect(dialog, SIGNAL(findPrevious(QString, Qt::CaseSensitivity)), this, SLOT(findup(QString, Qt::CaseSensitivity)));
//    ui-> actionFind_and_Replace -> connect(dialog, SIGNAL(findNext(QString, Qt::CaseSensitivity)), this, SLOT(finddown(QString, Qt::CaseSensitivity)));
//    ui-> actionFind_and_Replace -> connect(dialog, SIGNAL(replace(QString)), this, SLOT(replacetext(QString)));

}

MainWindow::~MainWindow(){
    delete highlighter;
    delete editor;
    delete ui;
}

void MainWindow::openDialog() {
    QString tmpFilePath = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (!tmpFilePath.isEmpty()) {
        filePath = tmpFilePath;
        //setWindowTitle(tr(editorName) + QString(" | ") + filePath);
        fileIsOpened = true;
        editor->openFile(filePath);
    }
}

void MainWindow::saveDialog() {
    if (fileIsOpened) {
        editor->saveFile(filePath);
    } else {
        saveAsDialog();
    }
}

void MainWindow::saveAsDialog(){
    QString tmpFilePath = QFileDialog::getSaveFileName(this, tr("Save File"), "",tr("File Type (*.txt);;C++ File (*.cpp *.h)"));
    if (!tmpFilePath.isEmpty()){
        filePath = tmpFilePath;
        //setWindowTitle(tr(editorName) + QString(" | ") + filePath);
        fileIsOpened = true;
        editor->saveFile(filePath);
    }
}

void MainWindow::ifstate(){
    editor->ifstate();
}

void MainWindow::ifelsestate(){
    editor->ifelsestate();
}

void MainWindow::forstate(){
    editor->forstate();
}

void MainWindow::whilestate(){
    editor->whilestate();
}

void MainWindow::dowhilestate(){
    editor->dowhilestate();
}
