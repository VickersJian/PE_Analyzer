#ifndef FILE_LOCATION_CALCULATOR_H
#define FILE_LOCATION_CALCULATOR_H

#include <QDialog>

namespace Ui {
class File_Location_Calculator;
}

class File_Location_Calculator : public QDialog
{
    Q_OBJECT

public:
    explicit File_Location_Calculator(QWidget *parent = nullptr);
    ~File_Location_Calculator();

private slots:

    void change();


    void on_pushButton_clicked();

private:
    Ui::File_Location_Calculator *ui;
};

#endif // FILE_LOCATION_CALCULATOR_H
