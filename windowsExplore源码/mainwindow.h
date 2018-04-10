#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QDirModel>
#include <QSplitter>
#include <QPalette>
#include <QDir>
#include <QVector>
#include <QtGui>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QModelIndex>
#include <QLineEdit>
#include <QPushButton>
#include <QFileInfo>
#include <QLabel>
#include <QFileDialog>
#include <QThread>

namespace Ui
{
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QModelIndex indexNow;  //记录上一级文件夹
    QVector<QModelIndex> allIndex; //记录浏览记录
    QDirModel *q;
    int indexPoint; //用于记录打开的文件夹记录
    int viewPoint;  //表示为 什么视图
    QStringList fileAndDirName;  //记录搜索的变量
    QAction *searchFinishAction; //后面搜索按钮要加入
    void contextMenuEvent(QContextMenuEvent *event);
    void juiceButton();

    //void setButtonIcon();//按钮图标编辑函数


    //8.19
    int findAllFileAndDir(const QString &dirPath);

    //8.18添加
    QModelIndex DirIndex;
    QModelIndex selectFileIndex;
    QList<QModelIndex>PathIdex;
    QModelIndex copyIdex;
    bool isCopy;
    QString copyPath;
    QString copyFileName;
    QString copyPostfixName;
    QString newPath;

    bool isCopyFile;
    bool isDelete;

    bool findSameFile(QString filename,QString folderDir);//查找同名文件
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir);
    bool removeFolderContent(QFileInfo fileList);

private slots://槽函数

    //前进，后退，上移，搜索，更新槽函数
    void goButtonSlot();
    void goBackButtonSlot();
    void backButtonSlot();
    void updateSlot();
    void doubleClickSlot(QModelIndex);
    void searchFileSlot();
    void checkView();

    //8.18添加
    void getPath(const QModelIndex &index);

    //鼠标右键槽函数
    void newTxtFile(void);
    void newExelFile(void);
    void newWordFile(void);
    void newDFile(void);
    void filedeleteslot();
    void filecopyslot();
    void filepasteslot();
    void filerenameslot();
    void fileopenslot();
    void filedetailslot();

    //编辑按钮下槽函数
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionRename_triggered();
    void on_actionDelete_triggered();

    //文件按钮下槽函数
    void on_actionOpen_O_triggered();
    void on_actionTxt_triggered();
    void on_actionWord_triggered();
    void on_actionExel_triggered();

    //计算机按钮下槽函数
    void on_action_shuxing_triggered();

    //查看按钮下槽函数
    void listviewSlot();
    void bigiconSlot();
    void middleiconSlot();
    void smalliconSlot();
    void listinfoSlot();


    //刷新视图
    void refreshView();

    //接受thread的数据 8.24
    void receQstring(QStringList);
    void solveSraech();
    void doubleClickInQstringList(QModelIndex);
    void clickCloseSlot();
    //过滤器功能
    void filterSlot();
    void showHelpFilter();



    //8.26
     void on_action_CPU_triggered();
     void openrask();

     void on_actionAbout_Explorer_triggered();
};

#endif // MAINWINDOW_H
