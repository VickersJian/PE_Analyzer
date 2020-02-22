#include "sections.h"
#include "ui_sections.h"

Sections::Sections(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sections)
{
    ui->setupUi(this);
    setWindowTitle("Sections Table");
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    infor=ui->tableView;
}

Sections::~Sections()
{
    delete ui;
}
