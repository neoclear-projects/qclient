#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QIntValidator>
#include <QCoreApplication>
#include <QMessageBox>
#include "transmit.h"
#include "codec.h"
#include "dbaccess.h"
#include <QString>
#include <algorithm>
#include <QJsonDocument>
#include <QJsonObject>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent),
      inputText(new CTextEdit(this)),
      displayText(new CMessageDisplay(this)),
      onlineDisplay(new COnlineDisplay(this)),
      ui(new Ui::ClientWindow),
      server(new QTcpSocket(this))
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Offline"));
    initLayout();
    initConnect();
    initUI();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::initLayout() {
    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(displayText, 0, 0, 6, 6);
    grid->addWidget(ui->onlineTitle, 0, 6, 1, 2);
    grid->addWidget(inputText, 6, 0, 2, 5);
    grid->addWidget(ui->sendButton, 6, 5, 1, 1);
    grid->addWidget(ui->exitButton, 7, 5, 1, 1);

    grid->addWidget(onlineDisplay, 1, 6, 2, 2);

    grid->addWidget(ui->hostName, 3, 6, 1, 2);
    grid->addWidget(ui->portNumber, 4, 6, 1, 2);
    grid->addWidget(ui->userName, 5, 6, 1, 2);

    grid->addWidget(ui->loadButton, 6, 6, 1, 1);
    grid->addWidget(ui->saveButton, 6, 7, 1, 1);
    grid->addWidget(ui->connectButton, 7, 6, 1, 1);
    grid->addWidget(ui->disconnectButton, 7, 7, 1, 1);

    grid->setRowStretch(1, 2);


    ui->centralwidget->setLayout(grid);
}

void ClientWindow::initConnect() {
    connect(ui->sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
    connect(ui->exitButton, &QPushButton::clicked, this, &ClientWindow::exitProgram);
    connect(ui->connectButton, &QPushButton::clicked, this, &ClientWindow::connectServer);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &ClientWindow::disconnectServer);
    connect(inputText, &CTextEdit::returnPressed, this, &ClientWindow::sendMessage);

    connect(ui->loadButton, &QPushButton::clicked, this, &ClientWindow::loadProfile);
    connect(ui->saveButton, &QPushButton::clicked, this, &ClientWindow::saveProfile);

    connect(server, &QTcpSocket::readyRead, this, &ClientWindow::receiveData);
}

void ClientWindow::initUI() {
    ui->hostName->setPlaceholderText(tr("Enter Server IP"));
    ui->portNumber->setPlaceholderText(tr("Enter Port Number"));
    ui->userName->setPlaceholderText(tr("Enter Your Username"));
    ui->portNumber->setValidator(new QIntValidator(0, 65535, this));

    QString IpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression IpRegex ("^" + IpRange
                                    + "(\\." + IpRange + ")"
                                    + "(\\." + IpRange + ")"
                                    + "(\\." + IpRange + ")$");
    ui->hostName->setValidator(new QRegularExpressionValidator(IpRegex, this));
}

void ClientWindow::exitProgram() {
    QCoreApplication::quit();
}

void ClientWindow::connectServer() {
    if (server->state() == QTcpSocket::ConnectedState)
        return;
    server->connectToHost(ui->hostName->text(),
                          ui->portNumber->text().toInt());
    if (!server->waitForConnected(1000)) {
        QMessageBox::critical(nullptr, tr("Failure"), tr("Connection Timed Out"));
    }

    // Send name
    sendName();
    requestSync();
    requestUserStatus();
}

void ClientWindow::disconnectServer() {
    if (server->state() == QTcpSocket::ConnectedState) {
        this->setWindowTitle(tr("Offline"));
        server->close();
    }
}

void ClientWindow::sendMessage() {
    char *buffer = transmit::gen_buf();

    encode::gen_msg(buffer, inputText->toPlainText().trimmed().toStdString().c_str());
    server->write(buffer, BUF_SIZE);

    inputText->clear();

    transmit::free_buf(buffer);
}

void ClientWindow::sendName() {
    char *buffer = transmit::gen_buf();

    encode::gen_name(buffer, ui->userName->text().toStdString().c_str());
    server->write(buffer, BUF_SIZE);

    this->setWindowTitle(ui->userName->text() + tr("@") +
                         ui->hostName->text() + tr(":") +
                         ui->portNumber->text());

    transmit::free_buf(buffer);
}

