#ifndef LOGDLG_H
#define LOGDLG_H

#include <QWidget>
#include <QSqlQuery>


QT_BEGIN_NAMESPACE
namespace Ui { class logdlg; }
QT_END_NAMESPACE

class logdlg : public QWidget
{
    Q_OBJECT

public:
    logdlg(QWidget *parent = nullptr);
    ~logdlg();
    int login_validate(); //登陆验证
    void logdlg_exit(); //登陆界面退出
//    void create_table(QSqlQuery* sql_query,QString& data); //创建表格
//    void insert_data(QSqlQuery* sql_query,QString& data); //插入数据
//    void sort_data(QSqlQuery* sql_query,QString& data); //查询数据

private:
    Ui::logdlg *ui;
};
#endif // LOGDLG_H
