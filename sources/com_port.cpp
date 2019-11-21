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
#include <QtSerialPort/QSerialPortInfo>
#include <QMetaEnum>

#include "com_port.h"

namespace nayk { //=============================================================

#if defined (QT_GUI_LIB)
//------------------------------------------------------------------------------
void fillComboBoxPortName(QComboBox *comboBox, const QString &defaultValue)
{
    int index = 0;
    for(int i=0; i<QSerialPortInfo::availablePorts().size(); ++i) {

        QString name = QSerialPortInfo::availablePorts().at(i).portName();
        comboBox->addItem( name, name );
        if(defaultValue == name) index = comboBox->count()-1;
    }
    if(index < comboBox->count()) comboBox->setCurrentIndex(index);
}
//------------------------------------------------------------------------------
void fillComboBoxBaudRate(QComboBox *comboBox, QSerialPort::BaudRate defaultValue)
{
    int index = 0;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::BaudRate>();

    for(int i=0; i<metaEnum.keyCount(); ++i) {

        QSerialPort::BaudRate value = static_cast<QSerialPort::BaudRate>( metaEnum.value(i) );
        if(value != QSerialPort::UnknownBaud) {
            comboBox->addItem( ComPort::baudRateToStr(value), value );
            if(value == defaultValue) index = comboBox->count()-1;
        }
    }

    if(index < comboBox->count()) comboBox->setCurrentIndex(index);
}
//------------------------------------------------------------------------------
void fillComboBoxDataBits(QComboBox *comboBox, QSerialPort::DataBits defaultValue)
{
    int index = 0;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();

    for(int i=0; i<metaEnum.keyCount(); ++i) {

        QSerialPort::DataBits value = static_cast<QSerialPort::DataBits>( metaEnum.value(i) );
        if(value != QSerialPort::UnknownDataBits) {
            comboBox->addItem( ComPort::dataBitsToStr(value), value );
            if(value == defaultValue) index = comboBox->count()-1;
        }
    }

    if(index < comboBox->count()) comboBox->setCurrentIndex(index);
}
//------------------------------------------------------------------------------
void fillComboBoxStopBits(QComboBox *comboBox, QSerialPort::StopBits defaultValue)
{
    int index = 0;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();

    for(int i=0; i<metaEnum.keyCount(); ++i) {

        QSerialPort::StopBits value = static_cast<QSerialPort::StopBits>( metaEnum.value(i) );
        if(value != QSerialPort::UnknownStopBits) {
            comboBox->addItem( ComPort::stopBitsToStr(value), value );
            if(value == defaultValue) index = comboBox->count()-1;
        }
    }

    if(index < comboBox->count()) comboBox->setCurrentIndex(index);
}
//------------------------------------------------------------------------------
void fillComboBoxParity(QComboBox *comboBox, QSerialPort::Parity defaultValue)
{
    int index = 0;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();

    for(int i=0; i<metaEnum.keyCount(); ++i) {

        QSerialPort::Parity value = static_cast<QSerialPort::Parity>( metaEnum.value(i) );
        if(value != QSerialPort::UnknownParity) {
            comboBox->addItem( ComPort::parityToStr(value), value );
            if(value == defaultValue) index = comboBox->count()-1;
        }
    }

    if(index < comboBox->count()) comboBox->setCurrentIndex(index);
}
//------------------------------------------------------------------------------
void fillComboBoxFlowControl(QComboBox *comboBox, QSerialPort::FlowControl defaultValue)
{
    int index = 0;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::FlowControl>();

    for(int i=0; i<metaEnum.keyCount(); ++i) {

        QSerialPort::FlowControl value = static_cast<QSerialPort::FlowControl>( metaEnum.value(i) );
        if(value != QSerialPort::UnknownFlowControl) {
            comboBox->addItem( ComPort::flowControlToStr(value), value );
            if(value == defaultValue) index = comboBox->count()-1;
        }
    }

    if(index < comboBox->count()) comboBox->setCurrentIndex(index);
}
//------------------------------------------------------------------------------
#endif
//==============================================================================
ComPort::ComPort(QObject *parent) : QObject(parent)
{
    connect(&serialPort, &QSerialPort::errorOccurred,
            this, &ComPort::serialPort_errorOccurred);
    connect(&serialPort, &QSerialPort::requestToSendChanged,
            this, &ComPort::serialPort_requestToSendChanged);
    connect(&serialPort, &QSerialPort::dataTerminalReadyChanged,
            this, &ComPort::serialPort_dataTerminalReadyChanged);
    connect(&serialPort, &QSerialPort::readyRead,
            this, &ComPort::serialPort_readyRead);
}
//==============================================================================
ComPort::~ComPort()
{
    close();
}
//==============================================================================
QString ComPort::lastError() const
{
    return m_lastError;
}
//==============================================================================
void ComPort::setPortName(const QString &portName)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set port name: %2")
                .arg(serialPort.portName())
                .arg(portName), Log::LogDbg );