void ClientWindow::requestSync() {
    char *buffer = transmit::gen_buf();

    std::vector<QString> last
            = dbAccess::getInstance()->queryOne("select DISTINCT time from Message order by time DESC", {});
    struct datetime dt;
    if (last.empty())
        dt = util::string_to_datetime("0000-00-00 00:00:00");
    else
        dt = util::string_to_datetime(last.front().toStdString());

    encode::gen_sync(buffer, dt);
    qDebug() << util::datetime_to_string(dt).c_str();
    server->write(buffer, BUF_SIZE);

    transmit::free_buf(buffer);
}

void ClientWindow::requestUserStatus() {
    char *buffer = transmit::gen_buf();

    encode::gen_usr_sts(buffer);
    server->write(buffer, BUF_SIZE);

    transmit::free_buf(buffer);
}

void ClientWindow::receiveData() {
    char *buffer = transmit::gen_buf();

    while (server->bytesAvailable()) {
        transmit::clear_buf(buffer);

        server->read(buffer, BUF_SIZE);

        qDebug() << "Response: " << (int)buffer[0];

        switch (*buffer) {
        case 0x0:
            handleMessage(buffer);
            break;
        case 0x3:
            handleSyncMessage(buffer);
            break;
        case 0x7:
            handleSyncEnd();
            break;
        case 0x4:
        case 0x5:
            handleLogonUser(buffer);
            break;
        case 0x6:
            handleLogoffUser(buffer);
            break;
        }
    }
    transmit::free_buf(buffer);
}

void ClientWindow::handleMessage(const char *buffer) {
    char *sender = util::gen_name_buf();
    char *msg = util::gen_msg_buf();
    struct datetime dt;

    decode::decMessage(buffer, sender, &dt, msg);

    // some dummy stuffs
    if (strlen(msg) == 0) {
        util::free_name_buf(sender);
        util::free_msg_buf(msg);
        return;
    }

    // Do some database stuff here

    // print it on the screen
    displayText->addMessage(QString(sender), dt, QString(msg));
//    displayText->append(QString(sender) + tr("(") +
//                            QString(util::datetime_to_string(dt).c_str()) + tr("): ") +
//                            QString(msg) + tr("\n"));
    dbAccess::getInstance()->query("INSERT INTO Message VALUES (?, ?, ?)", {
                                       QString(sender),
                                       QString(msg),
                                       QString(util::datetime_to_string(dt).c_str())
                                   });

    util::free_name_buf(sender);
    util::free_msg_buf(msg);
}

void ClientWindow::handleSyncMessage(const char *buffer) {
    char *sender = util::gen_name_buf();
    char *msg = util::gen_msg_buf();
    struct datetime dt;

    decode::decMessage(buffer, sender, &dt, msg);

    // Do some database stuff here
    dbAccess::getInstance()->query("INSERT INTO Message VALUES (?, ?, ?)", {
                                       QString(sender),
                                       QString(msg),
                                       QString(util::datetime_to_string(dt).c_str())
                                   });

    util::free_name_buf(sender);
    util::free_msg_buf(msg);
}

void ClientWindow::handleSyncEnd() {
    std::vector<std::vector<QString>> result
            = dbAccess::getInstance()->query("SELECT username, content, time FROM Message order by time DESC limit 0, 60", {});
    std::reverse(result.begin(), result.end());
    displayText->clear();
    for (std::vector<QString>& it : result) {
        displayText->addMessage(it[0], util::string_to_datetime(it[2].toStdString()), it[1]);
    }
}

void ClientWindow::printCnt() {
    qDebug() << ">>> " << cnt++ << endl;
}

void ClientWindow::handleLogonUser(const char *buffer) {
    char *name = util::gen_name_buf();

    decode::decUser(buffer, name);
    onlineDisplay->addUser(QString(name));

    util::free_name_buf(name);
}

void ClientWindow::handleLogoffUser(const char *buffer) {
    char *name = util::gen_name_buf();

    decode::decUser(buffer, name);
    onlineDisplay->deleteUser(QString(name));

    util::free_name_buf(name);
}

void ClientWindow::loadProfile() {
    QFile file("profile.json");
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "Failed to load file";
    QJsonObject jObj = QJsonDocument::fromJson(file.readAll()).object();

    ui->hostName->setText(jObj["host"].toString());
    ui->portNumber->setText(QString::number(jObj["port"].toInt()));
    ui->userName->setText(jObj["name"].toString());
    file.close();
}

void ClientWindow::saveProfile() {
    QFile file("profile.json");
    if (!file.open(QIODevice::WriteOnly))
        qDebug() << "Failed to save file";
    QJsonObject jObj;
    jObj.insert("host", ui->hostName->text());
    jObj.insert("port", ui->portNumber->text().toInt());
    jObj.insert("name", ui->userName->text());

    QJsonDocument jDoc;
    jDoc.setObject(jObj);
    file.write(jDoc.toJson());
    file.close();

}
