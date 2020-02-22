#include "fileheader.h"
#include"subordinate_function.h"

file_header::file_header(int f)
{
  file_descriptor=f;
  _lseek(file_descriptor,OffFileHeader,SEEK_SET);
  _read(f,&fileHeader,sizeof(fileHeader));
}

QString file_header::show_header_in_window(){
    QString information;
    addQString(information,"\n","-------------------FILE HEADER INFO-------------------","\n");
    addQString(information,"machine: ",fileHeader.Machine,"\n");
    addQString(information,"number of section: ",fileHeader.NumberOfSections,"\n");
    addQString(information,"time date stamp: ",fileHeader.TimeDateStamp,"\n");
    addQString(information,"pointer to symbol table: ",fileHeader.PointerToSymbolTable,"\n");
    addQString(information,"number of symbols: ",fileHeader.NumberOfSymbols,"\n");
    addQString(information,"sizeof optional header: ",fileHeader.SizeOfOptionalHeader,"\n");
    addQString(information,"characteristics: ",fileHeader.Characteristics,"\n");
    return information;
}
