#include "mywindow.h"
#include "ui_mywindow.h"
#include <decisiontree.h>

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);
}

MyWindow::~MyWindow()
{
    delete ui;
}

void MyWindow::set_dt(chicago_crime::DecisionTree* dt){
    ui->map->dt_ = dt;
}
