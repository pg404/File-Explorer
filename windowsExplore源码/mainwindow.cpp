#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchthread.h"
#include "filterdailog.h"
#include "task.h"
#include "widget.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //以下为模型定义区域
    model = new QFileSystemModel;
    model ->setRootPath(":/");

    ui->treeView->setModel(model);
    ui->treeView->setHeaderHidden(true);

    //由于listView可能会被改变 故将listView的初始放在一个函数
    refreshView();
    listviewSlot();
    //隐藏详细信息
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);


    //以下为变量定义区域
    allIndex.append(model->index(":/"));
    indexPoint = allIndex.size()-1;
    viewPoint =0;

    ui->tableView->setShowGrid(false);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setModel(model);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui->treeView->header()->setSortIndicatorShown(true);
    ui->treeView->header()->setSectionsClickable(true);
    ui->tableView->hide();

    QStandardItemModel *extraModel=new QStandardItemModel;
    QStandardItem *item = new QStandardItem(QStringLiteral("   我的电脑"));

    extraModel->appendRow(item);
    ui->listViewExtra->setModel(extraModel);
    ui->listViewExtra->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->splitter->setStretchFactor(0,1); //调整比例 handle分离器调整
    ui->splitter->setStretchFactor(1,6);
    QPalette pal;
    pal.setColor(QPalette::Background,Qt::white);
    this->setPalette(pal);

    //以下为控件初始定义区域
    ui->searchLineEdit->setPlaceholderText(QStringLiteral("搜索本台电脑"));
    QAction *searchAction = new QAction(QIcon(":/new/icon/image/search.png"),QStringLiteral("搜索"),this);
    ui->searchLineEdit->addAction(searchAction,QLineEdit::TrailingPosition);

    //以下为样式表区域
    //ui->searchLineEdit->setStyleSheet("border-color:black; border-width:5px; border-style:solid; border-left-style:transparents");

    ui->treeView->setStyleSheet("border-color:rgb(248,248,248); border-width:1px; border-style:solid; border-top-style:transparent");

    ui->goButton->setStyleSheet("QPushButton{border-image: url(:/new/icon/image/go_1.png);}");
    ui->goBackButton->setStyleSheet("QPushButton{border-image: url(:/new/icon/image/back1.png);}");
    ui->backButton->setStyleSheet("QPushButton{border-image: url(:/new/icon/image/shift_up.png);}");


    //连接按钮事件与相应槽函数
    QObject::connect(ui->goButton,SIGNAL(clicked(bool)),this,SLOT(goButtonSlot()));
    QObject::connect(ui->goBackButton,SIGNAL(clicked(bool)),this,SLOT(goBackButtonSlot()));
    QObject::connect(ui->backButton,SIGNAL(clicked(bool)),this,SLOT(backButtonSlot()));

    QObject::connect(ui->updBTN,SIGNAL(clicked(bool)),this,SLOT(updateSlot()));
    QObject::connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickSlot(QModelIndex)));  //setRootIndex打开当前目录

    QObject::connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)) ,this,SLOT(doubleClickSlot(QModelIndex)));
    QObject::connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickSlot(QModelIndex)));

    QObject::connect(ui->actionList,SIGNAL(triggered(bool)),this,SLOT(listviewSlot()));
    QObject::connect(ui->actionBIcon,SIGNAL(triggered(bool)),this,SLOT(bigiconSlot()));
    QObject::connect(ui->actionMIcon,SIGNAL(triggered(bool)),this,SLOT(middleiconSlot()));
    QObject::connect(ui->actionSIcon,SIGNAL(triggered(bool)),this,SLOT(smalliconSlot()));
    QObject::connect(ui->actionListinfo,SIGNAL(triggered(bool)),this,SLOT(listinfoSlot()));

    QObject::connect(searchAction,SIGNAL(triggered(bool)),this,SLOT(searchFileSlot()));

    QObject::connect(ui->action_txt,SIGNAL(triggered(bool)),this,SLOT(filterSlot()));
    QObject::connect(ui->action_rar,SIGNAL(triggered(bool)),this,SLOT(filterSlot()));
    QObject::connect(ui->action_jpg,SIGNAL(triggered(bool)),this,SLOT(filterSlot()));
    QObject::connect(ui->action_mp4,SIGNAL(triggered(bool)),this,SLOT(filterSlot()));
    QObject::connect(ui->action_mp3,SIGNAL(triggered(bool)),this,SLOT(filterSlot()));
    QObject::connect(ui->action_helpsort,SIGNAL(triggered(bool)),this,SLOT(showHelpFilter()));
    QObject::connect(ui->actionTask,SIGNAL(triggered()),this,SLOT(openrask()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

//视图基本按钮

void MainWindow::refreshView()
{

    ui->listView->setModel(model);
    ui->listViewExtra->setStyleSheet("border-top-color:rgb(220,220,220); border-top-width:1px; border-top-style:solid; "
                                     "border-right-width:1px; border-right-color:rgb(248,248,248); border-right-style:solid;");

    ui->listView->setStyleSheet("border-top-color:rgb(220,220,220); border-width:1px; border-top-style:solid;");
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
}

void MainWindow::juiceButton()
{
    if (indexPoint == allIndex.size()-1)
        ui->goButton->setStyleSheet("QPushButton{border-image: url(:/new/icon/image/go_1.png);}");
    else
        ui->goButton->setStyleSheet("QPushButton{border-image: url(:/new/icon/image/go_2.png);}");
    if (indexPoint>0)
        ui->goBackButton->setStyleSheet("QPushButton{border-image: url(:/new/icon/image/back2.png);}");
    else
        ui->goBackButton->setStyleSheet("QPushButton{border-image: url(:/new/icon/image/back1.png);}");
}

//前进功能
void MainWindow::goButtonSlot()
{
    if (indexPoint+1 != allIndex.size())
    {
        ui->listView->setRootIndex(allIndex.at(++indexPoint));
        QString filePath = model->filePath(allIndex.at(indexPoint));
        ui->ShowExploreEdit->setText(filePath);
        ui->searchLineEdit->setPlaceholderText(QStringLiteral("搜索“")+filePath+QStringLiteral("”"));
    }
    juiceButton();
    QModelIndex ti=ui->listView->currentIndex();
    ui->tableView->setRootIndex(ti);
}

//返回上一级功能
void MainWindow::backButtonSlot()
{
    indexNow = ui->listView->model()->parent(indexNow);
    ui->listView->setRootIndex(indexNow);
    ui->tableView->setRootIndex(indexNow);
    QString filePath = model->filePath(indexNow);
    ui->ShowExploreEdit->setText(filePath);
    ui->searchLineEdit->setPlaceholderText(QStringLiteral("搜索“")+filePath+QStringLiteral("”"));
}

//后退一步功能
void MainWindow::goBackButtonSlot()
{
    if (indexPoint > 0)
    {
        ui->listView->setRootIndex(allIndex.at(--indexPoint));
        QString filePath = model->filePath(allIndex.at(indexPoint));
        ui->ShowExploreEdit->setText(filePath);
        ui->searchLineEdit->setPlaceholderText(QStringLiteral("搜索“")+filePath+QStringLiteral("”"));
    }
    juiceButton();
    QModelIndex ti=ui->listView->currentIndex();
    ui->tableView->setRootIndex(ti);
}

//更新按钮功能
void MainWindow::updateSlot()
{
    ui->listView->setRootIndex(indexNow);
    ui->tableView->setRootIndex(indexNow);
}

//双击功能（不区分左右键）(增加了文件路径）
void MainWindow::doubleClickSlot(QModelIndex a)
{
    indexNow = a; //存储QModelIndex打开的文件的路径
    QFileInfo file = model->fileInfo(a);
    if(file.isDir()) //判断点击文件是否为文件夹，是则打开文件夹，否则是文件，打开文件内容
    {
    allIndex.remove(indexPoint+1,allIndex.size()-indexPoint-1);
    allIndex.append(a);
    indexPoint = allIndex.size()-1;
    ui->listView->setRootIndex(a);
    ui->tableView->setRootIndex(a);

    QString filePath = model->filePath(a);
    ui->ShowExploreEdit->setText(filePath);
    ui->searchLineEdit->setPlaceholderText(QStringLiteral("搜索“")+filePath+QStringLiteral("”"));
    juiceButton();
    }
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(file.absoluteFilePath()));//用对应程序打开文件
}


//8.18添加

bool MainWindow::copyDirectoryFiles(const QString &fromDir, const QString &toDir)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists())
    {    // 如果目标目录不存在，则进行创建
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir())
        {    // 当为目录时，递归的进行copy
            if(!copyDirectoryFiles(fileInfo.filePath(),
                                   targetDir.filePath(fileInfo.fileName())))
                return false;
        }
        else{            // 当允许覆盖操作时，将旧文件进行删除操作
            /*if(targetDir.exists(fileInfo.fileName()))
                {
               targetDir.remove(fileInfo.fileName());
                }
            */
            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}

