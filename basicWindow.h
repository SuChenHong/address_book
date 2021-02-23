#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include <QWidget>


namespace Ui {
class basicWindow;
}

class basicWindow : public QWidget
{
    Q_OBJECT

public:
    explicit basicWindow(QWidget *parent = nullptr);
    ~basicWindow();
    //void keyPressEvent(QKeyEvent *ev); //捕捉键盘
    bool sql_connect();//连接数据库
    void exit(); //主界面的退出
    void getAdd(); //主界面数据的添加
    void pop(); //主界面数据的删除
    void search(QString& str); //主界面数据的搜索
    void showData(); //显示数据
    void updateData(); //刷新数据 添加完数据后需要刷新
    void helpExplain(); //系统使用说明
    //void createTable(); //创建表格
    void DoubleClickToShowInfomation(); //双击查看信息
    void newDialog();
    //void mousePressEvent(QMouseEvent *event);

private:
    Ui::basicWindow *ui;
};

#endif // BASICWINDOW_H
