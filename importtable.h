#pragma once
#ifndef IMPORTTABLE_H
#define IMPORTTABLE_H

#include <QDialog>
#include<QTableView>
#include<allheader.h>
namespace Ui {
class ImportTable;
}

class ImportTable : public QDialog
{
    Q_OBJECT

public:
    explicit ImportTable(QWidget *parent = nullptr);
    QTableView *importlist;
    QTableView *details;
    ~ImportTable();
private slots:
    void Show_Details();
    //右键菜单
    void slotContextMenu(QPoint pos);
    void Add_Import();
private:
    Ui::ImportTable *ui;


};

#endif // IMPORTTABLE_H
