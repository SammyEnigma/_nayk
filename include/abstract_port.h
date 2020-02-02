#ifndef ABSTRACT_PORT_H
#define ABSTRACT_PORT_H

#include <QObject>
#if !defined (WITHOUT_LOG)
#    include "Log"
#endif

namespace nayk { //=============================================================

//==============================================================================
class AbstractPort : public QObject
{
    Q_OBJECT

public:
    explicit AbstractPort(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~AbstractPort() {}
    virtual bool open(bool readOnly = false) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual bool isReady() { return true; }
    virtual qint64 write(const QByteArray &bytes) = 0;
    virtual qint64 write(const char *bytes, qint64 bytesCount) = 0;
    QString lastError() const { return m_lastError; }
    virtual QByteArray read(qint64 count = -1) = 0;
    virtual qint64 read(char *bytes, qint64 count) = 0;
    QByteArray readBuffer() const { return m_buffer; }
    bool autoRead() const { return m_autoRead; }
    void setAutoRead(bool autoRead) { m_autoRead = autoRead; }

signals:
#if !defined (WITHOUT_LOG)
    void toLog(const QString &text, nayk::Log::LogType logType = nayk::Log::LogInfo);
#endif
    void portError();
    void portOpen();
    void portClose();
    void bytesRead(qint64 count);
    void bytesWrite(qint64 count);
    void readyRead();
    void readyChange(bool ready);

protected:
    bool m_autoRead {true};
    QString m_lastError {""};
    QByteArray m_buffer;
};
//==============================================================================

} // namespace //===============================================================
#endif // ABSTRACT_PORT_H
