#include "logdlg.h"
#include "ui_logdlg.h"
#include "basicWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QAction>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QTextCodec>


logdlg::logdlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::logdlg)
{
    ui->setupUi(this);
    setWindowTitle("登陆界面");
    //验证通讯录账号和密码
    int(logdlg::*validate)(void) = &logdlg::login_validate;
    connect(ui->btn_login,&QPushButton::clicked,this,validate);

    //退出通讯录的登陆界面
    logdlg_exit();

}
int logdlg::login_validate()
{
    QString login_admin = ui->lineEdit_admin->text().toUtf8().data(); //获取输入的账户名
    QString login_pwd = ui->lineEdit_pwd->text().toUtf8().data(); //获取输入的密码
//    MainWindow *new_win = new MainWindow();
//    new_win->setWindowTitle("主界面");
//    void(MainWindow::*display)(void) = &MainWindow::show;
    QMessageBox * info = new QMessageBox(this);
    if(login_admin == "admin" && login_pwd == "sch201004")
    {
        basicWindow *new_win = new basicWindow();
        new_win->setWindowTitle("主界面");
        //void(MainWindow::*display)(void) = &MainWindow::show;
        qDebug() << "login successfully" ;
        if(QMessageBox::Yes == info->question(this,"登陆提示","登陆成功！",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        {
            this->hide();
            new_win->show();
        }
        else
        {
        }
        //connect(ui->btn_login,&QPushButton::clicked,new_win,display);
        new_win->setAttribute(Qt::WA_DeleteOnClose);
        info->setAttribute(Qt::WA_DeleteOnClose);
        return true;
    }
    else
    {
        info->critical(this,"错误提示","账户或密码错误！");
        ui->lineEdit_admin->clear();
        ui->lineEdit_pwd->clear();
        info->setAttribute(Qt::WA_DeleteOnClose);
        return false;
    }

}
void logdlg::logdlg_exit()
{
    connect(ui->btn_esc,&QPushButton::clicked,this,&QWidget::close);
}
logdlg::~logdlg()
{
    delete ui;
}

