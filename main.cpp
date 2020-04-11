#include "clientwindow.h"

#include <QApplication>
#include "dbaccess.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dbAccess::getInstance()->connect(QObject::tr("client.db"));
    dbAccess::getInstance()->query(QObject::tr("CREATE TABLE IF NOT EXISTS Message ("
                                               "username    TEXT,"
                                               "content     TEXT,"
                                               "time        TEXT,"
                                               "UNIQUE(username, content, time)"
                                               ")"), {});
    ClientWindow w;
    w.show();
    return a.exec();
}