//删除文件夹
bool MainWindow::removeFolderContent(QFileInfo fileList)
{
    if(fileList.isDir())
    {
        int childCount =0;
        QString dir = fileList.filePath();
        QDir thisDir(dir);
        childCount = thisDir.entryInfoList().count();
        QFileInfoList newFileList = thisDir.entryInfoList();
        if(childCount>2)
        {
            for(int i=0;i<childCount;i++)
            {
                if(newFileList.at(i).fileName().operator ==(".")|newFileList.at(i).fileName().operator ==(".."))
                {
                    continue;
                }
                removeFolderContent(newFileList.at(i));
            }
        }
        fileList.absoluteDir().rmpath(fileList.fileName());
    }
    else if(fileList.isFile())
    {
        fileList.absoluteDir().remove(fileList.fileName());
    }
    return true;
}

//查找同名文件（新建txt.word.exel用到）
bool MainWindow::findSameFile(QString filename,QString folderDir)
{

    QDir dir(folderDir);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists())
    {return false;}//文件不存，则返回false
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    int infoNum=fileList.size();
    for(int i=infoNum-1;i>=0;i--)
    {
        curFile = fileList[i];
        if(0 == filename.compare(curFile.fileName()))
        {
            return true;
        }
    }
    return false;
}

//获取选中文件路径
void MainWindow::getPath(const QModelIndex &index)
{
    selectFileIndex = index;//获取选中文件
    isDelete = true;
}


