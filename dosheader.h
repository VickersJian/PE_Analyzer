#pragma once
#ifndef DOSHEADER_H
#define DOSHEADER_H
#include "allheader.h"
class dos_header
{
  private:
    IMAGE_DOS_HEADER dosh;
    int file_descriptor;

  public:
    dos_header(int f);
//    void show_header();
    QString show_header_in_window();
    const IMAGE_DOS_HEADER *ptr_DosHead_ReadOnly=&dosh;
};

//extern long int OffDosHeader;
#endif // DOSHEADER_H
