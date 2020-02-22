#include "widget.h"
#include"allheader.h"
#include"subordinate_function.h"
#include "ui_widget.h"
#include<iostream>
#include<QFile>
#include<QMessageBox>
#include<QTextStream>
#include<windows.h>
#include<qdebug.h>
#include<QFileDialog>
#include<iterator>
#include<share.h>
#include<details.h>
#include<directories.h>
#include<sections.h>
#include<file_location_calculator.h>


extern char charfilename[FILE_LOCATION_LENTH];
extern bool file_open_condition;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->filelocation->setPlaceholderText("Please enter the location of the file");
    //按下回车发送信号给按钮“打开”
    connect(ui->filelocation,SIGNAL(returnPressed()),this,SLOT(on_openfile_clicked()));


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_openfile_clicked()
{
    bool detect=false;
    int c=0;
    QString file_name=ui->filelocation->text();
    if(file_name==""){
        file_name=QFileDialog::getOpenFileName(nullptr,"打开",".","*.exe *.dll");
        ui->filelocation->setText(file_name);
    }else if(file_name==charfilename){
        file_name=QFileDialog::getOpenFileName(nullptr,"打开",".","*.exe *.dll");
        ui->filelocation->setText(file_name);
    }
    QFile file(file_name);
    if(file.open(QIODevice::ReadOnly)){
        file_open_condition=true;
        char* b=charfilename;
        QStringToCharArray(file_name,b);

        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        CalculateOffset(c);
        Calculate_Import_Offset(c);
        dos_header DOSH(c);
        file_header FILEH(c);
        opt_header OPTH(c);
        if(DOSH.ptr_DosHead_ReadOnly->e_magic!=0x5a4d)goto exit;
        _lseek(c,DOSH.ptr_DosHead_ReadOnly->e_lfanew,SEEK_SET);
        DWORD PESign;
        _read(c,&PESign,sizeof(DWORD));
        if(PESign!=0x4550)goto exit;
        if(OPTH.ptr_OptinonHead_ReadOnly->Magic!=0x10b){QMessageBox::information(this, tr(""), tr("暂时仅支持32位的可执行文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);detect=true;goto exit;
}
        ui->EntryPoint->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->AddressOfEntryPoint));
        ui->ImageBase->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->ImageBase));
        ui->SizeOfImage->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->SizeOfImage));
        ui->BaseOfCode->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->BaseOfCode));
        ui->BaseOfData->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->BaseOfData));
        ui->SectionAlignment->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->SectionAlignment));
        ui->FileAlignment->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->FileAlignment));
        ui->Magic->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->Magic));
        ui->Subsystem->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->Subsystem));
        ui->NumberOfsections->setText(Number_x16_ToQstring(FILEH.ptr_FileHead_ReadOnly->NumberOfSections));
        ui->TimeDateStamp->setText(Number_x16_ToQstring(FILEH.ptr_FileHead_ReadOnly->TimeDateStamp));
        ui->SizeOfHeaders->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->SizeOfHeaders));
        ui->Characteristics->setText(Number_x16_ToQstring(FILEH.ptr_FileHead_ReadOnly->Characteristics));
        ui->Checksum->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->CheckSum));
        ui->SizeOfOptionlHeader->setText(Number_x16_ToQstring(FILEH.ptr_FileHead_ReadOnly->SizeOfOptionalHeader));
        ui->NumOfRvaAndSizes->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->NumberOfRvaAndSizes));
        _close( c );
        file.close();
    }
    else if(file_name!=""){
        exit:
        file_open_condition=false;
        if(detect==false){QMessageBox::warning(this, tr("Error"), tr("Open File failed:%1").arg(file.errorString()), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);}
        if(c!=0)_close(c);
    }
}






void Widget::on_Details_clicked(){
    if(file_open_condition){
        int c=0;
        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        dos_header DOSH(c);
        file_header FILEH(c);
        opt_header OPTH(c);
        sec_header SECH(c);
        Details details;
        details.show();
        QString a;
        a+=DOSH.show_header_in_window();
        a+=FILEH.show_header_in_window();
        a+=OPTH.show_header_in_window();
        a+=SECH.show_header_in_window();
        _close( c );
        details.showDetails->setText(a);
        details.exec();
    }else QMessageBox::warning(this, tr("错误"), tr("请先打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}



void Widget::on_directories_clicked()
{
    int c=0;
    if(file_open_condition){

            _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
            Directories directories;
            opt_header OPTH(c);
            directories.ExportTable->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress));
            directories.ExportTableSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size));
            directories.ImportTable->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
            directories.ImportTableSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size));
            directories.Resource->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress));
            directories.ResourceSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size));
            directories.Exception->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress));
            directories.ExceptionSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size));
            directories.Security->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress));
            directories.SecuritySize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size));
            directories.Relocation->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress));
            directories.RelocationSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size));
            directories.Debug->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress));
            directories.DebugSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size));
            directories.Copyright->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress));
            directories.CopyrightSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size));
            directories.Globalptr->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress));
            directories.GlobalptrSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size));
            directories.TLSTabal->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress));
            directories.TLSTabalSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size));
            directories.LoadConfig->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress));
            directories.LoadConfigSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size));
            directories.BoundImport->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress));
            directories.BoundImportSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size));
            directories.IAT->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress));
            directories.IATSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size));
            directories.DelayImport->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress));
            directories.DelayImportSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size));
            directories.COM->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress));
            directories.COMSize->setText(Number_x16_ToQstring(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size));
            directories.Reserved->setText("0x00000000");
            directories.ReservedSize->setText("0x00000000");
            if(c!=0){_close(c);c=0;}
            directories.show();

            directories.exec();
            int x=_sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
            if(x!=0){QMessageBox::warning(this, tr("错误"), tr("刷新失败，请重新打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);goto exit;}
            opt_header OPTH2(c);
            file_header FILEH2(c);
            ui->SizeOfImage->setText(Number_x16_ToQstring(OPTH2.ptr_OptinonHead_ReadOnly->SizeOfImage));
            ui->NumberOfsections->setText(Number_x16_ToQstring(FILEH2.ptr_FileHead_ReadOnly->NumberOfSections));
            if(c!=0){_close(c);c=0;}
        }else QMessageBox::warning(this, tr("错误"), tr("请先打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    exit:if(c!=0){_close(c);}
}


void Widget::on_Import_clicked()
{
    if(file_open_condition){
        int c=0;
        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        sec_header SECH(c);
        Sections sections;
        QStandardItemModel* model = new QStandardItemModel(&sections);
        model->setColumnCount(6);
        model->setHeaderData(0,Qt::Horizontal, "Name");
        model->setHeaderData(1,Qt::Horizontal,"VOffset");
        model->setHeaderData(2,Qt::Horizontal,"VSize");
        model->setHeaderData(3,Qt::Horizontal,"ROffset");
        model->setHeaderData(4,Qt::Horizontal,"RSize");
        model->setHeaderData(5,Qt::Horizontal,"Flags");
        SECH.Show_Sec_in_ViewTable(model);
        sections.infor->setModel(model);
         _close( c );
        sections.show();

        sections.exec();

    }else QMessageBox::warning(this, tr("错误"), tr("请先打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}

void Widget::on_File_Location_Calculator_clicked()
{
    File_Location_Calculator calculator;
    calculator.show();
    calculator.exec();
}
