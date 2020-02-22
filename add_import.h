#ifndef ADD_IMPORT_H
#define ADD_IMPORT_H

#include <QDialog>

namespace Ui {
class Add_Import;
}

class Add_Import : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Import(QWidget *parent = nullptr);
    ~Add_Import();

private slots:
    void on_pushButton_clicked();

    void on_Add_clicked();

private:
    Ui::Add_Import *ui;
};

#endif // ADD_IMPORT_H
