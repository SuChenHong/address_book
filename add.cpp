#include "add.h"
#include "ui_add.h"
#include <QDebug>
#include <QSqlError>
#include "basicWindow.h"
#include <QMessageBox>
#include <typeinfo>

Add::Add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Add)
{

    //extern bool global_signals; //用于判断是否插入数据从而对数据库进行更新
    ui->setupUi(this);
    this->setWindowTitle("添加用户");
    QSqlQuery * sql_query = new QSqlQuery;
    basicWindow * openWindow = new basicWindow();
    //添加数据
    connect(ui->btn_save,&QPushButton::clicked,[=](){
       const int min = 8;
       const int max = 11;
       QString name = ui->lineEdit_name->text();
       QString sex = ui->lineEdit_sex->text();
       QString number = ui->lineEdit_number->text().toUtf8().data();
       int tmp = number.count();
       if(name == "" || sex == "" || number == "")
       {
           QMessageBox::critical(this,"错误提示","内容均不能为空");
           ui->lineEdit_sex->clear();
           ui->lineEdit_name->clear();
           ui->lineEdit_number->clear();
       }
       else if(tmp < min || tmp > max)
       {
           QMessageBox::critical(this,"错误提示","电话号码只能为8位或11位");
           ui->lineEdit_number->clear();
       }
       else
       {
           QString *insert_sql = new QString();
           *insert_sql = QString("insert into %1 values('%2','%3',%4)").arg("person").arg(name).arg(sex).arg(number);
           qDebug() << *insert_sql;
           insert_data(sql_query,*insert_sql);
           this->close();
           openWindow->setWindowTitle("主界面");
           openWindow->show();
           openWindow->setAttribute(Qt::WA_DeleteOnClose);


       }

    });
    connect(ui->btn_cancel,&QPushButton::clicked,[=](){
        this->close();
        openWindow->setWindowTitle("主界面");
        openWindow->show();
        openWindow->setAttribute(Qt::WA_DeleteOnClose);
    });

}

//bool global_signals = 0;
void Add::insert_data(QSqlQuery *sql_query, QString &data)
{
    sql_query->prepare(data);
    if(!sql_query->exec())
    {
        qDebug() << sql_query->lastError();
    }
    else
    {
        qDebug() << "Successfully inserted";
    }
    ui->lineEdit_name->clear();
    ui->lineEdit_sex->clear();
    ui->lineEdit_number->clear();

}



Add::~Add()
{
    delete ui;
}