#endif

    serialPort.setPortName(portName);
}
//==============================================================================
bool ComPort::setBaudRate(QSerialPort::BaudRate baudRate)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set BaudRate: %2")
                .arg(serialPort.portName())
                .arg(baudRateToStr(baudRate)), Log::LogDbg );
#endif

    if (serialPort.setBaudRate(baudRate)) return true;

    m_lastError = tr("%1: Failed to set BaudRate: %2")
            .arg(serialPort.portName())
            .arg(baudRateToStr(baudRate));

#if !defined (WITHOUT_LOG)
    emit toLog( m_lastError, Log::LogError);
#endif

    return false;
}
//==============================================================================
bool ComPort::setDataBits(QSerialPort::DataBits dataBits)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set DataBits: %2")
                .arg(serialPort.portName())
                .arg(dataBitsToStr(dataBits)), Log::LogDbg );
#endif

    if (serialPort.setDataBits(dataBits)) return true;

    m_lastError = tr("%1: Failed to set DataBits: %2")
            .arg(serialPort.portName())
            .arg(dataBitsToStr(dataBits));

#if !defined (WITHOUT_LOG)
    emit toLog( m_lastError, Log::LogError);
#endif

    return false;
}
//==============================================================================
bool ComPort::setStopBits(QSerialPort::StopBits stopBits)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set StopBits: %2")
                .arg(serialPort.portName())
                .arg(stopBitsToStr(stopBits)), Log::LogDbg );
#endif

    if (serialPort.setStopBits(stopBits)) return true;

    m_lastError = tr("%1: Failed to set StopBits: %2")
            .arg(serialPort.portName())
            .arg(stopBitsToStr(stopBits));

#if !defined (WITHOUT_LOG)
    emit toLog( m_lastError, Log::LogError);
#endif

    return false;
}
//==============================================================================
bool ComPort::setParity(QSerialPort::Parity parity)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set Parity: %2")
                .arg(serialPort.portName())
                .arg(parityToStr(parity)), Log::LogDbg );
#endif

    if (serialPort.setParity(parity)) return true;

    m_lastError = tr("%1: Failed to set Parity: %2")
            .arg(serialPort.portName())
            .arg(parityToStr(parity));

#if !defined (WITHOUT_LOG)
    emit toLog( m_lastError, Log::LogError);
#endif

    return false;
}
//==============================================================================
bool ComPort::setFlowControl(QSerialPort::FlowControl flowControl)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set FlowControl: %2")
                .arg(serialPort.portName())
                .arg(flowControlToStr(flowControl)), Log::LogDbg );
#endif

    if (serialPort.setFlowControl(flowControl)) return true;

    m_lastError = tr("%1: Failed to set FlowControl: %2")
            .arg(serialPort.portName())
            .arg(flowControlToStr(flowControl));

