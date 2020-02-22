
#include "import_descriptor.h"
#include"subordinate_function.h"
#include<share.h>


Import_Descriptor::Import_Descriptor(int f){
    file_descriptor=f;
    _lseek(file_descriptor,OffImportDescriptor,SEEK_SET);
    _read(f,&import_descriptor,sizeof(import_descriptor));
}
void Import_Descriptor::Show_Import_Descriptor_in_ViewTable(QStandardItemModel* model){
    _lseek(file_descriptor,OffImportDescriptor+static_cast<long>(sizeof(import_descriptor)),SEEK_SET);
    int i=0;
    int c=0;
    int lenth=0;
    int x=_sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
    if (x!=0){qDebug()<<x;goto exit;}
    while(import_descriptor.Characteristics||import_descriptor.OriginalFirstThunk||import_descriptor.ForwarderChain||import_descriptor.Name||import_descriptor.OriginalFirstThunk||import_descriptor.TimeDateStamp){
        unsigned char name[256]={1};
        unsigned char detect[3]={1};
        _lseek(c,RVA_To_FileOffset(import_descriptor.Name),SEEK_SET);
        while(detect[0]!='\0'||detect[1]!='\0'){
            _read(c,&detect,2);
            if(detect[0]!='\0'||detect[1]!='\0'){
                for(int i_=0;i_!=2;++i_,++lenth)
                    name[lenth]=detect[i_];
            }
        }
        name[lenth+1]='\0';
        Show_in_ViewTable(model,i,Unsigned_Char_To_QString(name),Number_x16_ToQstring(import_descriptor.OriginalFirstThunk),Number_x16_ToQstring(import_descriptor.TimeDateStamp),Number_x16_ToQstring(import_descriptor.ForwarderChain),Number_x16_ToQstring(import_descriptor.Name),Number_x16_ToQstring(import_descriptor.FirstThunk));
        ++i;
        lenth=0;
        _read(file_descriptor,&import_descriptor,sizeof(import_descriptor));
    }
    _close(c);
//    if(a)qDebug()<<a;
    exit:;
}



void Import_Descriptor::Show_Thunk_in_ViewTable(QStandardItemModel* model,QString OriginalFirstThunk_QString,QString FirstThunk_QString){
    int i=0;
    int lenth=0;
    DWORD OriginalFirstThunk_RVA=QString_To_Number_x16(OriginalFirstThunk_QString);
    DWORD FirstThunk_RVA=QString_To_Number_x16(FirstThunk_QString);



    if(OriginalFirstThunk_RVA!=0){
        long int OriginalFirstThunk_FileOffset=RVA_To_FileOffset(OriginalFirstThunk_RVA);
        _lseek(file_descriptor,OriginalFirstThunk_FileOffset,SEEK_SET);
        _read(file_descriptor,&thunk_data,sizeof(thunk_data));
        //用数据判断最高位是否为0
        while (thunk_data.u1.AddressOfData!=0) {
            if(!(thunk_data.u1.AddressOfData&2147483648)){

                _lseek(file_descriptor,RVA_To_FileOffset(thunk_data.u1.AddressOfData),SEEK_SET);
                _read(file_descriptor,&import_by_name,sizeof(import_by_name.Hint));

                unsigned char name[256]={8};
                unsigned char detect[2]={8};
                while(detect[0]!='\0'){
                    _read(file_descriptor,&detect,1);
                    if(detect[0]!='\0'){
                        for(int i_=0;i_!=1;++i_,++lenth)
                            name[lenth]=detect[i_];
                    }
                }
                name[lenth+1]='\0';

                Show_in_ViewTable(model,i,Number_x16_ToQstring(FileOffset_To_RVA(Number_x16_ToQstring(OriginalFirstThunk_FileOffset))),Number_x16_ToQstring(OriginalFirstThunk_FileOffset),Number_x16_ToQstring(thunk_data.u1.Ordinal),Number_x16_ToQstring(import_by_name.Hint),Unsigned_Char_To_QString(name));
                _lseek(file_descriptor,OriginalFirstThunk_FileOffset+=static_cast<long>(sizeof(thunk_data)),SEEK_SET);
                lenth=0;
            }else {
                //去掉最高位
                DWORD det=thunk_data.u1.AddressOfData^2147483648;
                unsigned char a[2]={'-'};
                Show_in_ViewTable(model,i,Number_x16_ToQstring(FileOffset_To_RVA(Number_x16_ToQstring(OriginalFirstThunk_FileOffset))),Number_x16_ToQstring(OriginalFirstThunk_FileOffset),Number_x16_ToQstring(thunk_data.u1.Ordinal),Unsigned_Char_To_QString(a),Number_x16_ToQstring(det));
            }
            ++i;
            _read(file_descriptor,&thunk_data,sizeof(thunk_data));
            }
    }




    else{
        long int FirstThunk_FileOffset=RVA_To_FileOffset(FirstThunk_RVA);
        _lseek(file_descriptor,FirstThunk_FileOffset,SEEK_SET);
        _read(file_descriptor,&thunk_data,sizeof(thunk_data));
        if(!(thunk_data.u1.AddressOfData&2147483648)){

            while (thunk_data.u1.AddressOfData!=0) {

                if(!(thunk_data.u1.AddressOfData&2147483648)){

                    _lseek(file_descriptor,RVA_To_FileOffset(thunk_data.u1.AddressOfData),SEEK_SET);
                    _read(file_descriptor,&import_by_name,sizeof(import_by_name.Hint));

                    unsigned char name[256]={1};
                    unsigned char detect[2]={1};
                    while(detect[0]!='\0'){
                        _read(file_descriptor,&detect,1);
                        if(detect[0]!='\0'){
                            for(int i_=0;i_!=1;++i_,++lenth)
                                name[lenth]=detect[i_];
                        }
                    }
                    name[lenth+1]='\0';

                    Show_in_ViewTable(model,i,Number_x16_ToQstring(FileOffset_To_RVA(Number_x16_ToQstring(FirstThunk_FileOffset))),Number_x16_ToQstring(FirstThunk_FileOffset),Number_x16_ToQstring(thunk_data.u1.Ordinal),Number_x16_ToQstring(import_by_name.Hint),Unsigned_Char_To_QString(name));
                    _lseek(file_descriptor,FirstThunk_FileOffset+=static_cast<long>(sizeof(thunk_data)),SEEK_SET);
                    lenth=0;
                }else {
                    //去掉最高位
                    DWORD det=thunk_data.u1.AddressOfData^2147483648;
                    unsigned char a[2]={'-'};
                    Show_in_ViewTable(model,i,Number_x16_ToQstring(FileOffset_To_RVA(Number_x16_ToQstring(FirstThunk_FileOffset))),Number_x16_ToQstring(FirstThunk_FileOffset),Number_x16_ToQstring(thunk_data.u1.Ordinal),Unsigned_Char_To_QString(a),Number_x16_ToQstring(det));
                }
                ++i;
                _read(file_descriptor,&thunk_data,sizeof(thunk_data));
                }

        }
    }
}


int Import_Descriptor::Number_Of_Import(){
    _lseek(file_descriptor,OffImportDescriptor+static_cast<long>(sizeof(import_descriptor)),SEEK_SET);
    int i=0;
    while(import_descriptor.Characteristics||import_descriptor.OriginalFirstThunk||import_descriptor.ForwarderChain||import_descriptor.Name||import_descriptor.OriginalFirstThunk||import_descriptor.TimeDateStamp){
        ++i;
        _read(file_descriptor,&import_descriptor,sizeof(import_descriptor));
    }return i;
}