//8.19

int MainWindow::findAllFileAndDir(const QString &dirPath)
{
    QDir dir(dirPath);
    int result = 0;
    foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::Dirs | QDir::Files))
    {
        QString strName = fileInfo.fileName();
        if ((strName == QString(".")) || (strName == QString("..")))
            continue;
        if (fileInfo.isDir())
        {
            QString str = dirPath + "/" + strName + "/";
            result++;
            result += findAllFileAndDir(str);
        }
        else
        {
            result++;
        }
    }
    return result;
}



//添加鼠标右键事件功能

void MainWindow::contextMenuEvent(QContextMenuEvent *)
{
    QMenu *menu = new QMenu(this);

    //添加右键菜单
    QAction *newDAction = new QAction(QStringLiteral("New Folder"),this);
    QMenu *CreateFilemenu = new QMenu(QStringLiteral("New File"),this);
    QAction *newTxtAction = new QAction(QStringLiteral("Note"),this);
    QAction *newExcelAction = new QAction(QStringLiteral("Excel"),this);
    QAction *newWordAction = new QAction(QStringLiteral("Word "),this);

    menu->addAction(newDAction);
    menu->addAction(CreateFilemenu->menuAction());

    CreateFilemenu->addAction(newTxtAction);
    CreateFilemenu->addAction(newExcelAction);
    CreateFilemenu->addAction(newWordAction);

    QAction *fileopen = menu->addAction(QStringLiteral("Open"));
    QAction *filecopy = menu->addAction(QStringLiteral("Copy"));
    QAction *filepaste = menu->addAction(QStringLiteral("Paste"));

    QAction *filedelete = menu->addAction(QStringLiteral("Delete"));
    QAction *filerename = menu->addAction(QStringLiteral("Rename"));
    QAction *filedetail = menu->addAction(QStringLiteral("Detail"));

    menu->move(cursor().pos());
    menu->show();
    if(!isCopy)
         filepaste->setEnabled(false);
     else
         filepaste->setEnabled(true);
    if(isDelete){
        filedelete->setEnabled(true);
    }else{
        filedelete->setEnabled(false);
    }
    //连接右键菜单按钮与相应功能槽函数
    connect(newDAction,SIGNAL(triggered()),this,SLOT(newDFile()));
    connect(newTxtAction,SIGNAL(triggered()),this,SLOT(newTxtFile()));
    connect(newExcelAction,SIGNAL(triggered()),this,SLOT(newExelFile()));
    connect(newWordAction,SIGNAL(triggered()),this,SLOT(newWordFile()));

    connect(fileopen,SIGNAL(triggered()),this,SLOT(fileopenslot()));
    connect(filedelete, SIGNAL(triggered(bool)), this, SLOT(filedeleteslot()));
    connect(filecopy, SIGNAL(triggered(bool)), this, SLOT(filecopyslot()));
    connect(filepaste, SIGNAL(triggered(bool)), this, SLOT(filepasteslot()));
    connect(filerename, SIGNAL(triggered(bool)), this, SLOT(filerenameslot()));
    connect(filedetail, SIGNAL(triggered(bool)), this, SLOT(filedetailslot()));

}



