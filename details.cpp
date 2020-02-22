#include "details.h"
#include "ui_details.h"

Details::Details(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Details)
{
    ui->setupUi(this);
    setWindowTitle("PE Details");
    showDetails=ui->showDetails;
}

Details::~Details()
{
    delete ui;
}

