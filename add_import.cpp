#include "add_import.h"
#include "ui_add_import.h"
#include"subordinate_function.h"
#include <malloc.h>
#include<share.h>
#include<QMessageBox>
#include"import_descriptor.h"
#include<errno.h>
#include<importtable.h>
Add_Import::Add_Import(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Import)
{
    ui->setupUi(this);
    setWindowTitle("Add Import");
}

Add_Import::~Add_Import()
{
    delete ui;
}

void Add_Import::on_pushButton_clicked()
{
    close();
}

void Add_Import::on_Add_clicked()
{
    int c=0;
    char dllname[256]={0};
    char apiname[256]={0};
//    ImportTable *stay=nullptr;
    QStringToCharArray(ui->DLL_Name->text(),dllname);
    QStringToCharArray(ui->API_Name->text(),apiname);
    if(file_open_condition)
    {

        //,_O_BINARY以二进制打开，_SH_DENYNO允许读取和写入，_S_IREAD | _S_IWRITE允许读取和写入。
        errno_t x= _sopen_s(&c,charfilename,_O_BINARY|_O_RDWR ,_SH_DENYNO,_S_IREAD | _S_IWRITE);
        qDebug()<<c;
        if(x!=0){QMessageBox::warning(this, tr("错误"), tr("_sopen_s在文件打开时出错，错误：%1").arg(x), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);goto exit;}
        opt_header OPTH(c);
        sec_header SECH(c);
        file_header FILEH(c);
        Import_Descriptor import_descriptor(c);


//        IMAGE_IMPORT_DESCRIPTOR

        SECH.Get_Last_Section();

        QString Last_secName=Unsigned_Char_To_QString(SECH.ptr_SectionHead_ReadOnly->Name);
        qDebug()<<Last_secName;
                if( Last_secName!=".VicSEC"){
            IMAGE_SECTION_HEADER newSection;
//        PEAlign( SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->Misc.VirtualSize,OPTH.ptr_OptinonHead_ReadOnly->SectionAlignment);


            memcpy(newSection.Name, ".VicSEC", 8); //节区头名字最多为8个字节,包括结尾的\0
            newSection.Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;
            //添加一个dll信息,所以在原来大小的基础上加一个导入表描述符大小加dll名字字符串大小+4个IMAGE_THUNK_DATA32大小
            int number_of_import=import_descriptor.Number_Of_Import();
            int sizeofimport=number_of_import*static_cast<int>(sizeof(IMAGE_IMPORT_DESCRIPTOR));
            newSection.Misc.VirtualSize = static_cast<DWORD>(sizeofimport+2*static_cast<int>(sizeof(IMAGE_IMPORT_DESCRIPTOR))+static_cast<int>(strlen(apiname)+1+2+strlen(dllname)+1)+4*static_cast<int>(sizeof(IMAGE_THUNK_DATA32)));
            newSection.NumberOfLinenumbers = 0;
            newSection.NumberOfRelocations = 0;
            newSection.PointerToLinenumbers = 0;
            newSection.PointerToRelocations = 0;
            newSection.SizeOfRawData = PEAlign(newSection.Misc.VirtualSize, OPTH.ptr_OptinonHead_ReadOnly->FileAlignment);

            newSection.VirtualAddress =PEAlign( SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->Misc.VirtualSize,OPTH.ptr_OptinonHead_ReadOnly->SectionAlignment);
            newSection.PointerToRawData = PEAlign( SECH.ptr_SectionHead_ReadOnly->PointerToRawData+SECH.ptr_SectionHead_ReadOnly->Misc.VirtualSize,OPTH.ptr_OptinonHead_ReadOnly->FileAlignment);
//            qDebug()<<Number_x16_ToQstring(newSection.VirtualAddress)<<Number_x16_ToQstring(newSection.Misc.VirtualSize)<<endl<<Number_x16_ToQstring(newSection.PointerToRawData)<<Number_x16_ToQstring(newSection.SizeOfRawData);
            _lseek(c,OffSecHeader+FILEH.ptr_FileHead_ReadOnly->NumberOfSections*static_cast<long>(sizeof(IMAGE_SECTION_HEADER)),SEEK_SET);
            _write(c,&newSection,sizeof(newSection));
            WORD newNumberOfSections=FILEH.ptr_FileHead_ReadAndWrite->NumberOfSections+1;
            DWORD newSizeOfImage=OPTH.ptr_OptinonHead_ReadAndWrite->SizeOfImage + newSection.SizeOfRawData;
            _lseek(c,OffFileHeader+0x2,SEEK_SET);
            _write(c,&newNumberOfSections,sizeof(WORD));
            _lseek(c,OffOptHeader+0x38,SEEK_SET);
            _write(c,&newSizeOfImage,sizeof(DWORD));
////        }
//            _lseek(c,0,SEEK_SET);
//            unsigned int FileSize =static_cast<unsigned int>(_lseek(c,0,SEEK_END));
            void* ptr_fp_temp=malloc(newSection.SizeOfRawData);

            memset(ptr_fp_temp, 0, newSection.SizeOfRawData);
            if(!ptr_fp_temp){
                QMessageBox::warning(this, tr("错误"), tr("分配缓存失败"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
                _close(c);
//                qDebug()<<FileSize<<"eero";
                goto exit;
            }
           void* fp_temp=ptr_fp_temp;
            //计算导入表的文件偏移
            SECH.RVA_Get_Need_Section(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
            DWORD File_Offset=OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress-SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->PointerToRawData;

            _lseek(c,static_cast<long>(File_Offset),SEEK_SET);
           int a= _read(c,fp_temp,static_cast<unsigned int>(sizeofimport));
           if(!a){
               QMessageBox::warning(this, tr("错误"), tr("写入缓存失败"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
               _close(c);
               goto exit;
           }
//           qDebug()<<*static_cast<DWORD*>(fp_temp);
//            free(fp_temp);
//           qDebug()<<fp_temp;
//           fp_temp=static_cast<int*>(fp_temp)+1;
//            qDebug()<<fp_temp;
//            qDebug()<<fp_temp;
//        SECH.Get_Enough_Size_Section(number_of_import*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+2*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+ 1+static_cast<int>(strlen(apiname)+strlen(dllname))+1+2+2*static_cast<int>(sizeof(IMAGE_THUNK_DATA32)));
////            void* memory_ptr =malloc(SECH.ptr_SectionHead_ReadOnly->SizeOfRawData);
//            WORD* ptr=reinterpret_cast<WORD*>(malloc(number_of_import*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+2*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+ 1+static_cast<int>(strlen(apiname)+strlen(dllname))+1+2+2*static_cast<int>(sizeof(IMAGE_THUNK_DATA32))));
//            //初始化内存

////            * memory_ptr = static_cast<char*>(memory);

//            //读取所有导入表





//            _read(c,ptr, OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size-sizeof(IMAGE_IMPORT_DESCRIPTOR));
//            //偏移到最后一个导入表之后

            //新增导入表
            IMAGE_IMPORT_DESCRIPTOR newImportDescriptor;
            newImportDescriptor.OriginalFirstThunk = newSection.VirtualAddress +static_cast<DWORD>( sizeofimport)+ 2*sizeof(IMAGE_IMPORT_DESCRIPTOR) + strlen(dllname) + 1;
            newImportDescriptor.FirstThunk =newImportDescriptor.OriginalFirstThunk + 2*sizeof(IMAGE_THUNK_DATA32);
            newImportDescriptor.ForwarderChain = 0;
            newImportDescriptor.Name = newSection.VirtualAddress+static_cast<DWORD>( sizeofimport)+ 2*sizeof(IMAGE_IMPORT_DESCRIPTOR);
            newImportDescriptor.TimeDateStamp = 0;
            fp_temp =static_cast<IMAGE_IMPORT_DESCRIPTOR*>(fp_temp)+number_of_import;
            memcpy(fp_temp,&newImportDescriptor, sizeof(IMAGE_IMPORT_DESCRIPTOR));
            fp_temp =static_cast<IMAGE_IMPORT_DESCRIPTOR*>(fp_temp)+1;
            IMAGE_IMPORT_DESCRIPTOR EndImportDescriptor;
            EndImportDescriptor.OriginalFirstThunk = 0;
            EndImportDescriptor.ForwarderChain = 0;
            EndImportDescriptor.Name = 0;
            EndImportDescriptor.TimeDateStamp = 0;
            memcpy(fp_temp, &EndImportDescriptor, sizeof(EndImportDescriptor));
            fp_temp =static_cast<IMAGE_IMPORT_DESCRIPTOR*>(fp_temp)+1;
//            qDebug()<<fp_temp;
//            ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2;
                //导入表描述符中的name字段,注意2者之间rva的关联
            memcpy(fp_temp, dllname, strlen(dllname)+1);

//            qDebug()<<fp_temp;
//                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1;
                 //添加注入的dll的IAT和INT.4个元素,前2个是给IAT的,后2个给INT的均以
                 IMAGE_THUNK_DATA32 thunk_data[4];
//                 thunk_data[0].u1.AddressOfData |= 0x80000000; //将最高位置1,表示是以序号导入
                 thunk_data[0].u1.AddressOfData = newImportDescriptor.OriginalFirstThunk+4*sizeof(IMAGE_THUNK_DATA32);
                 thunk_data[1].u1.AddressOfData = 0;
////                 thunk_data[2].u1.AddressOfData |= 0x80000000; //将最高位置1,表示是以序号导入
                 thunk_data[2].u1.AddressOfData =newImportDescriptor.OriginalFirstThunk+4*sizeof(IMAGE_THUNK_DATA32);
                 thunk_data[3].u1.AddressOfData =0;
                 fp_temp=static_cast<char*>(fp_temp)+strlen(dllname)+1;
                 memcpy(fp_temp, thunk_data, 4 * sizeof(IMAGE_THUNK_DATA32));
                 fp_temp=static_cast<IMAGE_THUNK_DATA32*>(fp_temp)+4;
                 memcpy(fp_temp,"0",  sizeof(WORD));
                 fp_temp=static_cast<WORD*>(fp_temp)+1;
                 memcpy(fp_temp,apiname, strlen(apiname)+1+2 );
                 _lseek(c,static_cast<long>(newSection.PointerToRawData),SEEK_SET);
                 _write(c,ptr_fp_temp,newSection.SizeOfRawData);
//                 memcpy(ptr_fp_temp,apiname, strlen(apiname)+1+2 );
//                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1+4 * sizeof(IMAGE_THUNK_DATA32);
//                 IMAGE_IMPORT_BY_NAME import_by_name;
//                 import_by_name.Hint=0;
//                 memcpy(ptr,& import_by_name,  sizeof(WORD));
//                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1+4 * sizeof(IMAGE_THUNK_DATA32)+sizeof(WORD);
//                 memcpy(ptr,apiname,strlen(apiname)+1);
//                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1+4 * sizeof(IMAGE_THUNK_DATA32)+sizeof(WORD) + strlen(apiname) + 1;
//                 _lseek(c,static_cast<long>(SECH.ptr_SectionHead_ReadOnly->PointerToRawData+SECH.ptr_SectionHead_ReadOnly->Misc.VirtualSize),SEEK_SET);
//                 _write(c,ptr,static_cast<unsigned int>(number_of_import*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+2*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+ 1+static_cast<int>(strlen(apiname)+strlen(dllname))+1+2+2*static_cast<int>(sizeof(IMAGE_THUNK_DATA32))));
             //绑定导入表改为0,保险一些
                 IMAGE_OPTIONAL_HEADER opth2;
                 _lseek(c,OffOptHeader,SEEK_SET);
                 _read(c,&opth2,sizeof(opth2));
                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = newSection.VirtualAddress;
                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = newSection.Misc.VirtualSize;
                _lseek(c,OffOptHeader,SEEK_SET);
                _write(c,&opth2,sizeof(opth2));

        if(c!=0)_close(c);c=0;
        qDebug()<<c;
        free(ptr_fp_temp);
        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
        qDebug()<<c;
        CalculateOffset(c);
        Calculate_Import_Offset(c);
//        Import_Descriptor ImportDES(c);
//        class ImportTable importtable;
//        stay=&importtable;
//        QStandardItemModel* model = new QStandardItemModel(&importtable);
//        model->setColumnCount(6);
//        model->setHeaderData(0,Qt::Horizontal, "DllName");
//        model->setHeaderData(1,Qt::Horizontal,"OriginalFirstThunk");
//        model->setHeaderData(2,Qt::Horizontal,"TimeDateStamp");
//        model->setHeaderData(3,Qt::Horizontal,"ForwarderChain");
//        model->setHeaderData(4,Qt::Horizontal,"Name");
//        model->setHeaderData(5,Qt::Horizontal,"FirstThunk");
//        ImportDES.Show_Import_Descriptor_in_ViewTable(model);

//        importtable.importlist->setModel(model);

//        importtable.importlist->setColumnWidth(0, 250);
//        importtable.importlist->setColumnWidth(1, 130);
//        importtable.importlist->setColumnWidth(2, 130);
//        importtable.importlist->setColumnWidth(3, 130);
//        importtable.importlist->setColumnWidth(4, 100);
//        importtable.importlist->setColumnWidth(5, 100);

        if(c!=0)_close(c);c=0;

        }














//        else{
////            _lseek(c,OffSecHeader+static_cast<long>(static_cast<unsigned int>(FILEH-1)*sizeof(IMAGE_SECTION_HEADER)),SEEK_SET);
////            _read(file_descriptor,&secHeader,sizeof(secHeader));
//            int number_of_import=import_descriptor.Number_Of_Import();
//            int sizeofimport=number_of_import*static_cast<int>(sizeof(IMAGE_IMPORT_DESCRIPTOR));
//            SECH.ptr_SectionHead_ReadAndWrite->Misc.VirtualSize+=static_cast<DWORD>(static_cast<int>(sizeof(IMAGE_IMPORT_DESCRIPTOR))+static_cast<int>(strlen(apiname)+1+2+strlen(dllname)+1)+4*static_cast<int>(sizeof(IMAGE_THUNK_DATA32)));
//            SECH.ptr_SectionHead_ReadAndWrite->SizeOfRawData=PEAlign(SECH.ptr_SectionHead_ReadOnly->Misc.VirtualSize, OPTH.ptr_OptinonHead_ReadOnly->FileAlignment);
//            _lseek(c,OffSecHeader+static_cast<long>(static_cast<unsigned int>(FILEH.ptr_FileHead_ReadOnly->NumberOfSections-1)*sizeof(IMAGE_SECTION_HEADER)),SEEK_SET);
//            _write(c,SECH.ptr_SectionHead_ReadOnly,sizeof(IMAGE_SECTION_HEADER));
//            void* ptr_fp_temp=malloc(SECH.ptr_SectionHead_ReadOnly->SizeOfRawData);

//            memset(ptr_fp_temp, 0, SECH.ptr_SectionHead_ReadOnly->SizeOfRawData);
//            if(!ptr_fp_temp){
//                QMessageBox::warning(this, tr("错误"), tr("分配缓存失败"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
//                _close(c);
////                qDebug()<<FileSize<<"eero";
//                goto exit;
//            }
//           void* fp_temp=ptr_fp_temp;
//            //计算导入表的文件偏移
//            SECH.RVA_Get_Need_Section(OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
//            DWORD File_Offset=OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress-SECH.ptr_SectionHead_ReadOnly->VirtualAddress+SECH.ptr_SectionHead_ReadOnly->PointerToRawData;

//            _lseek(c,static_cast<long>(File_Offset),SEEK_SET);
//           int a= _read(c,fp_temp,static_cast<unsigned int>(sizeofimport));
//           if(!a){
//               QMessageBox::warning(this, tr("错误"), tr("写入缓存失败"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
//               _close(c);
//               goto exit;
//           }


//            //新增导入表
//            IMAGE_IMPORT_DESCRIPTOR newImportDescriptor;
//            newImportDescriptor.OriginalFirstThunk = SECH.ptr_SectionHead_ReadOnly->VirtualAddress +static_cast<DWORD>( sizeofimport)+ 2*sizeof(IMAGE_IMPORT_DESCRIPTOR) + strlen(dllname) + 1;
//            newImportDescriptor.FirstThunk =newImportDescriptor.OriginalFirstThunk + 2*sizeof(IMAGE_THUNK_DATA32);
//            newImportDescriptor.ForwarderChain = 0;
//            newImportDescriptor.Name = SECH.ptr_SectionHead_ReadOnly->VirtualAddress+static_cast<DWORD>( sizeofimport)+ 2*sizeof(IMAGE_IMPORT_DESCRIPTOR);
//            newImportDescriptor.TimeDateStamp = 0;
//            fp_temp =static_cast<IMAGE_IMPORT_DESCRIPTOR*>(fp_temp)+number_of_import;
//            memcpy(fp_temp,&newImportDescriptor, sizeof(IMAGE_IMPORT_DESCRIPTOR));
//            fp_temp =static_cast<IMAGE_IMPORT_DESCRIPTOR*>(fp_temp)+1;
//            IMAGE_IMPORT_DESCRIPTOR EndImportDescriptor;
//            EndImportDescriptor.OriginalFirstThunk = 0;
//            EndImportDescriptor.ForwarderChain = 0;
//            EndImportDescriptor.Name = 0;
//            EndImportDescriptor.TimeDateStamp = 0;
//            memcpy(fp_temp, &EndImportDescriptor, sizeof(EndImportDescriptor));
//            fp_temp =static_cast<IMAGE_IMPORT_DESCRIPTOR*>(fp_temp)+1;
////            qDebug()<<fp_temp;
////            ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2;
//                //导入表描述符中的name字段,注意2者之间rva的关联
//            memcpy(fp_temp, dllname, strlen(dllname)+1);

////            qDebug()<<fp_temp;
////                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1;
//                 //添加注入的dll的IAT和INT.4个元素,前2个是给IAT的,后2个给INT的均以
//                 IMAGE_THUNK_DATA32 thunk_data[4];
////                 thunk_data[0].u1.AddressOfData |= 0x80000000; //将最高位置1,表示是以序号导入
//                 thunk_data[0].u1.AddressOfData = newImportDescriptor.OriginalFirstThunk+4*sizeof(IMAGE_THUNK_DATA32);
//                 thunk_data[1].u1.AddressOfData = 0;
//////                 thunk_data[2].u1.AddressOfData |= 0x80000000; //将最高位置1,表示是以序号导入
//                 thunk_data[2].u1.AddressOfData =newImportDescriptor.OriginalFirstThunk+4*sizeof(IMAGE_THUNK_DATA32);
//                 thunk_data[3].u1.AddressOfData =0;
//                 fp_temp=static_cast<char*>(fp_temp)+strlen(dllname)+1;
//                 memcpy(fp_temp, thunk_data, 4 * sizeof(IMAGE_THUNK_DATA32));
//                 fp_temp=static_cast<IMAGE_THUNK_DATA32*>(fp_temp)+4;
//                 memcpy(fp_temp,nullptr,  sizeof(WORD));
//                 fp_temp=static_cast<WORD*>(fp_temp)+1;
//                 memcpy(fp_temp,apiname, strlen(apiname)+1+2 );
//                 _lseek(c,static_cast<long>(newSection.PointerToRawData),SEEK_SET);
//                 _write(c,ptr_fp_temp,newSection.SizeOfRawData);
////                 memcpy(ptr_fp_temp,apiname, strlen(apiname)+1+2 );
////                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1+4 * sizeof(IMAGE_THUNK_DATA32);
////                 IMAGE_IMPORT_BY_NAME import_by_name;
////                 import_by_name.Hint=0;
////                 memcpy(ptr,& import_by_name,  sizeof(WORD));
////                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1+4 * sizeof(IMAGE_THUNK_DATA32)+sizeof(WORD);
////                 memcpy(ptr,apiname,strlen(apiname)+1);
////                 ptr =reinterpret_cast<WORD*>(reinterpret_cast<DWORD>(ptr) + OPTH.ptr_OptinonHead_ReadOnly->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size - sizeof(IMAGE_IMPORT_DESCRIPTOR))+sizeof(IMAGE_IMPORT_DESCRIPTOR)*2+strlen(dllname) + 1+4 * sizeof(IMAGE_THUNK_DATA32)+sizeof(WORD) + strlen(apiname) + 1;
////                 _lseek(c,static_cast<long>(SECH.ptr_SectionHead_ReadOnly->PointerToRawData+SECH.ptr_SectionHead_ReadOnly->Misc.VirtualSize),SEEK_SET);
////                 _write(c,ptr,static_cast<unsigned int>(number_of_import*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+2*static_cast<int>(sizeof(IMAGE_SECTION_HEADER))+ 1+static_cast<int>(strlen(apiname)+strlen(dllname))+1+2+2*static_cast<int>(sizeof(IMAGE_THUNK_DATA32))));
//             //绑定导入表改为0,保险一些
//                 IMAGE_OPTIONAL_HEADER opth2;
//                 _lseek(c,OffOptHeader,SEEK_SET);
//                 _read(c,&opth2,sizeof(opth2));
//                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
//                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
//                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = newSection.VirtualAddress;
//                opth2.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = newSection.Misc.VirtualSize;
//                _lseek(c,OffOptHeader,SEEK_SET);
//                _write(c,&opth2,sizeof(opth2));

//        _close(c);
//        free(ptr_fp_temp);
//        _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
//        Calculate_Import_Offset(c);
//        _close(c);
//        }
        QMessageBox::information(this, tr(""), tr("已添加成功，请勿重复添加"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

         if(c!=0)_close(c);c=0;
//         qDebug()<<c;
        close();

//               if(c==0){
//                   stay->show();
//                   stay->exec();
//               }
    }else QMessageBox::warning(this, tr("错误"), tr("请先打开文件"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    exit:if(c!=0)_close(c);
}