#if !defined (WITHOUT_LOG)
    emit toLog( m_lastError, Log::LogError);
#endif

    return false;
}
//==============================================================================
bool ComPort::open(bool readOnly)
{
    if (serialPort.isOpen()) return true;

    emit beforeOpen();

    if (serialPort.open( readOnly ? QIODevice::ReadOnly : QIODevice::ReadWrite)) {

        serialPort.setReadBufferSize(m_bufferSize);
        m_buffer.clear();
        serialPort.clear();
        m_ready = serialPort.flowControl() == QSerialPort::HardwareControl
                ? serialPort.isRequestToSend()
                : true;

#if !defined (WITHOUT_LOG)
        emit toLog( tr("%1: Port is open").arg(serialPort.portName()), Log::LogInfo );
#endif
        emit afterOpen();

        return true;
    }
    else {
        m_lastError = tr("%1: Failed to open port").arg(serialPort.portName());

#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        return false;
    }
}
//==============================================================================
void ComPort::close()
{
    if(!serialPort.isOpen()) return;

    emit beforeClose();

    serialPort.close();
    m_ready = false;

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Port is closed").arg(serialPort.portName()), Log::LogInfo );
#endif

    emit afterClose();
}
//==============================================================================
bool ComPort::isOpen() const
{
    return serialPort.isOpen();
}
//==============================================================================
bool ComPort::isReady()
{
    if(!serialPort.isOpen()) return false;

    if((serialPort.flowControl() == QSerialPort::HardwareControl)
            && (serialPort.isRequestToSend() != m_ready)) {

        m_ready = serialPort.isRequestToSend();
        emit readyChange(m_ready);
    }

    return m_ready;
}
//==============================================================================
qint64 ComPort::write(const QByteArray &bytes)
{
    if (!serialPort.isOpen()) {
        m_lastError = tr("%1: Port is not open").arg(serialPort.portName());

#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        return 0;
    }

    qint64 count = serialPort.write(bytes);

    if (count > 0) {

#if !defined (WITHOUT_LOG)
        emit toLog( tr("%1: %2")
                    .arg(serialPort.portName())
                    .arg(QString( bytes.left( static_cast<int>(count) ).toHex(' '))), Log::LogOut );
        emit toLog( tr("%1: Write %2 bytes")
                    .arg(serialPort.portName())
                    .arg(count), Log::LogDbg );
#endif

        emit bytesWrite(count);
    }

    return count;
}
//==============================================================================
QByteArray ComPort::read(qint64 count)
{
    m_buffer.clear();

    if (!serialPort.isOpen()) {
        m_lastError = tr("%1: Port is not open").arg(serialPort.portName());

#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        return m_buffer;
    }

    m_buffer = (count < 0) ? serialPort.readAll() : serialPort.read(count);
    if(m_buffer.isEmpty()) return m_buffer;

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: %2")
                .arg(serialPort.portName())
                .arg(QString( m_buffer.toHex(' '))), Log::LogIn );
    emit toLog( tr("%1: Read %2 bytes")
                .arg(serialPort.portName())
                .arg(m_buffer.size()), Log::LogDbg );
