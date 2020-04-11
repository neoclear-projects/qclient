#ifndef CMESSAGEDISPLAY_H
#define CMESSAGEDISPLAY_H

#include <QTextBrowser>
#include "util.h"

class CMessageDisplay : public QTextBrowser
{
    Q_OBJECT
public:
    CMessageDisplay(QWidget *parent = nullptr);

    void addMessage(QString sender, struct datetime dt, QString msg);
};

#endif // CMESSAGEDISPLAY_H
