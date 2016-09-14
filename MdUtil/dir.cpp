//（一）采用递归和QDir实现文件夹下所有文件遍历的方法

#include <QDir>

bool FindFile(const QString & path)

{

    QDir dir(path);

    if (!dir.exists())

        return false;

    dir.setFilter(QDir::Dirs|QDir::Files);//除了目录或文件，其他的过滤掉

    dir.setSorting(QDir::DirsFirst);//优先显示目录

    QFileInfoList list = dir.entryInfoList();//获取文件信息列表

    int i = 0;

    bool bIsDir;

    do{

        QFileInfo fileInfo = list.at(i);

        if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")

        {

            i++;

            continue;

        }

        bIsDir = fileInfo.isDir();

        if (bIsDir)

        {

            nFiles++;                //对文件属性进行处理

            //fileInfo.size(),fileInfo.fileName(),fileInfo.path()

            FindFile(fileInfo.filePath());

        }

        else

        {

            nFiles++;

        }

        i++;

    }

    while(i<list.size());

}

//（二）QT获取文件夹下所有文件数目及名字

path=QFileDialog::getExistingDirectory(NULL, tr("选择文件夹"),"D:\\",QFileDialog::ShowDirsOnly);
QDir *dir=new QDir(path);
QStringList filter;
//filter<<"*.dat";
//dir->setNameFilters(filter);
QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));

注释掉的两行是用来文件过滤用的，如果你要指定文件后缀名，这样写filter<<"*.dat"  就好了，如果要指定多个后缀名，这样写filter<<"*.dat"<<"*.txt"就行了

                                       文件数目：fileInfo->count();

文件名称：fileInfo->at(i).fileName();

文件路径（包含文件名）：fileInfo->at(i).filePath();

（三）QT遍历目录获取文件信息

QFileInfo
获取文件信息：文件名称，路径，大小，创建时间，修改时间，权限等
使用
路径：
UNIX: /home/dipper/file1
       Windows: C://dipper//file1

       构造函数：
       QFileInfo fileInfo(path);
QFileInfo fileInfo;
fileInfo.setFile(path);

exists(): 判断文件是否存在
          size(): 获取文件大小

  文件类型：
  isFile(), isDir(), isSymLink()

  symLinkTarget(): 返回符号链接的文件

  注意：
  UNIX和Windows（快捷方式）对符号链接处理不一样
  UNIX: size()返回链接的目标文件的大小
  Windows: size()返回链接文件本身的大小

  #ifdef Q_OS_UNIX

  QFileInfo info1("/home/bob/bin/untabify");
info1.isSymLink();          // returns true
info1.absoluteFilePath();   // returns "/home/bob/bin/untabify"
info1.size();               // returns 56201
info1.symLinkTarget();      // returns "/opt/pretty++/bin/untabify"

QFileInfo info2(info1.symLinkTarget());
info1.isSymLink();          // returns false
info1.absoluteFilePath();   // returns "/opt/pretty++/bin/untabify"
info1.size();               // returns 56201

#endif

#ifdef Q_OS_WIN

QFileInfo info1("C://Documents and Settings//Bob//untabify.lnk");
info1.isSymLink();          // returns true
info1.absoluteFilePath();   // returns "C:/Documents and Settings/Bob/untabify.lnk"
info1.size();               // returns 743
info1.symLinkTarget();      // returns "C:/Pretty++/untabify"

QFileInfo info2(info1.symLinkTarget());
info1.isSymLink();          // returns false
info1.absoluteFilePath();   // returns "C:/Pretty++/untabify"
info1.size();               // returns 63942

#endif

path(): 返回文件路径
        fileName(): 返回文件名称

  QFileInfo fileInfo("/home/dipper/xyz.tar.gz");
fileInfo.path(); // returns "/home/dipper"
fileInfo.fileName(); // returns "xyz.tar.gz"
fileInfo.baseName(); // returns "xyz"
fileInfo.completeBaseName(); // returns "xyz.tar"
fileInfo.suffix();   // returns "gz"
fileInfo.completeSuffix(); // returns "tar.gz"

日期相关：
created(): 创建时间
           lastModified(): 最近修改时间
  lastRead(): 最近读时间

  权限：
  isReadable(), isWritable(), isExecutable()

  所有者：
  ower(), owerId(), group(), groupId(), permissions(),
    permission(QFile::Permissions permissions)

  dir():  返回父目录
  下面三个都返回"~/examples/191697"
  QFileInfo fileInfo1("~/examples/191697/.");
QFileInfo fileInfo2("~/examples/191697/..");
QFileInfo fileInfo3("~/examples/191697/main.cpp");

