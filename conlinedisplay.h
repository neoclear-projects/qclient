#ifndef CONLINEDISPLAY_H
#define CONLINEDISPLAY_H

#include <QTextBrowser>
#include <QString>
#include <unordered_set>
#include <mutex>

class COnlineDisplay : public QTextBrowser
{
    Q_OBJECT
private:
    std::unordered_set<QString> onlineUser;
    std::mutex _m;
public:
    COnlineDisplay(QWidget *parent = nullptr);
    void updateDisplay();
    void addUser(QString name);
    void deleteUser(QString name);
};

#endif // CONLINEDISPLAY_H
