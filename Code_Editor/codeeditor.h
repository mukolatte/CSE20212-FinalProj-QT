#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QObject>

namespace Ui {
class codeeditor;
}

class codeeditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit codeeditor(QWidget *parent = 0);
    ~codeeditor();

private:
    Ui::codeeditor *ui;
};

#endif // CODEEDITOR_H
