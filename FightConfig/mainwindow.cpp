#include <iostream>
#include <fstream>
#include <qfiledialog.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->publishBtns, SIGNAL(clicked()), this, SLOT(onPublish()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPublish()
{
    QWidget *mygroup = ui->myGroup;
    QWidget* enemyGroup1 = ui->enemyGroup1;
    QWidget* enemyGroup2 = ui->enemyGroup2;
    QWidget* enemyGroup3 = ui->enemyGroup3;

    cout<<"publish"<<endl;
    std::string fightLevelStr = ui->fightLevel->currentText().toStdString();

    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.String("fightLevel");
    writer.Int(std::atoi(fightLevelStr.c_str()));

    writer.String("my");
    my2json(writer, mygroup);

    writer.String("enemy1");
    pve2json(writer, enemyGroup1);

    writer.String("enemy2");
    pve2json(writer, enemyGroup2);

    writer.String("enemy3");
    pve2json(writer, enemyGroup3);

    writer.EndObject();
    cout << s.GetString() << endl;

    QFileDialog *fileDialog = new QFileDialog(this,tr("选择投影数据文件目录"), "../../ProjectData/","" );
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNamesList;
    if(fileDialog->exec()) // ok
    {
        fileNamesList=fileDialog->selectedFiles();
        QString fileName = fileNamesList.at(0).toLocal8Bit().constData();
        QDir dir = fileDialog->directory();
        QString filePath = dir.filePath(fileName);
        cout<<filePath.toStdString()<<endl;

        ofstream outputFile;

        std::string saveFileName = filePath.toStdString()+std::string("/fight_")+fightLevelStr+std::string(".json");
        outputFile.open(saveFileName.c_str());
        outputFile<<s.GetString()<<endl;
        outputFile.close();
    }
}

void MainWindow::my2json(Writer<StringBuffer> &writer, QWidget*group)
{
    writer.StartArray();
    foreach (auto obj, group->children()) {
        QWidget* child = qobject_cast<QWidget*>(obj);
        QCheckBox* ok = qobject_cast<QCheckBox*>(getChildByName(child, QString("ok")));

        QComboBox* hero = qobject_cast<QComboBox*>(getChildByName(child, QString("hero")));
        QSpinBox* hero_type = qobject_cast<QSpinBox*>(getChildByName(child, QString("hero_type")));
        QSpinBox* hero_level = qobject_cast<QSpinBox*>(getChildByName(child, QString("hero_level")));

        QComboBox* tou = qobject_cast<QComboBox*>(getChildByName(child, QString("tou")));
        QSpinBox* tou_type = qobject_cast<QSpinBox*>(getChildByName(child, QString("tou_type")));
        QSpinBox* tou_level = qobject_cast<QSpinBox*>(getChildByName(child, QString("tou_level")));

        QComboBox* shi = qobject_cast<QComboBox*>(getChildByName(child, QString("shi")));
        QSpinBox* shi_type = qobject_cast<QSpinBox*>(getChildByName(child, QString("shi_type")));
        QSpinBox* shi_level = qobject_cast<QSpinBox*>(getChildByName(child, QString("shi_level")));

        QComboBox* wu = qobject_cast<QComboBox*>(getChildByName(child, QString("wu")));
        QSpinBox* wu_type = qobject_cast<QSpinBox*>(getChildByName(child, QString("wu_type")));
        QSpinBox* wu_level = qobject_cast<QSpinBox*>(getChildByName(child, QString("wu_level")));

        QComboBox* yi = qobject_cast<QComboBox*>(getChildByName(child, QString("yi")));
        QSpinBox* yi_type = qobject_cast<QSpinBox*>(getChildByName(child, QString("yi_type")));
        QSpinBox* yi_level = qobject_cast<QSpinBox*>(getChildByName(child, QString("yi_level")));

        if (ok->checkState() == 0) continue;

        writer.StartObject();
        if (hero) {
            writer.String("hero_id");
            writer.Int(hero->currentIndex()+1);
            writer.String("hero_name");
            writer.String(hero->currentText().toStdString().c_str());
        }
        if (hero_type) {
            writer.String("hero_type");
            writer.Int(hero_type->value());
        }
        if (hero_level) {
            writer.String("hero_level");
            writer.Int(hero_level->value());
        }

        if (tou) {
            writer.String("tou_id");
            writer.Int(tou->currentIndex()+1);
            writer.String("tou_name");
            writer.String(tou->currentText().toStdString().c_str());
        }
        if (tou_type) {
            writer.String("tou_type");
            writer.Int(tou_type->value());
        }
        if (tou_level) {
            writer.String("tou_level");
            writer.Int(tou_level->value());
        }

        if (shi) {
            writer.String("shi_id");
            writer.Int(shi->currentIndex()+1);
            writer.String("shi_name");
            writer.String(shi->currentText().toStdString().c_str());
        }
        if (shi_type) {
            writer.String("shi_type");
            writer.Int(shi_type->value());
        }
        if (shi_level) {
            writer.String("shi_level");
            writer.Int(shi_level->value());
        }

        if (wu) {
            writer.String("wu_id");
            writer.Int(wu->currentIndex()+1);
            writer.String("wu_name");
            writer.String(wu->currentText().toStdString().c_str());
        }
        if (wu_type) {
            writer.String("wu_type");
            writer.Int(wu_type->value());
        }
        if (wu_level) {
            writer.String("wu_level");
            writer.Int(wu_level->value());
        }

        if (yi) {
            writer.String("yi_id");
            writer.Int(yi->currentIndex()+1);
            writer.String("yi_name");
            writer.String(yi->currentText().toStdString().c_str());
        }
        if (yi_type) {
            writer.String("yi_type");
            writer.Int(yi_type->value());
        }
        if (yi_level) {
            writer.String("yi_level");
            writer.Int(yi_level->value());
        }

        writer.EndObject();
    }
    writer.EndArray();
}

void MainWindow::pve2json(Writer<StringBuffer> &writer, QWidget*group)
{
    writer.StartArray();
    foreach (auto obj, group->children()) {
        QWidget* child = qobject_cast<QWidget*>(obj);
        QCheckBox* ok = qobject_cast<QCheckBox*>(getChildByName(child, QString("ok")));
        QComboBox* hero = qobject_cast<QComboBox*>(getChildByName(child, QString("hero")));
        QSpinBox* level = qobject_cast<QSpinBox*>(getChildByName(child, QString("level")));
        QCheckBox* boss = qobject_cast<QCheckBox*>(getChildByName(child, QString("boss")));

        if (ok->checkState() == 0) continue;

        writer.StartObject();
        if (hero) {
            writer.String("hero_id");
            writer.Int(hero->currentIndex()+1);
            writer.String("hero");
            writer.String(hero->currentText().toStdString().c_str());
        }

        if (level) {
            writer.String("hero_level");
            writer.Int(level->value());
        }

        if (boss) {
            writer.String("boss");
            writer.Int(boss->checkState());
        } else {
            writer.String("boss");
            writer.Int(0);
        }
        writer.EndObject();
    }
    writer.EndArray();
}

QWidget* MainWindow::getChildByName(QWidget *parent, QString name)
{
    foreach (auto obj, parent->children()) {
        QWidget *child = qobject_cast<QWidget*>(obj);
        if (child->accessibleName() == name)
        {
            return child;
        }
    }
    return NULL;
}


