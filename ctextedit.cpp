#include "ctextedit.h"
#include <QDebug>

CTextEdit::CTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    this->setFont(QFont(tr("微软雅黑"), 12));
}

void CTextEdit::keyPressEvent(QKeyEvent *e) {
    QPlainTextEdit::keyPressEvent(e);
    if (e->key() == Qt::Key_Return) {
        emit returnPressed();
    }
}
