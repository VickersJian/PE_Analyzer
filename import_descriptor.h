#pragma once
#ifndef IMPORT_DESCRIPTOR_H
#define IMPORT_DESCRIPTOR_H

#include"allheader.h"

class Import_Descriptor
{
private:
    IMAGE_IMPORT_DESCRIPTOR import_descriptor;
    IMAGE_THUNK_DATA thunk_data;
    IMAGE_IMPORT_BY_NAME import_by_name;
    int file_descriptor;
public:
    Import_Descriptor(int f);
    const IMAGE_IMPORT_DESCRIPTOR *ptr_ImportDescriptor_ReadOnly=&import_descriptor;
    void Show_Import_Descriptor_in_ViewTable(QStandardItemModel* model);
    void Show_Thunk_in_ViewTable(QStandardItemModel* model,QString OriginalFirstThunk_QString,QString FirstThunk_QString);
    int Number_Of_Import();
};




#endif // IMPORT_DESCRIPTOR_H
