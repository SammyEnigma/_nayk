#ifndef LPT_PORT_H
#define LPT_PORT_H

#include <QObject>
#include <QStringList>

#if defined (QT_GUI_LIB)
#    include <QComboBox>
#endif

#include "AbstractPort"

#ifdef Q_OS_WIN32
#   include <qt_windows.h>
#else
#   include <QFile>
#endif

namespace nayk { //=============================================================

//==============================================================================
class LptPort : public AbstractPort
{
    Q_OBJECT

public:
    explicit LptPort(QObject *parent = nullptr);
    virtual ~LptPort();
    bool open(bool readOnly = false) override;
    void close() override;
    bool isOpen() const override;
    qint64 write(const QByteArray &bytes) override;
    QByteArray read(qint64 count = -1) override;
    QString portName() const;
    void setPortName(const QString &portName);
    static QStringList availablePorts();
#if defined (QT_GUI_LIB)
    static void fillComboBoxAvailablePorts(QComboBox *comboBox, const QVariant &defaultValue = QVariant());
#endif

private:
    QString m_portName {""};
#ifdef Q_OS_WIN32
    HANDLE m_handle {INVALID_HANDLE_VALUE};
#else
    QFile m_deviceFile;
#endif

private slots:
    void port_error();
    void port_bytesWriten(qint64 count);
};
//==============================================================================

} // namespace //===============================================================
#endif // LPT_PORT_H