下面三个都返回"."
QFileInfo fileInfo4(".");
QFileInfo fileInfo5("..");
QFileInfo fileInfo6("main.cpp");

相对路径和绝对路径
QString absolute = "/local/bin";
QString relative = "local/bin";
QFileInfo absFile(absolute);
QFileInfo relFile(relative);

QDir::setCurrent(QDir::rootPath());
// absFile and relFile now point to the same file

QDir::setCurrent("/tmp");
// absFile now points to "/local/bin",
// while relFile points to "/tmp/local/bin"

QDir 显示文件系统目录

实例：文件浏览器

//fileviewer.h
#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QtGui>

class FileViewer : public QDialog        // file information dialogs widget
{
    Q_OBJECT
public:
    FileViewer(QWidget *parent=0);
    ~FileViewer();
public:
    QLineEdit* currDir;
    QListWidget* fileListWidget;

public:
    void showFileInfoList(QFileInfoList list);

public slots:
    void showDir(QListWidgetItem * item);
    void showFiles(QDir dir);
    void showFiles();
};

#endif // FILEVIEWER_H

//fileviewer.cpp
#include "fileviewer.h"

FileViewer::FileViewer(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("File Viewer"));

    currDir = new QLineEdit;
    currDir->setText("c://");
    //currDir->setReadOnly(true);

    fileListWidget = new QListWidget;

    QVBoxLayout *vbLayout = new QVBoxLayout(this);
    vbLayout->addWidget(currDir);
    vbLayout->addWidget(fileListWidget);

    connect(currDir, SIGNAL(returnPressed()),
            this, SLOT(showFiles()));

    connect(fileListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(showDir(QListWidgetItem *)));

    QString root = "c://";
    QDir rootDir(root);

    QStringList strings;
    strings << "*" ;
    QFileInfoList list = rootDir.entryInfoList(strings);
    showFileInfoList(list);
}

FileViewer::~FileViewer()
{

}

//显示文件信息列表
void FileViewer::showFileInfoList(QFileInfoList list)
{
    fileListWidget->clear();
    for(int i=0; i<list.count(); i++)
    {
        QFileInfo tmpFileInfo = list.at(i);
        if((tmpFileInfo.isDir()))
        {
            QIcon icon(":/images/dir.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName); //在列表上显示图标和名称
            fileListWidget->addItem(tmp);
        }
        else if(tmpFileInfo.isFile())
        {
            QIcon icon(":/images/file.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem (icon, fileName);
            fileListWidget->addItem(tmp);
        }
    }
}

void FileViewer::showDir(QListWidgetItem * item)
{
    QString str = item->text();
    QDir dir;
    dir.setPath(currDir->text()); //设置目录
    dir.cd(str); //切换到另一个目录
    currDir->setText(dir.absolutePath());
    showFiles(dir);
}

void FileViewer::showFiles(QDir dir)
{
    QStringList strings;
    strings << "*" ;
    QFileInfoList list = dir.entryInfoList(strings,   //文件名称过滤器，"*", "*.tar.gz"
                                           QDir::AllEntries,  //文件属性过滤器，目录，文件，读写
                                           QDir::DirsFirst  //排序方式，名称，修改时间，大小，目录优先
                                           );
    showFileInfoList(list);
}

void FileViewer::showFiles()
{
    QDir dir(currDir->text());
    showFiles(dir);
}

//main.cpp
#include "fileviewer.h"
#include <QApplication>

int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);

    QApplication app(argc, argv);
    QTranslator translator;
    translator.load("fileviewer_zh","..");
    app.installTranslator(&translator);

    FileViewer *fileviewer = new FileViewer;
    fileviewer->show();
    return app.exec();
}

绝对路径
QDir("/home/user/Documents")
QDir("C:/Documents and Settings")

dirName(): 返回目录名称
           QDir("Documents/Letters/Applications").dirName() // "Applications"
  QDir().dirName()                                 // "."

  切换目录：cd(str), cdUp()

  创建，修改，删除：mkdir(), rename(), rmdir()

  获取目录里面的文件，目录等
  count(): 返回条目的数量
  QStringList list = dir.entryList();
QFileInfoList fileInfoList = dir.entryInfoList();

静态函数
Dir        Qstring
current(), currentPath()
home(),    homePath()
root(),    rootPath()
temp(),    tempPath()

QDir dir("example");
if (!dir.exists())
qWarning("Cannot find the example directory");
//使用静态函数：QFile::exists("example");

遍历目录，读写文件

