#include"subordinate_function.h"
#include<QDebug>
#include<QLineEdit>
#include<QStandardItemModel>
#include<share.h>

long int OffDosHeader=0;
long int OffFileHeader=0;
long int OffOptHeader=0;
long int OffSecHeader=0;
long int OffImportDescriptor=0;
char charfilename[FILE_LOCATION_LENTH]={"\0"};
bool file_open_condition=false;

void CalculateOffset(int fp){
  IMAGE_DOS_HEADER DosHeader;
  IMAGE_FILE_HEADER FileHeader;
  _read(fp,&DosHeader,sizeof(DosHeader)); //read system call
  OffDosHeader=0;
  OffFileHeader=DosHeader.e_lfanew+0x4;
  OffOptHeader=DosHeader.e_lfanew+0x18;
  _lseek(fp,OffFileHeader,SEEK_SET);
  _read(fp,&FileHeader,sizeof(FileHeader));
  OffSecHeader=OffOptHeader+FileHeader.SizeOfOptionalHeader;
  _lseek(fp,0,0);
}


void Calculate_Import_Offset(int fp){
    opt_header OPTH(fp);
    sec_header SECH(fp);
    SECH.RVA_Get_Need_Section(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    DWORD FileOffset=OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress-SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->PointerToRawData;
    OffImportDescriptor=static_cast<long>(FileOffset);
    _lseek(fp,0,0);
}




void QStringToCharArray (QString Qstr,char* ch){
    QByteArray ba;
    ba = Qstr.toLatin1();
    char* ptr = ba.data();
    int i=0;
    for(;i!=Qstr.size()+1;i+=1)
    ch[i]=ptr[i];
}
QString& useful_qstring(QString& a){
    QChar* c=a.begin();
    while(c!=(a.end())){
    if(*c=='\\'){
        *c='/';
        c+=1;
    }else
        c+=1;
    }
    return a;
}





//把数字转成Qstring，并以16进制显示
QString Number_x16_ToQstring(WORD b){
    QString a;
    a+="0x";
    return a+=QString("%1").toUpper().arg(b, LENTH, 16,QLatin1Char('0')).toUpper();   //4代表宽度，16表示16进制，空位补零
}
QString Number_x16_ToQstring(DWORD b){
    QString a;
    a+="0x";
    return a+=QString("%1").toUpper().arg(b, LENTH, 16,QLatin1Char('0')).toUpper();   //4代表宽度，16表示16进制，空位补零
}
QString Number_x16_ToQstring(LONG b){
    QString a;
    a+="0x";
    return a+=QString("%1").toUpper().arg(b, LENTH, 16,QLatin1Char('0')).toUpper();   //4代表宽度，16表示16进制，空位补零
}
QString Unsigned_Char_To_QString(unsigned char* ch){
    std::string str = reinterpret_cast <char*>(ch);
    QString Qstr = QString::fromStdString(str);
    return  Qstr;
}

QString Unsigned_Char_To_QString(const unsigned char* ch){
    unsigned char *a=const_cast<unsigned char*>(ch);
    std::string str = reinterpret_cast <char*>(a);
    QString Qstr = QString::fromStdString(str);
    return  Qstr;
}

//QString Unsigned_Char_To_QString(const unsigned char ch){
//    std::string str =static_cast<char>(ch);
//    QString Qstr = QString::fromStdString(str);
//    return  Qstr;
//}

DWORD QString_To_Number_x16(QString a){
    return a.toULong(nullptr,16);
}




//输出到文本框
void addQString(QString& a,const char* b){
    a+=b;
}
void addQString(QString& a,const char* b,const char* c){
    a+=b;
    a+=c;
}
void addQString(QString& a,const char* b,const char* c,const char* d){
    a+=b;
    a+=c;
    a+=d;
}
void addQString(QString& a,const char * b,WORD c,const char* d){
    a+=b;
    a+=Number_x16_ToQstring(c);
    a+=d;
}
void addQString(QString& a,const char * b,LONG c,const char* d){
    a+=b;
    a+=Number_x16_ToQstring(c);
    a+=d;
}
void addQString(QString& a,const char * b,DWORD c,const char* d){
    a+=b;
    a+=Number_x16_ToQstring(c);
    a+=d;
}
void addQString(QString& a,const char* b,BYTE *c,const char* d){
    a+=b;
    a+=Unsigned_Char_To_QString(c);
    a+=d;
}



void Show_in_QLineEdit(QLineEdit* edit,QString& b){
    edit->setText(b);
}

void Show_in_ViewTable(QStandardItemModel* model,int i,QString Name,QString VOffset,QString VSize,QString ROffset,QString RSize,QString Flags){
    model->setItem(i, 0, new QStandardItem(Name));
    model->setItem(i, 1, new QStandardItem(VOffset));
    model->setItem(i, 2, new QStandardItem(VSize));
    model->setItem(i, 3, new QStandardItem(ROffset));
    model->setItem(i, 4, new QStandardItem(RSize));
    model->setItem(i, 5, new QStandardItem(Flags));
    for(int c=0;c!=6;++c)
    model->item(i, c)->setTextAlignment(Qt::AlignCenter);
}

long int RVA_To_FileOffset(DWORD RVA){
    if(file_open_condition){
        int c=0;
        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        opt_header OPTH(c);
        _close(c);
        if(RVA>=OPTH.ptr_OptinonHead_ReadOnly->SizeOfHeaders){
            int c=0;
            _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
            sec_header SECH(c);
            SECH.RVA_Get_Need_Section(RVA);
            DWORD FileOffset=RVA-SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->PointerToRawData;
            _close(c);
            return static_cast<long>(FileOffset);
        }else return static_cast<long>(RVA);
    }else return 0;
}


DWORD FileOffset_To_RVA(QString File_Offset){
    int Int_File_Offset=File_Offset.toInt(nullptr,16);
    DWORD FileOffset=QString_To_Number_x16(File_Offset);
    if(file_open_condition){
        int c=0;
        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        opt_header OPTH(c);
        sec_header SECH(c);
        SECH.File_Offset_Get_Need_Section(Int_File_Offset);
        DWORD VA=FileOffset+OPTH.ptr_OptinonHead_ReadOnly->ImageBase+SECH.ptr_SectionHead_ReadOnly->VirtualAddress-SECH.ptr_SectionHead_ReadOnly->PointerToRawData;
        _close(c);
        return  VA-OPTH.ptr_OptinonHead_ReadOnly->ImageBase;
    }return  0;
}

void Show_in_ViewTable(QStandardItemModel* model,int i,QString Name,QString VOffset,QString VSize,QString ROffset,QString RSize){
    model->setItem(i, 0, new QStandardItem(Name));
    model->setItem(i, 1, new QStandardItem(VOffset));
    model->setItem(i, 2, new QStandardItem(VSize));
    model->setItem(i, 3, new QStandardItem(ROffset));
    model->setItem(i, 4, new QStandardItem(RSize));

    for(int c=0;c!=5;++c)
    model->item(i, c)->setTextAlignment(Qt::AlignCenter);
}



DWORD PEAlign(DWORD size, DWORD align)
{
    if ( size/ align * align == size)
                return size;
      return  (size / align + 1) * align;
}


