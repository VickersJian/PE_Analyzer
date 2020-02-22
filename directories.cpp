#include "directories.h"
#include "ui_directories.h"
#include"importtable.h"
#include<QStringListModel>
#include<QStandardItemModel>
#include"subordinate_function.h"
#include"allheader.h"
#include"import_descriptor.h"
#include<share.h>
#include<QMessageBox>
Directories::Directories(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Directories)
{
    ui->setupUi(this);
    setWindowTitle("Directory Table");
    ExportTable=ui->ExportTable;
    ExportTableSize=ui->ExportTable_2;
    ImportTable=ui->ImportTable;
    ImportTableSize=ui->ImportTable_2;
    Resource=ui->Resource;
    ResourceSize=ui->Resource_2;
    Exception=ui->Exception;
    ExceptionSize=ui->Exception_2;
    Security=ui->Security;
    SecuritySize=ui->Security_2;
    Relocation=ui->Relocation;
    RelocationSize=ui->Relocation_2;
    Debug=ui->Debug;
    DebugSize=ui->Debug_2;
    Copyright=ui->Copyright;
    CopyrightSize=ui->Copyright_2;
    Globalptr=ui->Globalptr;
    GlobalptrSize=ui->Globalptr_2;
    TLSTabal=ui->TLSTabal;
    TLSTabalSize=ui->TLSTabal_2;
    LoadConfig=ui->LoadConfig;
    LoadConfigSize=ui->LoadConfig_2;
    BoundImport=ui->BoundImport;
    BoundImportSize=ui->BoundImport_2;
    IAT=ui->IAT;
    IATSize=ui->IAT_2;
    DelayImport=ui->DelayImport;
    DelayImportSize=ui->DelayImport_2;
    COM=ui->COM;
    COMSize=ui->COM_2;
    Reserved=ui->Reserved;
    ReservedSize=ui->Reserved_2;
}

Directories::~Directories()
{
    delete ui;
}

void Directories::on_OK_clicked()
{
    close();
}

void Directories::on_more_clicked()
{
    int c=0;
    if(file_open_condition){

        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        opt_header OPTH(c);
       if(c!=0){_close(c);c=0;}
        if(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size){
        class ImportTable importtable;

        int c=0;
        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);

        Import_Descriptor ImportDES(c);

        QStandardItemModel* model = new QStandardItemModel(&importtable);
        model->setColumnCount(6);
        model->setHeaderData(0,Qt::Horizontal, "DllName");
        model->setHeaderData(1,Qt::Horizontal,"OriginalFirstThunk");
        model->setHeaderData(2,Qt::Horizontal,"TimeDateStamp");
        model->setHeaderData(3,Qt::Horizontal,"ForwarderChain");
        model->setHeaderData(4,Qt::Horizontal,"Name");
        model->setHeaderData(5,Qt::Horizontal,"FirstThunk");
        ImportDES.Show_Import_Descriptor_in_ViewTable(model);

        importtable.importlist->setModel(model);

        importtable.importlist->setColumnWidth(0, 250);
        importtable.importlist->setColumnWidth(1, 130);
        importtable.importlist->setColumnWidth(2, 130);
        importtable.importlist->setColumnWidth(3, 130);
        importtable.importlist->setColumnWidth(4, 100);
        importtable.importlist->setColumnWidth(5, 100);

        QStandardItemModel* model_ = new QStandardItemModel(this);
        model_->setColumnCount(5);
        model_->setHeaderData(0,Qt::Horizontal, "ThunkRVA");
        model_->setHeaderData(1,Qt::Horizontal,"ThunkOffset");
        model_->setHeaderData(2,Qt::Horizontal,"ThunkValue");
        model_->setHeaderData(3,Qt::Horizontal,"Hint");
        model_->setHeaderData(4,Qt::Horizontal,"ApiName");

        importtable.details->setModel(model_);

        importtable.details->setColumnWidth(0, 100);
        importtable.details->setColumnWidth(1, 130);
        importtable.details->setColumnWidth(2, 130);
        importtable.details->setColumnWidth(3, 130);
        importtable.details->setColumnWidth(4, 350);

       if(c!=0){_close(c);c=0;}
        importtable.show();

        importtable.exec();
        int x=_sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        if(x!=0){QMessageBox::warning(this, tr("错误"), tr("数据更新失败，请重新打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);goto exit;}
        opt_header OPTH(c);
        ui->ImportTable->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
        ui->ImportTable_2->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size));
        if(c!=0){_close(c);c=0;}
        }else QMessageBox::warning(this, tr("提示"), tr("该文件无导入表"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    }else QMessageBox::warning(this, tr("错误"), tr("请先打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    exit:if(c!=0){_close(c);}
}

void Directories::on_Resource_more_clicked()
{

}
