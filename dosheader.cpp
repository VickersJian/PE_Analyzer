#include "dosheader.h"
#include"allheader.h"
#include"subordinate_function.h"

dos_header::dos_header(int f){
        file_descriptor=f;
        _lseek(file_descriptor,OffDosHeader,SEEK_SET);
        _read(f,&dosh,sizeof(dosh));
}


QString dos_header::show_header_in_window(){
    QString information;
    addQString(information,"\n","---------------DOS HEADER INFO---------------","\n");
    addQString(information,"e_magic: ",dosh.e_magic,"\n");
    addQString(information,"e_cblp: ",dosh.e_cblp,"\n");
    addQString(information,"e_cp: ",dosh.e_cp,"\n");
    addQString(information,"e_crlc: ",dosh.e_crlc,"\n");
    addQString(information,"e_cparhdr: ",dosh.e_cparhdr,"\n");
    addQString(information,"e_minalloc: ",dosh.e_minalloc,"\n");
    addQString(information,"e_maxalloc: ",dosh.e_maxalloc,"\n");
    addQString(information,"e_ss: ",dosh.e_ss,"\n");
    addQString(information,"e_sp: ",dosh.e_sp,"\n");
    addQString(information,"e_csum: ",dosh.e_csum,"\n");
    addQString(information,"e_ip: ",dosh.e_ip,"\n");
    addQString(information,"e_cs: ",dosh.e_cs,"\n");
    addQString(information,"e_lfarlc: ",dosh.e_lfarlc,"\n");
    addQString(information,"e_ovno: ",dosh.e_ovno,"\n");

    addQString(information,"e_oemid: ",dosh.e_oemid,"\n");
    addQString(information,"e_oeminfo: ",dosh.e_oeminfo,"\n");

    addQString(information,"e_lfanew: ",dosh.e_lfanew,"\n");
    return information;
}