/*#########################*/
//#########鼠标右键槽函数
/*#########################*/

//new txt.file
void MainWindow::newTxtFile()
{
    /*#ifdef _WIN32
    path.replace("/","\\");//将路径中的'/'替换为windows中的'\\'
    path = path + "\\";
#else
    path = path + "/";//LINUX路径
#endif
    */
    QString path = model->filePath(indexNow)+"/";
    path.replace("/","\\");
    if(!findSameFile(tr("Note.txt"),path))
    {
        QFile file(path+tr("Note.txt"));
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else
    {
        int i = 1;
        QString num;
        while(1)
        {
            QString filename = tr("Note")+num.setNum(i)+".txt";
            if(!findSameFile(filename,path))
            {
                QFile file(path+filename);
                file.open(QIODevice::WriteOnly);
                file.close();
                return;
            }
            i++;
        }
    }
}

//new word file
void MainWindow::newWordFile()
{
     QString path = model->filePath(indexNow)+"/";
     path.replace("/","\\");
    if(!findSameFile(tr("Word.doc"),path))
    {
        QFile file(path+tr("Word.doc"));
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else
    {
        int i = 1;
        QString num;
        while(1)
        {
            QString filename = tr("Word")+num.setNum(i)+".doc";
            if(!findSameFile(filename,path))
            {
                QFile file(path+filename);
                file.open(QIODevice::WriteOnly);
                file.close();
                return;
            }
            i++;
        }
    }
}

//new exel file
void MainWindow::newExelFile()
{
     QString path = model->filePath(indexNow)+"/";
     path.replace("/","\\");
    if(!findSameFile(tr("Excel.xls"),path))
    {
        QFile file(path+tr("Excel.xls"));
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else
    {
        int i = 1;
        QString num;
        while(1)
        {
            QString filename = tr("Excel")+num.setNum(i)+".xls";
            if(!findSameFile(filename,path))
            {
                QFile file(path+filename);
                file.open(QIODevice::WriteOnly);
                file.close();
                return;
            }
            i++;
        }
    }
}

//newDfile
void MainWindow::newDFile()
{
    QModelIndex index = ui->listView->currentIndex();
        if (!index.isValid()) {
            return;
        }
        QString dirName = QInputDialog::getText(this,
                                                tr("Create Directory"),
                                                tr("Directory name"));
        if (!dirName.isEmpty()) {
            if (!model->mkdir(index, dirName).isValid()) {
                QMessageBox::information(this,
                                         tr("Create Directory"),
                                         tr("Failed to create the directory"));
            }
        }
}

//打开
void MainWindow::fileopenslot()
{
   QModelIndex a=ui->listView->currentIndex();
   doubleClickSlot(a);
}

//删除
void MainWindow::filedeleteslot()
{
   QModelIndex a =ui->listView->currentIndex();
    if (!a.isValid()) {
            return;
        }
        bool ok;
        if (model->fileInfo(a).isDir())
        {
            ok = model->rmdir(a);
        }
        else
        {
            ok = model->remove(a);
        }
        if (!ok)
        {
            QMessageBox::information(this,
                             tr("Remove"),
                             tr("Failed to remove %1").arg(model->fileName(a)));
        }
}

//重命名
void MainWindow::filerenameslot()
{
    QModelIndex a=ui->listView->currentIndex();
    model->setReadOnly(false);
    QFileInfo f=model->fileInfo(a);
        if (!a.isValid()) {
            return;
        }
        QString newName= QInputDialog::getText(this,
                                                tr("Rename File"),
                                                tr("newname"));
        QString startext = f.suffix();
        QFileInfo fi(newName);
        QString endext=fi.suffix();
        if (newName.isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Newname is empty!!!"));
            return;
        }
        else
        {
           if(startext!=endext)
           {
               if(endext==NULL)
           {
                   newName=newName+"."+startext;
               model->setData(a, newName, Qt::EditRole);
           }
           else{
             QMessageBox::StandardButton res= QMessageBox::warning(NULL,
                                                        "warning", "后缀名改变可能会导致文件不可用！",
                                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                        if(res==QMessageBox::No)
                          return;
                        if(res==QMessageBox::Yes)
                             //model->setData(index, value, Qt::EditRole);
           model->setData(a, newName, Qt::EditRole);
           }
           }
           else
                   model->setData(a,newName,Qt::EditRole);
        }
}

//文件信息
void MainWindow::filedetailslot()
{
    QDialog *finfo=new QDialog();
    QModelIndex fi=ui->listView->currentIndex();
    QFileInfo fin=model->fileInfo(fi);
    QLabel *name=new QLabel(tr("name :"));
    QIcon ficon=model->fileIcon(fi);
    QLabel *type=new QLabel(tr("type :"));
    QLabel *path=new QLabel(tr("path :"));
    QLabel *size=new QLabel(tr("size :"));
    QLabel *creat=new QLabel(tr("creatTime :"));
    QLabel *lasttime=new QLabel(tr("lastRead :"));
    QLabel *lastmodified=new QLabel(tr("lastModified :"));

    QLabel *nameline =new QLabel(fin.fileName());
    QLabel *typeline = new QLabel(model->type(fi));
    QLabel *pathline = new QLabel(fin.filePath());
    QLabel *sizeline = new QLabel(QString::number(fin.size()));
    QLabel *creatline = new QLabel(fin.created().toString("yyyy-MM-dd,hh:mm:ss"));
    QLabel *lTimeline = new QLabel(fin.lastRead().toString("yyyy-MM-dd,hh:mm:ss"));
    QLabel *lModifiedline = new QLabel(fin.lastModified().toString("yyyy-MM-dd,hh:mm:ss"));

    QVBoxLayout *fInfoName =new QVBoxLayout;
    fInfoName->addWidget(name);
    fInfoName->addWidget(type);
    fInfoName->addWidget(path);
    fInfoName->addWidget(size);
   // fInfoName->addItem(spaceitem1);
    fInfoName->addWidget(creat);
    fInfoName->addWidget(lasttime);
    fInfoName->addWidget(lastmodified);

    QVBoxLayout *fInfoData =new QVBoxLayout;
    fInfoData->addWidget(nameline);
    fInfoData->addWidget(typeline);
    fInfoData->addWidget(pathline);
    fInfoData->addWidget(sizeline);
   // fInfoData->addItem(spaceitem1);
    fInfoData->addWidget(creatline);
    fInfoData->addWidget(lTimeline);
    fInfoData->addWidget(lModifiedline);

    QHBoxLayout *datalayout =new QHBoxLayout;
    datalayout->addLayout(fInfoName);
    datalayout->addLayout(fInfoData);
    finfo->setLayout(datalayout);
    finfo->show();

}


void MainWindow::filecopyslot()
{
    copyIdex = selectFileIndex;
    isCopy = true;
    copyPath = model->fileInfo(copyIdex).absoluteFilePath();
#ifdef _WIN32
    copyPath.replace("/","\\");
    int begin = copyPath.lastIndexOf("\\")+1;
#else
    int begin = copyPath.lastIndexOf("/")+1;
#endif
    int end = copyPath.lastIndexOf(".")-1;
     copyPostfixName = copyPath.mid(end+1);
     copyFileName = copyPath.mid(begin,end-begin+1);
    isCopyFile = false;
    if(model->fileInfo(copyIdex).isFile())
    {
        isCopyFile = true;
    }
}

void MainWindow::filepasteslot()
{
    newPath = model->fileInfo(DirIndex).absoluteFilePath();
    #ifdef _WIN32
         newPath.replace("/","\\");
    #endif
        QString filename = copyFileName + copyPostfixName;
        if(isCopyFile){
            if(findSameFile(filename,newPath)){//文件存在
                int i = 1;
                QString num;
                while(1){
                    filename = copyFileName+tr("_Copy") + num.setNum(i)+copyPostfixName;
                    if(!findSameFile(filename,newPath))
                    {
                        break;
                    }
                    i++;
                }//while(1)
            }
    #ifdef _WIN32
           newPath = newPath + "\\" + filename;
    #else
            newPath = newPath + "/" + filename;
    #endif
            QFile::copy(copyPath,newPath);
        }else{
    #ifdef _WIN32
            int begin = copyPath.lastIndexOf("\\")+1;
    #else
            int begin = copyPath.lastIndexOf("/")+1;
    #endif
            QString filename = copyPath.mid(begin);
            if(findSameFile(filename,newPath)){//文件夹存在
                int i = 1;
                QString num;
                while(1){
                    QString filename1 = filename+tr("_Copy") + num.setNum(i);
                    if(!findSameFile(filename1,newPath))
                    {
                        filename = filename1;
                        break;
                    }
                    i++;
                }//while(1)
             }
    #ifdef _WIN32
            newPath = newPath+"\\"+filename;
    #else
            newPath = newPath+"/"+filename;
    #endif
            if(!copyDirectoryFiles(copyPath,newPath))
              qDebug()<<"no!";
        }

}


/*#########################*/
//#########文件按钮下槽函数
/*#########################*/

//打开
void MainWindow::on_actionOpen_O_triggered()
{
    QModelIndex a=ui->listView->currentIndex();
    doubleClickSlot(a);
}

//txt
void MainWindow::on_actionTxt_triggered()
{
    QString path = model->filePath(indexNow)+"/";
    path.replace("/","\\");
    if(!findSameFile(tr("Note.txt"),path))
    {
        QFile file(path+tr("Note.txt"));
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else
    {
        int i = 1;
        QString num;
        while(1)
        {
            QString filename = tr("Note")+num.setNum(i)+".txt";
            if(!findSameFile(filename,path))
            {
                QFile file(path+filename);
                file.open(QIODevice::WriteOnly);
                file.close();
                return;
            }
            i++;
        }
    }
}

//word
void MainWindow::on_actionWord_triggered()
{
    QString path = model->filePath(indexNow)+"/";
    path.replace("/","\\");
    if(!findSameFile(tr("Word.doc"),path))
    {
        QFile file(path+tr("Word.doc"));
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else
    {
        int i = 1;
        QString num;
        while(1)
        {
            QString filename = tr("Word")+num.setNum(i)+".doc";
            if(!findSameFile(filename,path))
            {
                QFile file(path+filename);
                file.open(QIODevice::WriteOnly);
                file.close();
                return;
            }
            i++;
        }
    }
}

//exel
void MainWindow::on_actionExel_triggered()
{
    QString path = model->filePath(indexNow)+"/";
    path.replace("/","\\");
    if(!findSameFile(tr("Excel.xls"),path))
    {
        QFile file(path+tr("Excel.xls"));
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else
    {
        int i = 1;
        QString num;
        while(1)
        {
            QString filename = tr("Excel")+num.setNum(i)+".xls";
            if(!findSameFile(filename,path))
            {
                QFile file(path+filename);
                file.open(QIODevice::WriteOnly);
                file.close();
                return;
            }
            i++;
        }
    }
}


/*#########################*/
//#########编辑按钮下槽函数
/*#########################*/


void MainWindow::on_actionCopy_triggered()
{
    QModelIndex a = ui->listView->currentIndex();
    //QFileInfo file =

}

void MainWindow::on_actionPaste_triggered()
{

}

//删除
void MainWindow::on_actionDelete_triggered()
{
    QModelIndex a =ui->listView->currentIndex();
     if (!a.isValid()) {
             return;
         }
         bool ok;
         if (model->fileInfo(a).isDir())
         {
             ok = model->rmdir(a);
         }
         else
         {
             ok = model->remove(a);
         }
         if (!ok)
         {
             QMessageBox::information(this,
                              tr("Remove"),
                              tr("Failed to remove %1").arg(model->fileName(a)));
         }
}

//重命名
void MainWindow::on_actionRename_triggered()
{
    QModelIndex a=ui->listView->currentIndex();
    model->setReadOnly(false);
    QFileInfo f=model->fileInfo(a);
        if (!a.isValid()) {
            return;
        }
        QString newName= QInputDialog::getText(this,
                                                tr("Rename File"),
                                                tr("newname"));
        QString startext = f.suffix();
        QFileInfo fi(newName);
        QString endext=fi.suffix();
        if (newName.isEmpty())
        {
            QMessageBox::warning(this,tr("Warning"),tr("Newname is empty!!!"));
            return;
        }
        else
        {
           if(startext!=endext)
           {
               if(endext==NULL)
           {
                   newName=newName+"."+startext;
               model->setData(a, newName, Qt::EditRole);
           }
           else{
             QMessageBox::StandardButton res= QMessageBox::warning(NULL,
                                                        "warning", "后缀名改变可能会导致文件不可用！",
                                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                        if(res==QMessageBox::No)
                          return;
                        if(res==QMessageBox::Yes)
                             //model->setData(index, value, Qt::EditRole);
           model->setData(a, newName, Qt::EditRole);
           }
           }
           else
                   model->setData(a,newName,Qt::EditRole);
        }
}

//属性按钮
void MainWindow::on_action_shuxing_triggered()
{

    QDialog *finfo=new QDialog();
    QModelIndex fi=ui->listView->currentIndex();
    QFileInfo fin=model->fileInfo(fi);
    QLabel *name=new QLabel(tr("name :"));
    QIcon ficon=model->fileIcon(fi);
    QLabel *type=new QLabel(tr("type :"));
    QLabel *path=new QLabel(tr("path :"));
    QLabel *size=new QLabel(tr("size :"));
    QLabel *creat=new QLabel(tr("creatTime :"));
    QLabel *lasttime=new QLabel(tr("lastRead :"));
    QLabel *lastmodified=new QLabel(tr("lastModified :"));

    QLabel *nameline =new QLabel(fin.fileName());
    QLabel *typeline = new QLabel(model->type(fi));
    QLabel *pathline = new QLabel(fin.filePath());
    QLabel *sizeline = new QLabel(QString::number(fin.size()));
    QLabel *creatline = new QLabel(fin.created().toString("yyyy-MM-dd,hh:mm:ss"));
    QLabel *lTimeline = new QLabel(fin.lastRead().toString("yyyy-MM-dd,hh:mm:ss"));
    QLabel *lModifiedline = new QLabel(fin.lastModified().toString("yyyy-MM-dd,hh:mm:ss"));

    QVBoxLayout *fInfoName =new QVBoxLayout;
    fInfoName->addWidget(name);
    fInfoName->addWidget(type);
    fInfoName->addWidget(path);
    fInfoName->addWidget(size);
   // fInfoName->addItem(spaceitem1);
    fInfoName->addWidget(creat);
    fInfoName->addWidget(lasttime);
    fInfoName->addWidget(lastmodified);

    QVBoxLayout *fInfoData =new QVBoxLayout;
    fInfoData->addWidget(nameline);
    fInfoData->addWidget(typeline);
    fInfoData->addWidget(pathline);
    fInfoData->addWidget(sizeline);
   // fInfoData->addItem(spaceitem1);
    fInfoData->addWidget(creatline);
    fInfoData->addWidget(lTimeline);
    fInfoData->addWidget(lModifiedline);

    QHBoxLayout *datalayout =new QHBoxLayout;
    datalayout->addLayout(fInfoName);
    datalayout->addLayout(fInfoData);
    finfo->setLayout(datalayout);
    finfo->show();

}

//列表
void MainWindow::listviewSlot()
{
    ui->listView->setViewMode(QListView::ListMode);
    ui->listView->setSpacing(0);
    ui->listView->setIconSize(QSize(26,25));
    ui->listView->setGridSize(QSize(30,27));
    viewPoint = 0;
}

//大图标
void MainWindow::bigiconSlot()
{
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setIconSize(QSize(100,125));
    ui->listView->setGridSize(QSize(100,120));
    ui->listView->setResizeMode(QListView::Adjust);
    viewPoint = 1;
}

//中图标
void MainWindow::middleiconSlot()
{

    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setIconSize(QSize(60,75));
    ui->listView->setGridSize(QSize(100,120));
    ui->listView->setResizeMode(QListView::Adjust);
    viewPoint = 2;
}

//小图标
void MainWindow::smalliconSlot()
{
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setIconSize(QSize(20,25));
    ui->listView->setGridSize(QSize(50,60));
    ui->listView->setResizeMode(QListView::Adjust);
    viewPoint = 3;
}

//详细信息
void MainWindow::listinfoSlot()
{
    if (ui->actionListinfo->isChecked())
    {
        ui->listView->hide();
        ui->tableView->show();
    }
    if (!ui->actionListinfo->isChecked())
    {
        ui->listView->show();
        ui->tableView->hide();
    }
}

void MainWindow::checkView()
{
    switch (viewPoint) {
    case 0:
        listviewSlot();
        break;
    case 1:
        bigiconSlot();
        break;
    case 2:
        middleiconSlot();
        break;
    case 3:
        smalliconSlot();
        break;
    default:
        listviewSlot();
        break;
    }
}


//搜索功能


void MainWindow::searchFileSlot()
{
    QString path = model->filePath(allIndex.at(indexPoint));
    QString searchText = ui->searchLineEdit->text();
    ui->searchLineEdit->setText(QStringLiteral("正在多线程搜索,可以进行其他操作"));
    SearchThread *searchThread1 = new SearchThread(path,searchText);
    connect(searchThread1,SIGNAL(sendData(QStringList)),this,SLOT(receQstring(QStringList)));
    searchThread1->start();
}


void MainWindow::receQstring(QStringList fileStringNew)
{
    if (!fileStringNew.isEmpty())
    {
    fileAndDirName = fileStringNew;
    ui->searchLineEdit->setText(QStringLiteral("搜索完毕，点击图标查看"));
    searchFinishAction = new QAction(QIcon(":/new/icon/image/showSearch.png"),QStringLiteral("搜索完毕,显示"),this);
    ui->searchLineEdit->addAction(searchFinishAction,QLineEdit::TrailingPosition);
    connect(searchFinishAction,SIGNAL(triggered(bool)),this,SLOT(solveSraech()));
    }
    else
        ui->searchLineEdit->setText(QStringLiteral("搜索无此文件，请检查"));
}

void MainWindow::solveSraech()
{
    searchFinishAction->setIcon(QIcon(":/new/icon/image/close.png"));
    searchFinishAction->setText(QStringLiteral("关闭搜索界面"));
    ui->searchLineEdit->setText(QStringLiteral("点击“×”关闭此页面"));
    QStringListModel *sModel = new QStringListModel;
    ui->listView->setModel(sModel);
    ui->listView->setViewMode(QListView::ListMode);
    ui->listView->setStyleSheet("border-top-color:rgb(220,220,220); border-top-width:1px; border-top-style:solid; "
                                "border-right-width:1px; border-right-color:rgb(248,248,248); border-right-style:solid;"
                                "font-size:20px");
    ui->listView->setIconSize(QSize(26,60));
    ui->listView->setGridSize(QSize(26,60));
    sModel->setStringList(fileAndDirName);
    this->setContextMenuPolicy(Qt::NoContextMenu);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    disconnect(searchFinishAction,SIGNAL(triggered(bool)),this,SLOT(solveSraech()));
    connect(searchFinishAction,SIGNAL(triggered(bool)),this,SLOT(clickCloseSlot()));
    disconnect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickSlot(QModelIndex)));
    connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickInQstringList(QModelIndex)));
}

void MainWindow::clickCloseSlot()
{
    refreshView();
    checkView();
    searchFinishAction->setVisible(false);
    ui->searchLineEdit->clear();
    ui->listView->setRootIndex(allIndex.at(indexPoint));
    disconnect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickInQstringList(QModelIndex)));
    connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickSlot(QModelIndex)));
}

