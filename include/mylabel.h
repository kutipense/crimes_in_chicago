#ifndef MYLABEL_H
#define MYLABEL_H

#include<QLabel>
#include<QMouseEvent>
#include<decisiontree.h>

/* gui file */

namespace chicago_crime {
    class DecisionTree;
}

class MyLabel : public QLabel
{
    Q_OBJECT

public:
    explicit MyLabel(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *ev);
    chicago_crime::DecisionTree* dt_;
signals:
    void update_panel(const QString&);
public slots:
    void update_loc(bool);
    void update_time(int);
private:
    int x_,y_;
    bool loc_ = 0;
    int time_;

    void update_info();
};


#endif // MYLABEL_H