#endif

    emit bytesRead( m_buffer.size() );

    if(serialPort.flowControl() != QSerialPort::SoftwareControl) return m_buffer;

    for(int i=0; i<m_buffer.size(); ++i) {

        if ( m_buffer.at(i) == m_charXon ) {
#if !defined (WITHOUT_LOG)
            emit toLog( tr("%1: XON symbol found")
                        .arg(serialPort.portName()), Log::LogDbg );
#endif
            emit xon(true);

            if(!m_ready) {
                m_ready = true;
                emit readyChange(m_ready);
            }
        }
        else if ( m_buffer.at(i) == m_charXoff ) {

#if !defined (WITHOUT_LOG)
            emit toLog( tr("%1: XOFF symbol found")
                        .arg(serialPort.portName()), Log::LogDbg );
#endif
            emit xon(false);

            if(m_ready) {
                m_ready = false;
                emit readyChange(m_ready);
            }
        }
    }

    return m_buffer;
}
//==============================================================================
QByteArray ComPort::readBuffer() const
{
    return m_buffer;
}
//==============================================================================
#if defined (QT_GUI_LIB)
void ComPort::fillComboBoxPortProperty(QComboBox *comboBox, ComPort::PortProperty portProperty,
                                       const QVariant &defaultValue)
{
    if(!comboBox) return;
    comboBox->clear();

    switch (portProperty) {
    case ComPort::PortName:
        fillComboBoxPortName(comboBox, defaultValue.toString());
        break;
    case ComPort::PortBaudRate:
        fillComboBoxBaudRate(comboBox, qvariant_cast<QSerialPort::BaudRate>(defaultValue));
        break;
    case ComPort::PortDataBits:
        fillComboBoxDataBits(comboBox, qvariant_cast<QSerialPort::DataBits>(defaultValue));
        break;
    case ComPort::PortStopBits:
        fillComboBoxStopBits(comboBox, qvariant_cast<QSerialPort::StopBits>(defaultValue));
        break;
    case ComPort::PortParity:
        fillComboBoxParity(comboBox, qvariant_cast<QSerialPort::Parity>(defaultValue));
        break;
    case ComPort::PortFlowControl:
        fillComboBoxFlowControl(comboBox, qvariant_cast<QSerialPort::FlowControl>(defaultValue));
        break;
    }
}
#endif
//==============================================================================
QString ComPort::baudRateToStr(QSerialPort::BaudRate baudRate)
{
    if(baudRate == QSerialPort::UnknownBaud) {
        return tr("Unknown");
    }
    else {
        return QString::number( static_cast<int>(baudRate) );
    }
}
//==============================================================================
QString ComPort::dataBitsToStr(QSerialPort::DataBits dataBits)
{
    if(dataBits == QSerialPort::UnknownDataBits) {
        return tr("Unknown");
    }
    else {
        return QString::number( static_cast<int>(dataBits) );
    }
}
//==============================================================================
QString ComPort::stopBitsToStr(QSerialPort::StopBits stopBits)
{
    switch (stopBits) {
    case QSerialPort::OneStop:
        return "1";
    case QSerialPort::OneAndHalfStop:
        return "1.5";
    case QSerialPort::TwoStop:
        return "2";
    default:
        return tr("Unknown");
    }
}
//==============================================================================
QString ComPort::parityToStr(QSerialPort::Parity parity)
{
    switch (parity) {
    case QSerialPort::NoParity:
        return tr("No");
    case QSerialPort::EvenParity:
        return tr("Even");
    case QSerialPort::OddParity:
        return tr("Odd");
    case QSerialPort::SpaceParity:
        return tr("Space");
    case QSerialPort::MarkParity:
        return tr("Mark");
    default:
        return tr("Unknown");
    }
}
//==============================================================================
QString ComPort::flowControlToStr(QSerialPort::FlowControl flowControl)
{
    switch (flowControl) {
    case QSerialPort::NoFlowControl:
        return tr("No");
    case QSerialPort::HardwareControl:
        return tr("Hardware");
    case QSerialPort::SoftwareControl:
        return tr("Software");
    default:
        return tr("Unknown");
    }
}
//==============================================================================
QSerialPort::BaudRate ComPort::strToBaudRate(const QString &value)
{
    bool ok;
    int br = value.toInt(&ok);
    if(ok) {
        return static_cast<QSerialPort::BaudRate>(br);
    }
    else {
        return QSerialPort::Baud9600;
    }
}
//==============================================================================
QSerialPort::DataBits ComPort::strToDataBits(const QString &value)
{
    bool ok;
    int db = value.toInt(&ok);
    if(ok && (db > 4) && (db < 9)) {
        return static_cast<QSerialPort::DataBits>(db);
    }
    else {
        return QSerialPort::Data8;
    }
}
//==============================================================================
QSerialPort::StopBits ComPort::strToStopBits(const QString &value)
{
    if(value == "2" || value.contains(tr("Two"))) {
        return QSerialPort::TwoStop;
    }
    else if(value == "1.5" || value == "1,5" || value.contains(tr("OneAndHalf"))) {
        return QSerialPort::OneAndHalfStop;
    }
    else {
        return QSerialPort::OneStop;
    }
}
//==============================================================================
QSerialPort::Parity ComPort::strToParity(const QString &value)
{
    if(value.contains(tr("Even"))) {
        return QSerialPort::EvenParity;
    }
    else if(value.contains(tr("Odd"))) {
        return QSerialPort::OddParity;
    }
    else if(value.contains(tr("Space"))) {
        return QSerialPort::SpaceParity;
    }
    else if(value.contains(tr("Mark"))) {
        return QSerialPort::MarkParity;
    }
    else {
        return QSerialPort::NoParity;
    }
}
//==============================================================================
QSerialPort::FlowControl ComPort::strToFlowControl(const QString &value)
{
    if(value.contains(tr("Hardware"))) {
        return QSerialPort::HardwareControl;
    }
    else if(value.contains(tr("Software"))) {
        return QSerialPort::SoftwareControl;
    }
    else {
        return QSerialPort::NoFlowControl;
    }
}
//==============================================================================
bool ComPort::autoRead() const
{
    return m_autoRead;
}
//==============================================================================
void ComPort::setAutoRead(bool autoRead)
{
    m_autoRead = autoRead;
}
//==============================================================================
qint64 ComPort::bufferSize() const
{
    return m_bufferSize;
}
//==============================================================================
void ComPort::setBufferSize(const qint64 &bufferSize)
{
    m_bufferSize = bufferSize;

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set buffer size: %2")
                .arg(serialPort.portName())
                .arg(m_bufferSize), Log::LogDbg );
