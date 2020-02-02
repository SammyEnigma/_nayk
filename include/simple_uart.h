#ifndef SIMPLE_UART_H
#define SIMPLE_UART_H

#include <QObject>
#include <QString>
#include <QStringList>

#if defined (QT_GUI_LIB)
#    include <QComboBox>
#endif

#if defined (Q_OS_WIN32)
#   include <windows.h>
#endif

#include "abstract_port.h"

namespace nayk { //=============================================================

//==============================================================================
class SimpleUart: public AbstractPort
{
    Q_OBJECT

public:
    explicit SimpleUart(QObject *parent = nullptr);
    explicit SimpleUart(const QString &portName, QObject *parent = nullptr);
    virtual ~SimpleUart();
    bool setPortName(const QString &portName);
    bool setBaudRate(qint32 baudRate);
    QString portName() const;
    qint32 baudRate() const;
    bool open(bool readOnly = false) override;
    void close() override;
    bool isOpen() const override;
    bool isReady() override;
    qint64 write(const QByteArray &bytes) override;
    qint64 write(const char *bytes, qint64 bytesCount) override;
    int write(char byte);
    QByteArray read(qint64 count = -1) override;
    qint64 read(char *bytes, qint64 count) override;
    QString portSettings();
    static QStringList availablePorts();
    static QStringList availableBaudRates();
#if defined (QT_GUI_LIB)
    static void fillComboBoxPortName(QComboBox *comboBox, const QVariant &defaultValue = QVariant());
#endif

private:
    QString m_portName;
    qint32 m_baudRate;

#if defined (Q_OS_WIN32)
    HANDLE m_handle {INVALID_HANDLE_VALUE};
#else
    int m_handle {-1};
#endif

};
//==============================================================================

} // namespace nayk //==========================================================
#endif // SIMPLE_UART_H
