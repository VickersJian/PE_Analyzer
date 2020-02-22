#pragma once
#ifndef SUBORDINATE_FUNCTION_H
#define SUBORDINATE_FUNCTION_H
extern long int OffDosHeader;
extern long int OffFileHeader;
extern long int OffOptHeader;
extern long int OffSecHeader;
extern long int OffImportDescriptor;
#include"allheader.h"
#include<QTextStream>
#include<QLineEdit>
#include<QStandardItemModel>
typedef  struct _MAP_FILE_STRUCT{
    HANDLE hFile;
    HANDLE hMapping;
    LPVOID ImageBase;
} MAP_FILE_STRUCT;
void CalculateOffset(int fd);
void Calculate_Import_Offset(int fp);
long int RVA_To_FileOffset(DWORD RVA);
DWORD FileOffset_To_RVA(QString File_Offset);
//QString& useful_qstring(QString& a);
void QStringToCharArray (QString Qstr,char* ch);

QString Number_x16_ToQstring(WORD b);
QString Number_x16_ToQstring(DWORD b);
QString Number_x16_ToQstring(LONG b);

QString Unsigned_Char_To_QString(unsigned char* ch);
QString Unsigned_Char_To_QString(const unsigned char* ch);
//QString Unsigned_Char_To_QString(unsigned char ch);

DWORD QString_To_Number_x16(QString a);

void addQString(QString& a,const char* b);
void addQString(QString& a,const char* b,const char* c);
void addQString(QString& a,const char* b,const char* c,const char* d);
void addQString(QString& a,const char * b,WORD c,const char* d);
void addQString(QString& a,const char * b,DWORD c,const char* d);
void addQString(QString& a,const char * b,LONG c,const char* d);
void addQString(QString& a,const char* b,BYTE *c,const char* d);


void Show_in_QLineEdit(QLineEdit* edit,QString& b);

void Show_in_ViewTable(QStandardItemModel* model,int i,QString Name,QString VOffset,QString VSize,QString ROffset,QString RSize,QString Flags);
void Show_in_ViewTable(QStandardItemModel* model,int i,QString Name,QString VOffset,QString VSize,QString ROffset,QString RSize);

//文件或者内存对齐
DWORD PEAlign(DWORD size, DWORD dwAlignTo);
//void addQString(QString& a,WORD b);

//void addQString_Number_FRONT(QString& a,WORD b,const char* c);
//void addQString_Number_BACK(QString& a,WORD b,const char* c);

//void addQString_Number_FRONT(QString& a,LONG b,const char* c);
//void addQString_Number_BACK(QString& a,LONG b,const char* c);





//bool IsPEFile(LPVOID ImageBase);

#endif // SUBORDINATE_FUNCTION_H
