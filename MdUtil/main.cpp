#include <QCoreApplication>
#include <QDirIterator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <string.h>
#include <QString>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QDir>

#include <QDate>
void scanDir(QDir dir, QList<QString> &con_path)
{
    dir.setNameFilters(QStringList("*.csv"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

//    qDebug() << "Scanning: " << dir.path();

    QStringList fileList = dir.entryList();
    for (int i=0; i<fileList.count(); i++)
    {
//        qDebug() << "Found file: " << dir.path() + "/" + fileList[i];
        con_path.push_back(QString(dir.path() + "/" + fileList[i]));

    }

    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();
    for (int i=0; i<dirList.size(); ++i)
    {
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
        scanDir(QDir(newPath), con_path);
    }
}
int main()
{

    system("clear");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data_res/md.db");

    if(!db.open()){
        qDebug() << "open database error" << endl;
        return -1;
    }

    QDir res_dir("./data_res/data");
    QList<QString> data_path;
    scanDir(res_dir, data_path);
    QString file_path;
    QList<QString>::Iterator itr = data_path.begin();
    // 计数
     int count = 0;
    for(; itr != data_path.end(); ++itr) {
//        qDebug() << *itr << endl;
        file_path = *itr;
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "open file error" << endl;
            return -2;
        }

        file.readLine();
        QString *sql = new QString;
        int flag = 0;

        while(!file.atEnd()){
            QByteArray line = file.readLine();
            QStringList l = QString(line).split(',');
            // 如果 l 的大小等于28 的时候继续执行, 如果不等于28的时候,则输出 l.size() == 28;
            Q_ASSERT(l.size() == 28);
            QDateTime stamp = QDateTime::fromString(l.at(2), "yyyy-MM-dd HH:mm:ss");

            if (!flag){

                sql->sprintf("insert into md values(%d,%d,'%.4f')",
                        l.at(1).toInt(),
                        stamp.toTime_t(),
                        l.at(3).toDouble()
                        );
                flag = 1;
                continue;
            }
            // 临时 buffer
            QString part_sql;
            part_sql.sprintf(",(%d,%d,'%.4f')",
                        l.at(1).toInt(),
                        stamp.toTime_t(),
                        l.at(3).toDouble()
                    );
            // 拼接每次的 sql 插入的内容
            sql->append(part_sql);
        }

        // 执行语句
        qDebug() << "插入..... 啊" << count++ << endl;
        db.exec(*sql);
        qDebug() << "完成.." << endl;
        file.close();
        delete sql;

    }
    db.close();

    return 0;
}

