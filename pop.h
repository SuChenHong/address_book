#ifndef POP_H
#define POP_H

#include <QWidget>

namespace Ui {
class Pop;
}

class Pop : public QWidget
{
    Q_OBJECT

public:
    explicit Pop(QWidget *parent = nullptr);
    ~Pop();

private:
    Ui::Pop *ui;
};

#endif // POP_H
