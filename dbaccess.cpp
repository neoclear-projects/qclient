#include "dbaccess.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QVariant>

std::atomic<dbAccess *> dbAccess::pInstance{ nullptr };
std::mutex dbAccess::_m;

dbAccess::dbAccess()
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
}

dbAccess* dbAccess::getInstance() {
    if (pInstance == nullptr) {
        _m.lock();
        if (pInstance == nullptr)
            pInstance = new dbAccess();
        _m.unlock();
    }
    return pInstance;
}

void dbAccess::connect(QString name) {
    db.setDatabaseName(name);
    db.open();
}

void dbAccess::close() {
    db.close();
}

std::vector<std::vector<QString>> dbAccess::query(QString stmt, std::vector<QVariant> args) {
    std::vector<std::vector<QString>> result;
    QSqlQuery q;
    q.prepare(stmt);
    for (QVariant& it : args)
        q.addBindValue(it);

    q.exec();
    while (q.next()) {
        result.push_back({});
        for (int i = 0; i < q.record().count(); i++)
            result.back().push_back(q.value(i).toString());
    }
    return result;
}

std::vector<QString> dbAccess::queryOne(QString stmt, std::vector<QVariant> args) {
    std::vector<std::vector<QString>> ans = query(stmt, args);
    if (ans.empty())
        return {};
    return ans[0];
}
