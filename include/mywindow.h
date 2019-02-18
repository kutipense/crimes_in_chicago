#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>

namespace chicago_crime{
    class DecisionTree;
}

/* gui file */

namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = nullptr);
    ~MyWindow();
    void set_dt(chicago_crime::DecisionTree*);
private:
    Ui::MyWindow *ui;
};


#endif // MYWINDOW_H
