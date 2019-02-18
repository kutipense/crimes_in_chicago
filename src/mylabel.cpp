#include <mylabel.h>
#include <iostream>
#include <crime_data.h>

using chicago_crime::CrimeDataPack;
using chicago_crime::CrimeData;
using chicago_crime::EXTERIOR;
using chicago_crime::INTERIOR;

/* gui file */

MyLabel::MyLabel(QWidget *parent) : QLabel (parent){}

void MyLabel::mousePressEvent(QMouseEvent *ev){
    x_ = ev->x()/20;
    y_ = ev->y()/20;
    update_info();
}

void MyLabel::update_loc(bool checked){
    loc_ = checked;
    update_info();
}

void MyLabel::update_time(int time){
    time_ = time;
    update_info();
}

void MyLabel::update_info(){
    CrimeData cd("None", loc_ == true ? INTERIOR : EXTERIOR, time_, x_*ROWS+y_);
    CrimeDataPack out = dt_->classify(cd);
    std::string str = "<font size=4>" + out.results() + "</font>";
    QString qs = QString::fromStdString(str);
    emit update_panel(qs);
}
