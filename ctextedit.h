#ifndef CTEXTEDIT_H
#define CTEXTEDIT_H

#include <QPlainTextEdit>

class CTextEdit : public QPlainTextEdit
{
    Q_OBJECT
signals:
    void returnPressed();

protected:
    void keyPressEvent(QKeyEvent *e) override;

public:
    CTextEdit(QWidget *parent = nullptr);
};

#endif // CTEXTEDIT_H
