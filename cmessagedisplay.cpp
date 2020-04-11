#include "cmessagedisplay.h"

CMessageDisplay::CMessageDisplay(QWidget *parent)
    : QTextBrowser(parent)
{
    this->setFont(QFont(tr("微软雅黑"), 12));
}

void CMessageDisplay::addMessage(QString sender, struct datetime dt, QString msg) {
    this->append(QString(sender) + tr("(") +
                 QString(util::datetime_to_string(dt).c_str()) + tr("): ") +
                 QString(msg) + tr("\n"));
}
