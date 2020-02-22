#pragma once
#ifndef DETAILS_H
#define DETAILS_H

#include <QDialog>
#include<QTextEdit>
namespace Ui {
class Details;
}

class Details : public QDialog
{
    Q_OBJECT

public:
    explicit Details(QWidget *parent = nullptr);
    Details(QWidget* s,int);
    QTextEdit *showDetails=nullptr;
    ~Details();

private:
    Ui::Details *ui;
};

#endif // DETAILS_H
