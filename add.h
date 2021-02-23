#ifndef ADD_H
#define ADD_H

#include <QWidget>
#include <QSqlQuery>

namespace Ui {
class Add;
}

class Add : public QWidget
{
    Q_OBJECT

public:
    int a;
    explicit Add(QWidget *parent = nullptr);
    ~Add();

    void insert_data(QSqlQuery* sql_query,QString& data); //插入数据

private:
    Ui::Add *ui;
};

#endif // ADD_H
