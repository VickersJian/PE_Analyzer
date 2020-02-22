#pragma once
#ifndef FILEHEADER_H
#define FILEHEADER_H
#include "allheader.h"
//extern long int OffFileHeader;
class file_header
{
  private :
    IMAGE_FILE_HEADER fileHeader; //Represents the COFF header format
    int file_descriptor;

  public:
    file_header(int f);
//    void show_header();
    QString show_header_in_window();
    const IMAGE_FILE_HEADER *ptr_FileHead_ReadOnly=&fileHeader;
    IMAGE_FILE_HEADER *ptr_FileHead_ReadAndWrite=&fileHeader;
};
#endif // FILEHEADER_H