QDir dir = QDir::root();                 // "/"
if (!dir.cd("tmp")) {                    // "/tmp"
    qWarning("Cannot find the /"/tmp/" directory");
} else {
QFile file(dir.filePath("ex1.txt")); // "/tmp/ex1.txt"
if (!file.open(QIODevice::ReadWrite))
qWarning("Cannot create the file %s", file.name());
}

列出当前目录下所有文件，不包括符号链接，按大小排序，升序
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QDir dir;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    std::cout << "     Bytes Filename" << std::endl;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                                .arg(fileInfo.fileName()));
        std::cout << std::endl;
    }
    return 0;
}

（四）获取文件的(完整)后缀名和(完整)前缀名
//获取文件前缀名
QFileInfo fi("/tmp/archive.tar.gz");
QString base= fi.baseName(); //
base = "archive"
        //获取文件的完整前缀名
        QFileInfo fi("/tmp/archive.tar.gz");
QString base= fi.completeBaseName(); //
base = "archive.tar"
        //获取文件后缀名
        QFileInfo
        fi("/tmp/archive.tar.gz");
QString
ext= fi.suffix();
// ext = "gz"
//获取文件的完整后缀名
QFileInfo fi("/tmp/archive.tar.gz");
QString ext= fi.completeSuffix(); //
ext = "tar.gz"


        QFileInfo
        获取文件信息：文件名称，路径，大小，创建时间，修改时间，权限等
        使用
        路径：
        UNIX: /home/dipper/file1
        Windows: C://dipper//file1

        构造函数：
        QFileInfo fileInfo(path);
QFileInfo fileInfo;
fileInfo.setFile(path);

exists(): 判断文件是否存在
          size(): 获取文件大小

  文件类型：
  isFile(), isDir(), isSymLink()

  symLinkTarget(): 返回符号链接的文件

  注意：
  UNIX和Windows（快捷方式）对符号链接处理不一样
  UNIX: size()返回链接的目标文件的大小
  Windows: size()返回链接文件本身的大小

  #ifdef Q_OS_UNIX

  QFileInfo info1("/home/bob/bin/untabify");
info1.isSymLink();          // returns true
info1.absoluteFilePath();   // returns "/home/bob/bin/untabify"
info1.size();               // returns 56201
info1.symLinkTarget();      // returns "/opt/pretty++/bin/untabify"

QFileInfo info2(info1.symLinkTarget());
info1.isSymLink();          // returns false
info1.absoluteFilePath();   // returns "/opt/pretty++/bin/untabify"
info1.size();               // returns 56201

#endif

#ifdef Q_OS_WIN

QFileInfo info1("C://Documents and Settings//Bob//untabify.lnk");
info1.isSymLink();          // returns true
info1.absoluteFilePath();   // returns "C:/Documents and Settings/Bob/untabify.lnk"
info1.size();               // returns 743
info1.symLinkTarget();      // returns "C:/Pretty++/untabify"

QFileInfo info2(info1.symLinkTarget());
info1.isSymLink();          // returns false
info1.absoluteFilePath();   // returns "C:/Pretty++/untabify"
info1.size();               // returns 63942

#endif

path(): 返回文件路径
        fileName(): 返回文件名称

  QFileInfo fileInfo("/home/dipper/xyz.tar.gz");
fileInfo.path(); // returns "/home/dipper"
fileInfo.fileName(); // returns "xyz.tar.gz"
fileInfo.baseName(); // returns "xyz"
fileInfo.completeBaseName(); // returns "xyz.tar"
fileInfo.suffix();   // returns "gz"
fileInfo.completeSuffix(); // returns "tar.gz"

日期相关：
created(): 创建时间
           lastModified(): 最近修改时间
  lastRead(): 最近读时间

  权限：
  isReadable(), isWritable(), isExecutable()

  所有者：
  ower(), owerId(), group(), groupId(), permissions(),
    permission(QFile::Permissions permissions)

  dir():  返回父目录
  下面三个都返回"~/examples/191697"
  QFileInfo fileInfo1("~/examples/191697/.");
QFileInfo fileInfo2("~/examples/191697/..");
QFileInfo fileInfo3("~/examples/191697/main.cpp");

下面三个都返回"."
QFileInfo fileInfo4(".");
QFileInfo fileInfo5("..");
QFileInfo fileInfo6("main.cpp");

相对路径和绝对路径
QString absolute = "/local/bin";
QString relative = "local/bin";
QFileInfo absFile(absolute);
QFileInfo relFile(relative);

QDir::setCurrent(QDir::rootPath());
// absFile and relFile now point to the same file

QDir::setCurrent("/tmp");
// absFile now points to "/local/bin",
// while relFile points to "/tmp/local/bin"

