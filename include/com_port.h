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
#ifndef COM_PORT_H
#define COM_PORT_H

#include <QObject>
#include <QVariant>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QTimer>

#if defined (QT_GUI_LIB)
#    include <QComboBox>
#endif

#if !defined (WITHOUT_LOG)
#    include "Log"
#endif

namespace nayk { //=============================================================

//==============================================================================
class ComPort : public QObject
{
    Q_OBJECT

public:
#if defined (QT_GUI_LIB)
    enum PortProperty {
        PortName = 0,
        PortBaudRate,
        PortDataBits,
        PortStopBits,
        PortParity,
        PortFlowControl
    };
    Q_ENUM(PortProperty)
#endif
    explicit ComPort(QObject *parent = nullptr);
    virtual ~ComPort();
    QString lastError() const;
    bool setPortName(const QString &portName);
    bool setBaudRate(QSerialPort::BaudRate baudRate);
    bool setDataBits(QSerialPort::DataBits dataBits);
    bool setStopBits(QSerialPort::StopBits stopBits);
    bool setParity(QSerialPort::Parity parity);
    bool setFlowControl(QSerialPort::FlowControl flowControl);
    bool open(bool readOnly = false);
    void close();
    bool isOpen() const;
    bool isReady();
    qint64 write(const QByteArray &bytes);
    QByteArray read(qint64 count = -1);
    QByteArray readBuffer() const;
    qint64 bufferSize() const;
    void setBufferSize(const qint64 &bufferSize);
    bool autoRead() const;
    void setAutoRead(bool autoRead);
    bool isRts();
    bool isDtr();
    bool isCts();
    bool isDsr();
    void clearAllBuffers();
    void clearReadBuffer();
    void clearWriteBuffer();
    QString portSettings();

#if defined (QT_GUI_LIB)
    static void fillComboBoxPortProperty(QComboBox *comboBox, PortProperty portProperty,
                                         const QVariant &defaultValue = QVariant());
#endif
    static QString baudRateToStr(QSerialPort::BaudRate baudRate);
    static QString dataBitsToStr(QSerialPort::DataBits dataBits);
    static QString stopBitsToStr(QSerialPort::StopBits stopBits);
    static QString parityToStr(QSerialPort::Parity parity);
    static QString flowControlToStr(QSerialPort::FlowControl flowControl);
    static QSerialPort::BaudRate strToBaudRate(const QString &value);
    static QSerialPort::DataBits strToDataBits(const QString &value);
    static QSerialPort::StopBits strToStopBits(const QString &value);
    static QSerialPort::Parity strToParity(const QString &value);
    static QSerialPort::FlowControl strToFlowControl(const QString &value);

signals:
#if !defined (WITHOUT_LOG)
    void toLog(const QString &text, Log::LogType logType = Log::LogInfo);
#endif
    void portError();
    void portOpen();
    void portClose();
    void bytesRead(qint64 count);
    void bytesWrite(qint64 count);
    void rts(bool on);
    void dtr(bool on);
    void cts(bool on);
    void dsr(bool on);
    void readyRead();

private:
    bool m_busy {true};
    bool m_cts {false};
    bool m_dsr {false};
    QSerialPort m_port;
    QString m_lastError {""};
    bool m_autoRead {true};
    QByteArray m_buffer;
    QTimer m_timer;

private slots:
    void serialPort_error(QSerialPort::SerialPortError error);
    void serialPort_requestToSendChanged(bool set);
    void serialPort_dataTerminalReadyChanged(bool set);
    void serialPort_readyRead();
    void serialPort_bytesWriten(qint64 count);
    void on_timerTimeOut();
};
//==============================================================================

} // namespace nayk //==========================================================
#endif // COM_PORT_H
