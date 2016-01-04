#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "json/rapidjson.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace rapidjson;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onPublish();

private:
    Ui::MainWindow *ui;
    QWidget* getChildByName(QWidget *parent, QString name);
    void pve2json(Writer<StringBuffer> &writer, QWidget*group);
    void my2json(Writer<StringBuffer> &writer, QWidget*group);
};

#endif // MAINWINDOW_H
