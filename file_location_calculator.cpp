#include "file_location_calculator.h"
#include "ui_file_location_calculator.h"
#include<QMessageBox>
#include"allheader.h"
#include<share.h>
#include"subordinate_function.h"
File_Location_Calculator::File_Location_Calculator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::File_Location_Calculator)
{
    ui->setupUi(this);
    setWindowTitle("File Location Calculator");
    ui->BelongSection->setFocusPolicy(Qt::NoFocus);
    connect(ui->VA,SIGNAL(textEdited(const QString&)),this,SLOT(change()));
    connect(ui->RVA,SIGNAL(textEdited(const QString&)),this,SLOT(change()));
    connect(ui->File_Offset,SIGNAL(textEdited(const QString&)),this,SLOT(change()));
}

File_Location_Calculator::~File_Location_Calculator()
{
    delete ui;
}

void File_Location_Calculator::change(){
    if(sender()==ui->VA){
        ui->RVA->clear();
        ui->File_Offset->clear();
        ui->BelongSection->clear();
    }else if (sender()==ui->RVA) {
        ui->VA->clear();
        ui->File_Offset->clear();
        ui->BelongSection->clear();
    }
    else if (sender()==ui->File_Offset) {
        ui->VA->clear();
        ui->RVA->clear();
        ui->BelongSection->clear();
    }
}

void File_Location_Calculator::on_pushButton_clicked()
{
    if(file_open_condition){

        int c=0;
        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        dos_header DOSH(c);
        sec_header SECH(c);
        opt_header OPTH(c);
        //来源RVA
        if(ui->RVA->text()!=""){
            DWORD RVA=QString_To_Number_x16(ui->RVA->text());

            SECH.RVA_Get_Need_Section(RVA);


            SECH.RVA_Get_Need_Section(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
            DWORD File_Offset=OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress-SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->PointerToRawData;



            ui->VA->setText(Number_x16_ToQstring(RVA+OPTH.ptr_OptinonHead_ReadOnly->ImageBase));
            ui->File_Offset->setText(Number_x16_ToQstring(File_Offset));

            if(RVA!=static_cast<DWORD>(File_Offset))
            ui->BelongSection->setText(Unsigned_Char_To_QString(SECH.ptr_SectionHead_ReadOnly->Name));
            else{
                QString a="Out Of Section";
                ui->BelongSection->setText(a);
            }
            //来源VA
        }else if(ui->VA->text()!=""){
            DWORD VA=QString_To_Number_x16(ui->VA->text());
            if(VA>=(OPTH.ptr_OptinonHead_ReadOnly->SizeOfHeaders+OPTH.ptr_OptinonHead_ReadOnly->ImageBase)){
            SECH.RVA_Get_Need_Section(VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase);

            DWORD result=VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase-SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->PointerToRawData;

            ui->RVA->setText(Number_x16_ToQstring(VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase));
            ui->File_Offset->setText(Number_x16_ToQstring(result));
            ui->BelongSection->setText(Unsigned_Char_To_QString(SECH.ptr_SectionHead_ReadOnly->Name));
            }else if(VA>=OPTH.ptr_OptinonHead_ReadOnly->ImageBase&&VA<(OPTH.ptr_OptinonHead_ReadOnly->SizeOfHeaders+OPTH.ptr_OptinonHead_ReadOnly->ImageBase)){
                DWORD result=VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase;
                ui->RVA->setText(Number_x16_ToQstring(VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase));
                ui->File_Offset->setText(Number_x16_ToQstring(result));
                ui->BelongSection->setText(Unsigned_Char_To_QString(SECH.ptr_SectionHead_ReadOnly->Name));
            }else if(VA<OPTH.ptr_OptinonHead_ReadOnly->ImageBase){
                QString a="Erro VA";
                ui->RVA->setText(a);
                ui->File_Offset->setText(a);
            }
            //来源File_Offset
        }else if(ui->File_Offset->text()!=""){
            DWORD File_Offset=QString_To_Number_x16(ui->File_Offset->text());
            int Int_File_Offset=ui->File_Offset->text().toInt(nullptr,16);
            if(File_Offset>=(static_cast<unsigned long>(DOSH.ptr_DosHead_ReadOnly->e_lfanew)+sizeof(IMAGE_FILE_HEADER)+sizeof(IMAGE_OPTIONAL_HEADER))){
            SECH.File_Offset_Get_Need_Section(Int_File_Offset);
            DWORD VA=File_Offset+OPTH.ptr_OptinonHead_ReadOnly->ImageBase+SECH.ptr_SectionHead_ReadOnly->VirtualAddress-SECH.ptr_SectionHead_ReadOnly->PointerToRawData;

            ui->VA->setText(Number_x16_ToQstring(VA));
            ui->RVA->setText(Number_x16_ToQstring(VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase));
            ui->BelongSection->setText(Unsigned_Char_To_QString(SECH.ptr_SectionHead_ReadOnly->Name));
            }else {
                DWORD VA=File_Offset+OPTH.ptr_OptinonHead_ReadOnly->ImageBase;
                QString a="Out Of Section";
                ui->VA->setText(Number_x16_ToQstring(VA));
                ui->RVA->setText(Number_x16_ToQstring(VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase));
                ui->BelongSection->setText(a);
            }
        }
        _close(c);
    }else QMessageBox::warning(this, tr("错误"), tr("请先打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}
