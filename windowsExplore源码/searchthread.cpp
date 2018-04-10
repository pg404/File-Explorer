#include "searchthread.h"

SearchThread::SearchThread()
{
    stopped = false;
}

SearchThread::SearchThread(QString path,QString text)
{
    getPath = path;
    getSearchText = text;
    stopped = false;
}

void SearchThread::run()
{
    while(!stopped)
    {
        //QStringList fileString;

        QDir dir(getPath);
        if (!dir.exists())
        {
            return;
        }
        QStringList nameFilter;
        nameFilter<<"*"+getSearchText.toUtf8()+"*";       //过滤规则getPath
        if (getPath == "C:/" || getPath == "D:/" ||getPath == "E:/" ||getPath == "F:/" ||getPath == "G:/")
        {

            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list =dir.entryInfoList();
            for(int i=0; i<list.count(); i++)
            {
                QFileInfo fileInfo = list.at(i);
                if(fileInfo.fileName().contains(getSearchText,Qt::CaseInsensitive))
                fileString.append(QStringLiteral("文件名及路径:  ")+fileInfo.fileName()+" \n"+fileInfo.absoluteFilePath());

            }
        }
        else if (getPath == ":/" ||getPath.isNull())
        {
            QDir dir1("C:/");
            QDir dir2("D:/");
            QDir dir3("E:/");
            QDir dir4("F:/");
            QDir dir5("G:/");

            dir1.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list1 =dir1.entryInfoList();
            for(int i=0; i<list1.count(); i++)
            {
                QFileInfo fileInfo = list1.at(i);
                if(fileInfo.fileName().contains(getSearchText,Qt::CaseInsensitive))
                fileString.append(QStringLiteral("文件名及路径:  ")+fileInfo.fileName()+" \n"+fileInfo.absoluteFilePath());

            }
            dir2.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list2 =dir2.entryInfoList();
            for(int i=0; i<list2.count(); i++)
            {
                QFileInfo fileInfo = list2.at(i);
                if(fileInfo.fileName().contains(getSearchText,Qt::CaseInsensitive))
                fileString.append(QStringLiteral("文件名及路径:  ")+fileInfo.fileName()+" \n"+fileInfo.absoluteFilePath());

            }
            dir3.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list3 =dir3.entryInfoList();
            for(int i=0; i<list3.count(); i++)
            {
                QFileInfo fileInfo = list3.at(i);
                if(fileInfo.fileName().contains(getSearchText,Qt::CaseInsensitive))
                fileString.append(QStringLiteral("文件名及路径:  ")+fileInfo.fileName()+" \n"+fileInfo.absoluteFilePath());

            }
            dir4.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list4 =dir4.entryInfoList();
            for(int i=0; i<list4.count(); i++)
            {
                QFileInfo fileInfo = list4.at(i);
                if(fileInfo.fileName().contains(getSearchText,Qt::CaseInsensitive))
                fileString.append(QStringLiteral("文件名及路径:  ")+fileInfo.fileName()+" \n"+fileInfo.absoluteFilePath());

            }
            dir5.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list5 =dir5.entryInfoList();
            for(int i=0; i<list5.count(); i++)
            {
                QFileInfo fileInfo = list5.at(i);
                if(fileInfo.fileName().contains(getSearchText,Qt::CaseInsensitive))
                fileString.append(QStringLiteral("文件名及路径:  ")+fileInfo.fileName()+" \n"+fileInfo.absoluteFilePath());
            }

        }
        if (getPath == ":/" ||getPath.isNull())
        {

               QDirIterator iter1 ("C:/",nameFilter,QDir::Files|QDir::Dirs,QDirIterator::Subdirectories);
               while (iter1.hasNext())
                {

                    QFileInfo info=iter1.fileInfo();
                    if (info.isDir())
                    {
                    fileString.prepend(QStringLiteral("文件夹名:  ")+"\n"+info.absoluteFilePath());
                    }
                    else if(info.isFile())
                    {
                    fileString.append(QStringLiteral("文件名及路径:  ")+info.fileName()+" \n"+info.absoluteFilePath());
                    }
                    iter1.next();
                }
               QDirIterator iter2 ("D:/",nameFilter,QDir::Files|QDir::Dirs,QDirIterator::Subdirectories);
               while (iter2.hasNext())
                {

                    QFileInfo info=iter2.fileInfo();
                    if (info.isDir())
                    {
                    fileString.prepend(QStringLiteral("文件夹名:  ")+"\n"+info.absoluteFilePath());
                    }
                    else if(info.isFile())
                    {
                    fileString.append(QStringLiteral("文件名及路径:  ")+info.fileName()+" \n"+info.absoluteFilePath());
                    }
                    iter2.next();
                }
               QDirIterator iter3 ("E:/",nameFilter,QDir::Files|QDir::Dirs,QDirIterator::Subdirectories);
               while (iter3.hasNext())
                {

                    QFileInfo info=iter3.fileInfo();
                    if (info.isDir())
                    {
                    fileString.prepend(QStringLiteral("文件夹名:  ")+"\n"+info.absoluteFilePath());
                    }
                    else if(info.isFile())
                    {
                    fileString.append(QStringLiteral("文件名及路径:  ")+info.fileName()+" \n"+info.absoluteFilePath());
                    }
                    iter3.next();
                }
               QDirIterator iter4 ("F:/",nameFilter,QDir::Files|QDir::Dirs,QDirIterator::Subdirectories);
               while (iter4.hasNext())
                {

                    QFileInfo info=iter4.fileInfo();
                    if (info.isDir())
                    {
                    fileString.prepend(QStringLiteral("文件夹名:  ")+"\n"+info.absoluteFilePath());
                    }
                    else if(info.isFile())
                    {
                    fileString.append(QStringLiteral("文件名及路径:  ")+info.fileName()+" \n"+info.absoluteFilePath());
                    }
                    iter4.next();
                }
               QDirIterator iter5 ("G:/",nameFilter,QDir::Files|QDir::Dirs,QDirIterator::Subdirectories);
               while (iter5.hasNext())
                {

                    QFileInfo info=iter5.fileInfo();
                    if (info.isDir())
                    {
                    fileString.prepend(QStringLiteral("文件夹名:  ")+"\n"+info.absoluteFilePath());
                    }
                    else if(info.isFile())
                    {
                    fileString.append(QStringLiteral("文件名及路径:  ")+info.fileName()+" \n"+info.absoluteFilePath());
                    }
                    iter5.next();
                }

        }
        else
        {
                QDirIterator iter (getPath,nameFilter,QDir::Files|QDir::Dirs,QDirIterator::Subdirectories);
                while (iter.hasNext())
                {

                QFileInfo info=iter.fileInfo();
                if (info.isDir())
                {
                    fileString.prepend(QStringLiteral("文件夹名:  ")+"\n"+info.absoluteFilePath());
                }
                else if(info.isFile())
                {
                    fileString.append(QStringLiteral("文件名及路径:  ")+info.fileName()+" \n"+info.absoluteFilePath());
                }
                iter.next();
                }
        }
        //得到QStringList,传递过去
        emit sendData(fileString);
        stopped = true;
    }
}

void SearchThread::stop()
{
    stopped = true;
}