void MainWindow::doubleClickInQstringList(QModelIndex a)
{
    clickCloseSlot();
    QString newPath = a.data().toString();
    int i;
    for (i=0; i<newPath.length(); i++)
    {
        if (newPath.at(i)=='\n')
            break;
    }
    newPath=newPath.remove(0,i+1);
    if (QFileInfo(newPath).isDir())
    ui->listView->setRootIndex(model->index(newPath));
    else if(QFileInfo(newPath).isFile())
    {
       QDesktopServices::openUrl(QUrl::fromLocalFile(newPath));
       ui->listView->setCurrentIndex(model->index(newPath));
    }
}
//过滤器
void MainWindow::filterSlot()
{
    QStringList nameFilter;
    if (ui->action_jpg->isChecked())
    {
        nameFilter<<"*.jpg"<<"*.png"<<"*.jpge";
    }
    if (ui->action_mp3->isChecked())
    {
        nameFilter<<"*.mp3"<<"*.wav"<<"*.m4a";
    }
    if (ui->action_rar->isChecked())
    {
        nameFilter<<"*.rar"<<"*.zip";
    }
    if (ui->action_mp4->isChecked())
    {
        nameFilter<<"*.mp4"<<"*.avi"<<"*.rmvb";
    }
    if (ui->action_txt->isChecked())
    {
        nameFilter<<"*.txt"<<"*.word"<<"*.excle"<<"*.ppt";
    }
    model->setNameFilters(nameFilter);
    model->setNameFilterDisables(false);
}

void MainWindow::showHelpFilter()
{
    FilterDailog *filetrDailog = new FilterDailog;
    filetrDailog->show();
}


void MainWindow::on_action_CPU_triggered()
{
    Widget *newwidget = new Widget;
    newwidget->show();

}

void MainWindow::openrask()
{
    Task *newTask =new Task;
    newTask->show();
}

void MainWindow::on_actionAbout_Explorer_triggered()
{
    aboutDialog *newdialog =new aboutDialog;
    newdialog->show();
}
