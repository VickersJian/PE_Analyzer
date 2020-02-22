#include "optionalheader.h"
#include"allheader.h"
#include"subordinate_function.h"

opt_header::opt_header(int f)
{
  file_descriptor=f;
  _lseek(file_descriptor,OffOptHeader,SEEK_SET);
  _read(f,&optHeader,sizeof(optHeader));
}


QString opt_header::show_header_in_window(){
    QString information;
    addQString(information,"\n","-------------OPTIONAL HEADER INFO--------------","\n");
    addQString(information,"magic: ",optHeader.Magic,"\n");
    addQString(information,"size of code: ",optHeader.SizeOfCode,"\n");
    addQString(information,"size of size of initialized data: ",optHeader.SizeOfInitializedData,"\n");
    addQString(information,"size of size of uninitialized data: ",optHeader.SizeOfUninitializedData,"\n");
    addQString(information,"address of entry point: ",optHeader.AddressOfEntryPoint,"\n");
    addQString(information,"base of code: ",optHeader.BaseOfCode,"\n");
    addQString(information,"base of data: ",optHeader.BaseOfData,"\n");
    addQString(information,"image base: ",optHeader.ImageBase,"\n");
    addQString(information,"section alignment: ",optHeader.SectionAlignment,"\n");
    addQString(information,"file alignment: ",optHeader.FileAlignment,"\n");
    addQString(information,"major operating system version: ",optHeader.MajorOperatingSystemVersion,"\n");
    addQString(information,"minor operating system version: ",optHeader.MinorOperatingSystemVersion,"\n");
    addQString(information,"major image version: ",optHeader.MajorImageVersion,"\n");
    addQString(information,"minorimage version: ",optHeader.MinorImageVersion,"\n");
    addQString(information,"major subsystem version: ",optHeader.MajorSubsystemVersion,"\n");
    addQString(information,"minor subsystem version: ",optHeader.MinorSubsystemVersion,"\n");
    addQString(information,"size of image: ",optHeader.SizeOfImage,"\n");
    addQString(information,"size of header: ",optHeader.SizeOfHeaders,"\n");
    addQString(information,"checksum: ",optHeader.CheckSum,"\n");
    addQString(information,"subsystem: ",optHeader.Subsystem,"\n");
    addQString(information,"dll characteristics: ",optHeader.DllCharacteristics,"\n");
    addQString(information,"size of stack reverse: ",optHeader.SizeOfStackReserve,"\n");
    addQString(information,"size of stack commit: ",optHeader.SizeOfStackCommit,"\n");
    addQString(information,"size of heap reverse: ",optHeader.SizeOfHeapReserve,"\n");
    addQString(information,"size of heap commit: ",optHeader.SizeOfHeapCommit,"\n");
    addQString(information,"loader flags: ",optHeader.LoaderFlags,"\n");
    addQString(information,"number of rva and size: ",optHeader.NumberOfRvaAndSizes,"\n");
    return information;
}

