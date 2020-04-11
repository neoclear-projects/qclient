#include "conlinedisplay.h"

COnlineDisplay::COnlineDisplay(QWidget *parent)
    : QTextBrowser(parent)
{
    this->setFont(QFont(tr("微软雅黑"), 12));
}

void COnlineDisplay::updateDisplay() {
    _m.lock();
    this->clear();
    for (QString name : onlineUser) {
        this->append(name);
    }
    _m.unlock();
}

void COnlineDisplay::addUser(QString name) {
    _m.lock();
    onlineUser.insert(name);
    _m.unlock();

    updateDisplay();
}

void COnlineDisplay::deleteUser(QString name) {
    _m.lock();
    onlineUser.erase(name);
    _m.unlock();
    updateDisplay();
}
