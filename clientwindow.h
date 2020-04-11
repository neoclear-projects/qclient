#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include<QtNetwork/QTcpSocket>
#include "ctextedit.h"
#include "cmessagedisplay.h"
#include "conlinedisplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

private:
    CTextEdit *inputText;
    CMessageDisplay *displayText;
    COnlineDisplay *onlineDisplay;

private:
    int cnt = 0;
    void printCnt();

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private:
    Ui::ClientWindow *ui;
    QTcpSocket *server;

    void initLayout();
    void initConnect();
    void initUI();

    void connectServer();
    void disconnectServer();
    void exitProgram();

    void sendMessage();
    void sendName();
    void requestSync();
    void requestUserStatus();

    void receiveData();

    void handleMessage(const char *buffer);
    void handleSyncMessage(const char *buffer);
    void handleSyncEnd();
    void handleLogonUser(const char *buffer);
    void handleLogoffUser(const char *buffer);

    void loadProfile();
    void saveProfile();
};
#endif // CLIENTWINDOW_H
