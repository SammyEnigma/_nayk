/****************************************************************************
** Copyright (c) 2019 Evgeny Teterin (nayk) <sutcedortal@gmail.com>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include "NaykGlobal"
#include <QObject>
#include <QtNetwork/QNetworkAddressEntry>

namespace network_client { //===================================================

typedef struct TcpIp4Struct {
    bool dhcp {false};
    bool autoDns {false};
    int gatewayMetric {0};
    int ifaceMetric {0};
    QString name {""};
    QString addr {""};
    QString mask {""};
    QString broadcast {""};
    QString gateway {""};
    QString dns1 {""};
    QString dns2 {""};
} TcpIp4Struct;

NAYK_EXPORT bool simpleGet(const QString &url, QString &resultString);
NAYK_EXPORT bool externalIP(QString &ip);
NAYK_EXPORT QString hostAddress();
NAYK_EXPORT QString connectionName(const QString &ip);
NAYK_EXPORT bool addressEntry(const QString &ip, QNetworkAddressEntry &addressEntry);
NAYK_EXPORT bool isCorrectIp(const QString &ip);
NAYK_EXPORT bool hostSettings(TcpIp4Struct &host, QString &err);
NAYK_EXPORT bool setHostSettings(const TcpIp4Struct &host, QString &err);
NAYK_EXPORT QString localHostName();

}; // namespace //==============================================================
#endif // NETWORK_CLIENT_H
