#pragma once
#ifndef SECTIONHEADER_H
#define SECTIONHEADER_H
#include"allheader.h"
#include<QStandardItemModel>
class sec_header
{
  private:
    IMAGE_SECTION_HEADER secHeader;
    int NumberOfSection;
    int file_descriptor;
  public:
    sec_header(int f);
    QString show_header_in_window();
    void Show_Sec_in_ViewTable(QStandardItemModel* model);
    void RVA_Get_Need_Section(DWORD RVA);
    void File_Offset_Get_Need_Section(int File_Offset);
    void Get_Last_Section();
    void Get_Enough_Size_Section(int Size);
    const  IMAGE_SECTION_HEADER *ptr_SectionHead_ReadOnly=&secHeader;
    IMAGE_SECTION_HEADER *ptr_SectionHead_ReadAndWrite=&secHeader;
};
#endif // SECTIONHEADER_H
