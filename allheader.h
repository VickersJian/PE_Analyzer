#pragma once
#ifndef ALLHEADER_H
#define ALLHEADER_H

#define LENTH 8
#define FILE_LOCATION_LENTH 256
#include<stdio.h>
#include<iostream>
#include<windows.h>
#include<iomanip>
#include<stdlib.h>
#include<io.h>//_read,_lseek函数
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<conio.h>

#include<QString>
#include<QDebug>
#include"dosheader.h"
#include"fileheader.h"
#include"sectionheader.h"
#include"optionalheader.h"
extern char charfilename[FILE_LOCATION_LENTH];
extern bool file_open_condition;
using namespace std;
#endif // ALLHEADER_H