#endif
}
//==============================================================================
QChar ComPort::charXon() const
{
    return m_charXon;
}
//==============================================================================
void ComPort::setCharXon(const QChar &charXon)
{
    m_charXon = charXon;

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set XON symbol value: %2")
                .arg(serialPort.portName())
                .arg(m_charXon), Log::LogDbg );
#endif
}
//==============================================================================
QChar ComPort::charXoff() const
{
    return m_charXoff;
}
//==============================================================================
void ComPort::setCharXoff(const QChar &charXoff)
{
    m_charXoff = charXoff;

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Set XOFF symbol value: %2")
                .arg(serialPort.portName())
                .arg(m_charXon), Log::LogDbg );
#endif
}
//==============================================================================
void ComPort::serialPort_errorOccurred(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::NoError) return;

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: %2")
                .arg(serialPort.portName())
                .arg(serialPort.errorString()), Log::LogError );
#endif

    emit portError();
}
//==============================================================================
void ComPort::serialPort_requestToSendChanged(bool set)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: RTS changed to '%2'")
                .arg(serialPort.portName())
                .arg(set ? tr("ON") : tr("OFF")), Log::LogDbg );
#endif

    emit rts(set);

    if(serialPort.flowControl() == QSerialPort::HardwareControl) {
        m_ready = set;
        emit readyChange( m_ready );
    }
}
//==============================================================================
void ComPort::serialPort_dataTerminalReadyChanged(bool set)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: DTR changed to '%2'")
                .arg(serialPort.portName())
                .arg(set ? tr("ON") : tr("OFF")), Log::LogDbg );
#endif

    emit dtr(set);
}
//==============================================================================
void ComPort::serialPort_readyRead()
{
    if(m_autoRead) {
        read();
    }
    else {
        emit readyRead();
    }
}
//==============================================================================

} // namespace nayk //==========================================================
