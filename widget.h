#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMessageBox>
#include"allheader.h"
#include<QFileDialog>
#include<details.h>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_openfile_clicked();


    void on_Details_clicked();




    void on_directories_clicked();

    void on_Import_clicked();

    void on_File_Location_Calculator_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
