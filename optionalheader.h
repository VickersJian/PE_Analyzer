#pragma once
#ifndef OPTIONALHEADER_H
#define OPTIONALHEADER_H
#include "allheader.h"

class opt_header
{
  private:
    IMAGE_OPTIONAL_HEADER optHeader; //Represents the optional header format.
    int file_descriptor;

  public:
    opt_header(int f);
//    void show_header();
    QString show_header_in_window();
    const IMAGE_OPTIONAL_HEADER *ptr_OptinonHead_ReadOnly=&optHeader;
    IMAGE_OPTIONAL_HEADER *ptr_OptinonHead_ReadAndWrite=&optHeader;
};

#endif // OPTIONALHEADER_H
