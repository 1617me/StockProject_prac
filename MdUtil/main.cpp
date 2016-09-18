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

/* 打开数据库 Database */
bool openDB(const QString &driver, const QString &path, QSqlDatabase &db){
    db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(path);
    return db.open();
}

/* merge strings */
int merge_sql_string(QString &file_path, QString *sql){
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "open file error" << endl;
        return -2;
    }

    file.readLine();
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
    sql-
    file.close();
    return 0;
}

/* 1. 导入数据库 */
void import_database(){
    /* 打开数据库 */
    QSqlDatabase db;
    if (!openDB("QSQLITE", "data_res/md.db", db)){
        printf("openDB error\n");
        return -1;
    }

    /* 遍历数据资源目录 */
    QDir res_dir("./data_res/data");
    QList<QString> data_path;
    /* 将遍历过的数据文件路径保存到 data_path 中 */
    scanDir(res_dir, data_path);
    QString file_path;
    QList<QString>::Iterator itr = data_path.begin();

    int count = 0;
    /* 使用迭代器遍历数据路径 list 拼接字符串sql 语句,并执行 */
    for(; itr != data_path.end(); ++itr) {
        file_path = *itr;

        QString *sql = new QString;
        merge_sql_string(file_path, sql);
        // 执行语句
        qDebug() << "插入....." << count++ << endl;
        db.exec(*sql);
        qDebug() << "插入 OK.." << endl;
        delete sql;

    }
    db.close();
}

int main()
{
    system("clear");

    import_database();

    return 0;
}

