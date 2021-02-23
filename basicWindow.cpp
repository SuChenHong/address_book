#include "basicWindow.h"
#include "ui_basicWindow.h"
#include "add.h"
#include "logdlg.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QTextCodec>
#include <QMessageBox>
#include <QDialog>
#include <QTimer>
#include <QListWidgetItem>
#include <QListWidget>
#include <QPushButton>
#include <QModelIndex>
#include <QLabel>
#include <typeinfo>
#include <QAction>
#include <QKeyEvent>

basicWindow::basicWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::basicWindow)
{
    extern bool IsTable;
    this->setWindowTitle("主界面");
    ui->setupUi(this);
    //连接数据库
    sql_connect();
    //创建表格 本系统限每次只能创建一个表格 包含所有内容
//    if(IsTable==false)
//        createTable();
    //点击退出
    exit();
    //添加数据
    getAdd();
    //显示数据
    showData();
    //删除数据
    pop();
    //刷新数据
    updateData();
    //帮助文档
    helpExplain();
    //搜索数据
    connect(ui->btn_search,&QPushButton::clicked,[=](){
        QString str;
        str = ui->lineEdit->text();
        search(str);
    });
    //双击查看信息
    DoubleClickToShowInfomation();

}

bool IsTable = false;
bool basicWindow::sql_connect()
{
    //qDebug() << QSqlDatabase::drivers();//打印qt支持的数据库类型
    //setWindowTitle("SQLITE");//设置窗口的标题
    //qDebug() << "open database";
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());//设置显示中文
    QSqlDatabase db;
    //判断是否已经连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
        return true;
    }
    else
    {
        db =  QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("F:\\sqlite\\ttt.db"); //打开或创建数据库
        //db.setDatabaseName("F:\\sqlite\\"+dbname+".db");
        if(!db.open()) //如果数据库打开失败，会弹出一个警告窗口
        {
            QMessageBox::warning(this, "警告", "数据库打开失败");
            return false;
        }
        else
        {
            qDebug() << "open db successfully";
            //QMessageBox::information(this,"结果","数据库打开成功");
            //db.close();
            return true;
        }
    }
}
void basicWindow::exit()
{
    connect(ui->btn_exit,&QPushButton::clicked,[=](){
        this->close();
        logdlg * logWin = new logdlg();
        logWin->show();
        logWin->setAttribute(Qt::WA_DeleteOnClose);
        QSqlDatabase * sql = new QSqlDatabase();
        sql->close();
        if(!sql->open())
        {
            qDebug() << "close db successfully";
        }
        delete sql;
        sql = nullptr;

    });
}
void basicWindow::getAdd()
{
    connect(ui->btn_add,&QPushButton::clicked,[=](){
        Add * newAdd = new Add();
        newAdd->show();
        this->close();
        newAdd->setAttribute(Qt::WA_DeleteOnClose);
    });

}
void basicWindow::pop()
{
    connect(ui->listWidget,&QListWidget::clicked,[=](){
        QListWidgetItem * item = ui->listWidget->currentItem();
        QString * name = new QString(item->text().toUtf8().data());
        qDebug() << *name << name->count();
        int count = name->count();
        if(count!=0)
        {
            count = 0;
            connect(ui->btn_pop,&QPushButton::clicked,[=](){
                QSqlQuery * sql_query = new QSqlQuery();
                QString * delete_sql = new QString();
                *delete_sql = QString("delete from %1 where name = '%2'").arg("person").arg(*name);
                sql_query->prepare(*delete_sql);

                if(!sql_query->exec())
                {
                    qDebug() << sql_query->lastError();
                }
                else
                {
                    qDebug() << "Successfully deleted";
                }
                ui->listWidget->clear();
                showData();

            });
        }
        else
        {
        }
    });

}
void basicWindow::search(QString& str)
{
    QSqlQuery * sql_query = new QSqlQuery();
    ui->listWidget->clear();
    QString *select_sql = new QString();
    *select_sql = QString("select * from %1").arg("person");
    sql_query->prepare(*select_sql);
    QStringList namelist;
    if(!sql_query->exec())
    {
        qDebug() << sql_query->lastError();
    }
    else
    {
        while(sql_query->next())
        {
            QString name = sql_query->value(0).toString();
            namelist.append(name);
        }
    }
    ui->listWidget->addItems(namelist);
    QList<QListWidgetItem *> list;
    list = ui->listWidget->findItems(str,Qt::MatchContains);
    QStringList strList;
    if(!list.isEmpty())
    {

        for(int i=0;i<list.length();i++)
            {
                strList<<list.at(i)->text();
            }
            ui->listWidget->clear();
            for(int i =0;i<strList.length();i++)
            {
                ui->listWidget->addItem(strList.at(i));
            }
    }
    else
    {
        ui->lineEdit->backspace(); //当查询不到时自动清除 挨个清除
        qApp->beep();
    }
    delete sql_query;
    sql_query = nullptr;
    delete select_sql;
    select_sql = nullptr;

}
void basicWindow::showData()
{
    QSqlQuery * sql_query = new QSqlQuery();
    QString *select_sql = new QString();
    *select_sql = QString("select * from %1").arg("person");
    sql_query->prepare(*select_sql);
    QStringList namelist;
    if(!sql_query->exec())
    {
        qDebug() << sql_query->lastError();
    }
    else
    {
        while(sql_query->next())
        {
            QString name = sql_query->value(0).toString();
            namelist.append(name);
        }
    }
    //qDebug() << namelist.count();
    for(int i = 0;i < namelist.count() ;i++)
    {
        ui->listWidget->insertItem(i,namelist[i]);
    }
    delete sql_query;
    sql_query = nullptr;
    delete select_sql;
    select_sql = nullptr;

}
void basicWindow::updateData()
{
    connect(ui->btn_update,&QPushButton::clicked,[=](){
        ui->listWidget->clear();
        showData();
    });
}
void basicWindow::helpExplain()
{
    connect(ui->btn_help,&QPushButton::clicked,[=](){
       QMessageBox::information(this,"帮助","不会有人不会用通讯录吧？");
    });

}
//void basicWindow::createTable()
//{
//    QSqlQuery * sql_query = new QSqlQuery();
//    QString * table1_name = new QString;
//    *table1_name = QString("create table %1(name varchar(10),sex varchar(1),number varchar(12));").arg("person");
//    sql_query->prepare(*table1_name);
//    if(!sql_query->exec())
//    {
//        qDebug() << "Error:Fail to create table" << sql_query->lastError();
//        IsTable = true;
//    }
//    else
//    {
//        qDebug() << "Successfully create table";
//        IsTable = true;
//    }
//    delete table1_name;
//    table1_name = nullptr;
//    delete sql_query;
//    sql_query = nullptr;
//}
extern int first_click;
extern int second_click;
extern int i;
int first_click = -1;
int second_click = -2;
int i = 0;
void basicWindow::DoubleClickToShowInfomation()
{

//    QListWidgetItem * item = new QListWidgetItem();
    connect(ui->listWidget,&QListWidget::clicked,[=](){
        QModelIndex * index = new QModelIndex();
        foreach(*index,ui->listWidget->selectionModel()->selectedIndexes()){
            i++;
            if(i%2==0)
            {
                first_click = index->row();
            }
            else if(i%2!=0)
            {
                second_click = index->row();
            }
            if(first_click == second_click)
            {
                this->hide();
                QDialog * dlg = new QDialog();
                dlg->setWindowTitle("个人信息");
                dlg->resize(400,300);

                QPushButton * btn = new QPushButton(dlg);
                btn->setGeometry(69,229,75,30);
                btn->setText("拨号");
                btn->setAutoDefault(false);

                QPushButton * btn2 = new QPushButton(dlg);
                btn2->setGeometry(169,229,75,30);
                btn2->setText("信息");
                btn2->setAutoDefault(false);

                QPushButton * btn3 = new QPushButton(dlg);
                btn3->setGeometry(269,229,75,30);
                btn3->setText("返回");
                btn3->setAutoDefault(false);

                QLabel * title = new QLabel(dlg);
                QFont * font = new QFont("Adobe Arabic",23, 50);
                title->setText("个人信息");
                title->setGeometry(120,10,121,51);
                title->setFont(*font);

                QLabel * label1 = new QLabel(dlg);
                label1->setText("姓    名:");
                label1->setGeometry(89,69,75,40);

                QLabel * label2 = new QLabel(dlg);
                label2->setText("性    别:");
                label2->setGeometry(89,109,75,40);

                QLabel * label3 = new QLabel(dlg);
                label3->setText("电    话:");
                label3->setGeometry(89,149,75,40);

                QLineEdit * edit_name = new QLineEdit(dlg);
                QLineEdit * edit_sex = new QLineEdit(dlg);
                QLineEdit * edit_num = new QLineEdit(dlg);

                edit_name->setAttribute(Qt::WA_DeleteOnClose);

                edit_name->setText("");
                edit_name->setGeometry(149,79,159,20);

                edit_sex->setText("");
                edit_sex->setGeometry(149,119,159,20);

                edit_num->setText("");
                edit_num->setGeometry(149,159,159,20);
                dlg->show();
                //dlg->exec();
                //dlg->setAttribute(Qt::WA_DeleteOnClose);

                connect(btn,&QPushButton::clicked,[=](){
                    QMessageBox::information(dlg,"信息","该功能暂未实现，后续持续上线！");
                });
                connect(btn2,&QPushButton::clicked,[=](){
                    QMessageBox::information(dlg,"信息","该功能暂未实现，后续持续上线！");
                });
                connect(btn3,&QPushButton::clicked,[=](){
                    this->show();
                    dlg->close();
                });
                QString Name,Sex,Num;
                QString getName = ui->listWidget->item(second_click)->text();
                QString getSex = QString("select sex from person where name = '%1'").arg(getName);
                QString getNum = QString("select number from person where name = '%1'").arg(getName);
                //qDebug() << getSex << getNum ;
                QSqlQuery * sql = new QSqlQuery();
                QSqlQuery * sql1 = new QSqlQuery();
                sql->prepare(getSex);
                if(!sql->exec())
                {
                    qDebug() << sql->lastError();
                }
                else
                {
                    while(sql->next())
                    {
                        Sex = sql->value(0).toString();
                    }
                }
                //qDebug() << Sex;
                sql1->prepare(getNum);
                if(!sql1->exec())
                {
                    qDebug() << sql1->lastError();
                }
                else
                {
                    while(sql1->next())
                    {
                        Num = sql1->value(0).toString();
                    }

                }

                edit_name->setText(getName);
                edit_sex->setText(Sex);
                edit_num->setText(Num);
//                qDebug() << edit_name->text() << edit_num->text() << edit_sex->text();
//                qDebug() << getName << Sex << Num;
                edit_name->displayText();
                edit_sex->displayText();
                edit_num->displayText();
                i = 0;
                first_click = -1;
                second_click = -2;
            }
          }
        delete index;
        index = nullptr;

    });

}

basicWindow::~basicWindow()
{
    delete ui;
}
