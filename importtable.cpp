#include "importtable.h"
#include "ui_importtable.h"
#include<QStandardItemModel>
#include<QStringListModel>
#include"subordinate_function.h"
#include<share.h>
#include"import_descriptor.h"
#include"add_import.h"
#include <QMenu>
#include <QAction>
#include<QMessageBox>
#include <QDialog>

ImportTable::ImportTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportTable)
{
    ui->setupUi(this);

    importlist=ui->tableView;
    details=ui->tableView_2;

    setWindowTitle("Import Table");

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableView_2->verticalHeader()->hide();
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    //发送信息到Show_Details()中
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT( Show_Details() ));

    //右键出现菜单
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));

}

ImportTable::~ImportTable()
{
    delete ui;
}

//右键菜单
void ImportTable::slotContextMenu(QPoint pos){
    QAction *Add_Import = new QAction ( "Add Import...",this);
    //练接到Add_Import()，处理菜单中的事件
    connect(Add_Import, SIGNAL(triggered()), this, SLOT(Add_Import()));
    QMenu *right_Menu = new QMenu(ui->tableView);
    right_Menu->addAction(Add_Import);

    QModelIndex index = ui->tableView->indexAt(pos);
    if (index.isValid()){
    right_Menu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
    }
}






void ImportTable::Add_Import(){
    class Add_Import add_import;
    add_import.show();
    add_import.exec();
//    close();

}







void ImportTable::Show_Details(){
    QAbstractItemModel *importlist_model = ui->tableView->model ();
    int row = ui-> tableView ->currentIndex().row();

    QModelIndex OriginalFirstThunk_index = importlist_model->index(row,1);
    QModelIndex FirstThunk_index = importlist_model->index(row,5);
    QString OriginalFirstThunk_QString = importlist_model->data(OriginalFirstThunk_index).toString();
    QString FirstThunk_QString = importlist_model->data(FirstThunk_index).toString();

    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal, "ThunkRVA");
    model->setHeaderData(1,Qt::Horizontal,"ThunkOffset");
    model->setHeaderData(2,Qt::Horizontal,"ThunkValue");
    model->setHeaderData(3,Qt::Horizontal,"Hint");
    model->setHeaderData(4,Qt::Horizontal,"ApiName");


    int c=0;
    _sopen_s(&c,charfilename,_O_BINARY,_SH_DENYWR,_S_IREAD);
    Import_Descriptor import_descriptor(c);

    import_descriptor.Show_Thunk_in_ViewTable(model,OriginalFirstThunk_QString,FirstThunk_QString);

    ui->tableView_2->setModel(model);

    ui->tableView_2->setColumnWidth(0, 100);
    ui->tableView_2->setColumnWidth(1, 130);
    ui->tableView_2->setColumnWidth(2, 130);
    ui->tableView_2->setColumnWidth(3, 130);
    ui->tableView_2->setColumnWidth(4, 350);


    int a=_close(c);
    if(a){QMessageBox::warning(this, tr("错误"), tr("_close()函数失败，错误 %1").arg(a), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);}

}
