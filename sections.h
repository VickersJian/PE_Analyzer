#pragma once
#ifndef SECTIONS_H
#define SECTIONS_H

#include <QDialog>
#include<QTableView>
#include<QStandardItemModel>
#include<QTableWidget>

namespace Ui {
class Sections;
}

class Sections : public QDialog
{
    Q_OBJECT

public:
    explicit Sections(QWidget *parent = nullptr);
    QTableView* infor;
    QTableWidget* ptr;
    ~Sections();

private:
    Ui::Sections *ui;
};

#endif // SECTIONS_H
