
#include "sectionheader.h"
#include"subordinate_function.h"


sec_header::sec_header(int f)
{
  IMAGE_FILE_HEADER fileheader;
  file_descriptor=f;
  _lseek(file_descriptor,OffFileHeader,SEEK_SET);
  _read(f,&fileheader,sizeof(fileheader));
  NumberOfSection=fileheader.NumberOfSections;

  _lseek(f,OffSecHeader,SEEK_SET);
  _read(f,&secHeader,sizeof(secHeader));
}

QString sec_header::show_header_in_window(){
    _lseek(file_descriptor,OffSecHeader+static_cast<long>(sizeof(secHeader)),SEEK_SET);
    int temp=NumberOfSection;
    QString information;
  addQString(information,"\n","--------------------SECTION HEADER INFO-----------------","\n");
  while(temp!=0)
  {
    addQString(information,"Name: ",secHeader.Name,"\n");
    addQString(information,"Virtual address: ",secHeader.VirtualAddress,"\n");
    addQString(information,"Virtual Size: ",secHeader.Misc.VirtualSize,"\n");
    addQString(information,"Size Of Raw Data: ",secHeader.SizeOfRawData,"\n");
    addQString(information,"Pointer To Raw Data: ",secHeader.PointerToRawData,"\n");
    addQString(information,"Pointer To Relocation: ",secHeader.PointerToRelocations,"\n");
    addQString(information,"Pointer To Line Number: ",secHeader.PointerToLinenumbers,"\n");
    addQString(information,"Number Of Line No: ",secHeader.NumberOfLinenumbers,"\n");
    addQString(information,"Number Of Relocation: ",secHeader.NumberOfRelocations,"\n");
    addQString(information,"Characteristics: ",secHeader.Characteristics,"\n");
    temp--;
    addQString(information,"\n","--------------------","\n");
    _read(file_descriptor,&secHeader,sizeof(secHeader));
  }
  return information;
}

void sec_header::Show_Sec_in_ViewTable(QStandardItemModel* model){
    _lseek(file_descriptor,OffSecHeader+static_cast<long>(sizeof(secHeader)),SEEK_SET);
    int temp=NumberOfSection;
    QString information;
    int i=0;
    while(temp!=0){
    Show_in_ViewTable(model,i,Unsigned_Char_To_QString(secHeader.Name),Number_x16_ToQstring(secHeader.VirtualAddress),Number_x16_ToQstring(secHeader.Misc.VirtualSize),Number_x16_ToQstring(secHeader.PointerToRawData),Number_x16_ToQstring(secHeader.SizeOfRawData),Number_x16_ToQstring(secHeader.Characteristics));
    temp--;
    ++i;
    _read(file_descriptor,&secHeader,sizeof(secHeader));
    }
}


void sec_header::RVA_Get_Need_Section(DWORD RVA){
    _lseek(file_descriptor,OffSecHeader+static_cast<long>(sizeof(secHeader)),SEEK_SET);
    bool detecte=false;
    int temp=NumberOfSection;
    while(NumberOfSection!=0&&!detecte){
        temp--;
        if(secHeader.VirtualAddress<=RVA&&RVA<secHeader.VirtualAddress+secHeader.SizeOfRawData){
            detecte=true;

        }
        else
            _read(file_descriptor,&secHeader,sizeof(secHeader));
    }
}


void sec_header::File_Offset_Get_Need_Section(int File_Offset){
    _lseek(file_descriptor,OffSecHeader+static_cast<long>(sizeof(secHeader)),SEEK_SET);
    DWORD FileOffset=secHeader.PointerToRawData;

    bool detecte=false;
    int temp=NumberOfSection;

    while(temp>0&&!detecte){
        temp--;


        if(static_cast<int>(FileOffset)<=File_Offset&&File_Offset<(static_cast<int>(FileOffset+secHeader.SizeOfRawData))){
            detecte=true;

        }
        else{
            _read(file_descriptor,&secHeader,sizeof(secHeader));
            FileOffset=secHeader.PointerToRawData;
        }
    }
}

void sec_header::Get_Last_Section(){
    int temp=NumberOfSection;
    _lseek(file_descriptor,OffSecHeader+static_cast<long>(static_cast<unsigned int>(temp-1)*sizeof(secHeader)),SEEK_SET);
    _read(file_descriptor,&secHeader,sizeof(secHeader));
}




void sec_header::Get_Enough_Size_Section(int Size){
    _lseek(file_descriptor,OffSecHeader+static_cast<long>(sizeof(secHeader)),SEEK_SET);
    DWORD LeftSize=secHeader.SizeOfRawData-secHeader.Misc.VirtualSize;

    bool detecte=false;
    int temp=NumberOfSection;

    while(temp>0&&!detecte){
        temp--;


        if(static_cast<int>(LeftSize)>Size){
            detecte=true;
        }
        else{
            _read(file_descriptor,&secHeader,sizeof(secHeader));
            LeftSize=secHeader.SizeOfRawData-secHeader.Misc.VirtualSize;
        }
    }
}
