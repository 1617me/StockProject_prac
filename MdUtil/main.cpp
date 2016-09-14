#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QFile>


int main(int argc, char *argv[])
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("md.db");

    if(!db.open()){
        qDebug() << "open database error" << endl;
        return -1;
    }

    QFile file("");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "open file error" << endl;
        return -2;
    }

    file.readLine();

    char *sql = new char[5120];
    int flag = 0;
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        QStringList l = QString(line).split(',');
        // 如果 l 的大小等于28 的时候继续执行, 如果不等于28的时候,则输出 l.size() == 28;
        Q_ASSERT(l.size() == 28);
        QDateTime stamp = QDateTime::fromString(l.at(2), "yyyy-MM-dd HH:mm:ss");

        if (!flag){
            //  插入的insert 头部分
            sprintf(sql, "insert into md values(%d,%d,'%.4f')",
                    l.at(1).toInt(),
                    stamp.toTime_t(),
                    l.at(3).toDouble()
                    );
            flag = 1;
            continue;
        }
        // 临时 buffer
        char tmp_sql[50];
        sprintf(tmp_sql, ",(%d,%d,'%.4f')",
                    l.at(1).toInt(),
                    stamp.toTime_t(),
                    l.at(3).toDouble()
                );
        // 拼接每次的 sql 插入的内容
        strcat(sql, tem_sql);
    }

    db.exec(sql);
    delete []sql;
    db.close();
    file.close();



    return 0;
}

