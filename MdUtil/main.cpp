#include <QCoreApplication>
#include <QDirIterator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <string.h>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <QDate>

int main(int argc, char *argv[])
{

    system("clear");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data_res/md.db");

    if(!db.open()){
        qDebug() << "open database error" << endl;
        return -1;
    }

    QDir res_dir("./data_res/real_data/");
    QDirIterator itr(res_dir);
    int i = 0;
    while(itr.hasNext()){
        QString file_path = itr.next();
        qDebug() << file_path << endl;
        if ( file_path == "./data_res/real_data/." || file_path == "./data_res/real_data/.."){
            qDebug() << "continue" << endl;
            continue;
        }
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
                //  插入的insert 头部分
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
        qDebug() << "插入..... 啊" << i++ << endl;
        db.exec(*sql);
        qDebug() << "完成.." << endl;
        file.close();
        delete sql;

    }

    db.close();

    return 0;
}

