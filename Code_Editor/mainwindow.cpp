#include <QtWidgets/QWidget>
#include "mainwindow.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QDialogButtonBox>
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

    font = new QFont;
    font->setFamily("Courier");
    font->setStyleHint(QFont::Monospace);
    font->setFixedPitch(true);
    font->setPointSize(11);
    editor->setFont(*font);
    editor->setTabStopWidth(20);

    fileIsOpened = false;
    editorName = QString("Galeanthropy");
    //this next block of code connects all of the functions (SLOT) to the corresponding buttons(ui->buttonName) on the UI
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
    ui->actionNew      -> connect(ui->actionNew,     SIGNAL(triggered()), this,   SLOT(newFile()));
    ui->actionModify_Active_Syntax_Rules -> connect(ui->actionModify_Active_Syntax_Rules,SIGNAL(triggered()), this,SLOT(checkParen()));
    ui->actionFind_and_Replace -> connect(ui->actionFind_and_Replace, SIGNAL(triggered()), this, SLOT(findReplace()));
    ui->actionFind -> connect(ui->actionFind, SIGNAL(triggered(bool)), this, SLOT(find()));

    setCentralWidget(editor);
    setWindowTitle(QString("%1 | %2").arg(editorName).arg(tr("untitled")));
    show();
}

MainWindow::~MainWindow(){
    delete highlighter;
    delete editor;
    delete font;
    delete ui;
}

void MainWindow::openDialog() {
    QMessageBox::StandardButton shouldSave;
    shouldSave = QMessageBox::question(this, "Save Document","This document has not been saved.",QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
    if (shouldSave == QMessageBox::Save) {
        saveDialog();
    } else if (shouldSave == QMessageBox::Ignore) {
        QString tmpFilePath = QFileDialog::getOpenFileName(this, tr("Open File"));
        if (!tmpFilePath.isEmpty()) {
            filePath = tmpFilePath;
            setWindowTitle(QString("%1 | %2").arg(editorName).arg(filePath.section('/',-1)));
            fileIsOpened = true;
            editor->openFile(filePath);
        }
    }
}
void MainWindow::checkParen(){
    editor->checkParen();
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
        setWindowTitle(QString("%1 | %2").arg(editorName).arg(filePath.section('/',-1)));
        fileIsOpened = true;
        editor->saveFile(filePath);
    }
}

void MainWindow::newFile() {
    fileIsOpened = false;
    QMessageBox::StandardButton shouldSave;
    shouldSave = QMessageBox::question(this, "Save Document","This document has not been saved.",QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
    if (shouldSave == QMessageBox::Save) {
        saveDialog();
    } else if (shouldSave == QMessageBox::Ignore) {
        setWindowTitle(QString("%1 | %2").arg(editorName).arg(tr("untitled")));
        editor->clear();
    }
}

void MainWindow::find(){
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Input Word for Find/Replace:"));

    // Add the lineEdits with their respective labels
    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QString label1 = QString("Find");
    form.addRow(label1, lineEdit1);
    QCheckBox *checkbox = new QCheckBox("C&ase sensitive", &dialog);
    form.addRow(checkbox);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        editor->findString(lineEdit1->text(),(checkbox->checkState() == Qt::Checked) ? Qt::CaseSensitive : Qt::CaseInsensitive);
    }
}

void MainWindow::findReplace(){
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Input Word for Find/Replace:"));

    // Add the lineEdits with their respective labels
    QList<QLineEdit *> fields;
    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QString label1 = QString("Find");
    form.addRow(label1, lineEdit1);
    fields << lineEdit1;
    QLineEdit *lineEdit2 = new QLineEdit(&dialog);
    QString label2 = QString("Replace");
    form.addRow(label2, lineEdit2);
    fields << lineEdit2;
    QCheckBox *checkbox = new QCheckBox("C&ase sensitive", &dialog);
    form.addRow(checkbox);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        editor->findReplace(lineEdit1->text(),lineEdit2->text(), (checkbox->checkState() == Qt::Checked) ? Qt::CaseSensitive : Qt::CaseInsensitive);
    }
}
//these next functions simply point to the functions inside of editor.cpp
//this is done because the user is actually writing inside of editor, not mainwindow
//main window just handles the opening and closing of the file. All manipulation is done
//through editor which is the main widget in the UI
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