QDir 显示文件系统目录

实例：文件浏览器

//fileviewer.h
#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QtGui>

class FileViewer : public QDialog        // file information dialogs widget
{
    Q_OBJECT
public:
    FileViewer(QWidget *parent=0);
    ~FileViewer();
public:
    QLineEdit* currDir;
    QListWidget* fileListWidget;

public:
    void showFileInfoList(QFileInfoList list);

public slots:
    void showDir(QListWidgetItem * item);
    void showFiles(QDir dir);
    void showFiles();
};

#endif // FILEVIEWER_H

//fileviewer.cpp
#include "fileviewer.h"

FileViewer::FileViewer(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("File Viewer"));

    currDir = new QLineEdit;
    currDir->setText("c://");
    //currDir->setReadOnly(true);

    fileListWidget = new QListWidget;

    QVBoxLayout *vbLayout = new QVBoxLayout(this);
    vbLayout->addWidget(currDir);
    vbLayout->addWidget(fileListWidget);

    connect(currDir, SIGNAL(returnPressed()),
            this, SLOT(showFiles()));

    connect(fileListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this, SLOT(showDir(QListWidgetItem *)));

    QString root = "c://";
    QDir rootDir(root);

    QStringList strings;
    strings << "*" ;
    QFileInfoList list = rootDir.entryInfoList(strings);
    showFileInfoList(list);
}

FileViewer::~FileViewer()
{

}

//显示文件信息列表
void FileViewer::showFileInfoList(QFileInfoList list)
{
    fileListWidget->clear();
    for(int i=0; i<list.count(); i++)
    {
        QFileInfo tmpFileInfo = list.at(i);
        if((tmpFileInfo.isDir()))
        {
            QIcon icon(":/images/dir.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName); //在列表上显示图标和名称
            fileListWidget->addItem(tmp);
        }
        else if(tmpFileInfo.isFile())
        {
            QIcon icon(":/images/file.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem (icon, fileName);
            fileListWidget->addItem(tmp);
        }
    }
}

void FileViewer::showDir(QListWidgetItem * item)
{
    QString str = item->text();
    QDir dir;
    dir.setPath(currDir->text()); //设置目录
    dir.cd(str); //切换到另一个目录
    currDir->setText(dir.absolutePath());
    showFiles(dir);
}

void FileViewer::showFiles(QDir dir)
{
    QStringList strings;
    strings << "*" ;
    QFileInfoList list = dir.entryInfoList(strings,   //文件名称过滤器，"*", "*.tar.gz"
                                           QDir::AllEntries,  //文件属性过滤器，目录，文件，读写
                                           QDir::DirsFirst  //排序方式，名称，修改时间，大小，目录优先
                                           );
    showFileInfoList(list);
}

void FileViewer::showFiles()
{
    QDir dir(currDir->text());
    showFiles(dir);
}

//main.cpp
#include "fileviewer.h"
#include <QApplication>

int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);

    QApplication app(argc, argv);
    QTranslator translator;
    translator.load("fileviewer_zh","..");
    app.installTranslator(&translator);

    FileViewer *fileviewer = new FileViewer;
    fileviewer->show();
    return app.exec();
}

绝对路径
QDir("/home/user/Documents")
QDir("C:/Documents and Settings")

dirName(): 返回目录名称
           QDir("Documents/Letters/Applications").dirName() // "Applications"
  QDir().dirName()                                 // "."

  切换目录：cd(str), cdUp()

  创建，修改，删除：mkdir(), rename(), rmdir()

  获取目录里面的文件，目录等
  count(): 返回条目的数量
  QStringList list = dir.entryList();
QFileInfoList fileInfoList = dir.entryInfoList();

静态函数
Dir        Qstring
current(), currentPath()
home(),    homePath()
root(),    rootPath()
temp(),    tempPath()

QDir dir("example");
if (!dir.exists())
qWarning("Cannot find the example directory");
//使用静态函数：QFile::exists("example");

遍历目录，读写文件

QDir dir = QDir::root();                 // "/"
if (!dir.cd("tmp")) {                    // "/tmp"
    qWarning("Cannot find the /"/tmp/" directory");
} else {
QFile file(dir.filePath("ex1.txt")); // "/tmp/ex1.txt"
if (!file.open(QIODevice::ReadWrite))
qWarning("Cannot create the file %s", file.name());
}

列出当前目录下所有文件，不包括符号链接，按大小排序，升序
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QDir dir;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    std::cout << "     Bytes Filename" << std::endl;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                                .arg(fileInfo.fileName()));
        std::cout << std::endl;
    }
    return 0;
}
