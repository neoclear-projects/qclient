#ifndef DBACCESS_H
#define DBACCESS_H

#include <atomic>
#include <mutex>
#include <QString>
#include <vector>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

class dbAccess
{
private:
    static std::atomic<dbAccess *> pInstance;
    static std::mutex _m;
    QSqlDatabase db;
    dbAccess();
public:
    static dbAccess* getInstance();

    void connect(QString name);
    void close();
    std::vector<std::vector<QString>> query(QString stmt, std::vector<QVariant> args);
    std::vector<QString> queryOne(QString stmt, std::vector<QVariant> args);

};

#endif // DBACCESS_H
