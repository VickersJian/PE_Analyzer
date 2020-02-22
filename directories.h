#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <QDialog>
#include<QLineEdit>

namespace Ui {
class Directories;
}

class Directories : public QDialog
{
    Q_OBJECT

public:
    explicit Directories(QWidget *parent = nullptr);
    QLineEdit* ExportTable;
    QLineEdit* ExportTableSize;
    QLineEdit* ImportTable;
    QLineEdit* ImportTableSize;
    QLineEdit* Resource;
    QLineEdit* ResourceSize;
    QLineEdit* Exception;
    QLineEdit* ExceptionSize;
    QLineEdit* Security;
    QLineEdit* SecuritySize;
    QLineEdit* Relocation;
    QLineEdit* RelocationSize;
    QLineEdit* Debug;
    QLineEdit* DebugSize;
    QLineEdit* Copyright;
    QLineEdit* CopyrightSize;
    QLineEdit* Globalptr;
    QLineEdit* GlobalptrSize;
    QLineEdit* TLSTabal;
    QLineEdit* TLSTabalSize;
    QLineEdit* LoadConfig;
    QLineEdit* LoadConfigSize;
    QLineEdit* BoundImport;
    QLineEdit* BoundImportSize;
    QLineEdit* IAT;
    QLineEdit* IATSize;
    QLineEdit* DelayImport;
    QLineEdit* DelayImportSize;
    QLineEdit* COM;
    QLineEdit* COMSize;
    QLineEdit* Reserved;
    QLineEdit* ReservedSize;
    ~Directories();

private slots:
    void on_OK_clicked();

    void on_more_clicked();

    void on_Resource_more_clicked();

private:
    Ui::Directories *ui;
};

#endif // DIRECTORIES_H
