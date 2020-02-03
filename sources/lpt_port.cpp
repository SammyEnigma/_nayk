#include "Convert"
#include "SystemUtils"
#include "lpt_port.h"

#ifdef Q_OS_WIN32
#   include <vector>
#   include <minwindef.h>
#   include <winreg.h>
#endif

namespace nayk { //=============================================================

//==============================================================================

#ifdef Q_OS_WIN32
static QStringList portNamesFromHardwareDeviceMap()
{
    HKEY hKey = nullptr;
    QStringList result;

    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\PARALLEL PORTS", 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        return result;

    DWORD index = 0;

    enum { MaximumValueNameInChars = 16383 };

    std::vector<wchar_t> outputValueName(MaximumValueNameInChars, 0);
    std::vector<wchar_t> outputBuffer(MAX_PATH + 1, 0);
    DWORD bytesRequired = MAX_PATH;
    for (;;) {
        DWORD requiredValueNameChars = MaximumValueNameInChars;
        const LONG ret = ::RegEnumValue(hKey, index, &outputValueName[0], &requiredValueNameChars,
                nullptr, nullptr, reinterpret_cast<PBYTE>(&outputBuffer[0]), &bytesRequired);
        if (ret == ERROR_MORE_DATA) {
            outputBuffer.resize(bytesRequired / sizeof(wchar_t) + 2, 0);
        }
        else if (ret == ERROR_SUCCESS) {

            QString value = QString::fromWCharArray(&outputBuffer[0]);
            int n = value.lastIndexOf("\\");
            if(n >= 0) value.remove(0, n + 1);
            if(!value.isEmpty()) {
                result.append(value);
            }
            ++index;
        } else {
            break;
        }
    }
    ::RegCloseKey(hKey);
    return result;
}
#endif
//==============================================================================
LptPort::LptPort(QObject *parent) : AbstractPort(parent)
{

}
//==============================================================================
LptPort::~LptPort()
{
    close();
}
//==============================================================================
bool LptPort::open(bool readOnly)
{
    if(isOpen()) return true;

#ifdef Q_OS_WIN32
    LPCWSTR fileName = reinterpret_cast<LPCWSTR>(m_portName.utf16());
    DWORD mode = readOnly ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE;
    m_handle = CreateFile( fileName, mode, 0, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if(m_handle == INVALID_HANDLE_VALUE) {

        m_lastError = tr("Не удалось открыть порт: %1 %2")
                .arg("Error CreateFile")
                .arg(GetLastError());
        port_error();
        return false;
    }
#else
    if(m_portName.left(5) != "/dev/")
        m_deviceFile.setFileName("/dev/" + m_portName);
    else
        m_deviceFile.setFileName(m_portName);

    if(!m_deviceFile.open( readOnly ? QIODevice::ReadOnly : QIODevice::ReadWrite )) {

        m_lastError = tr("Не удалось открыть порт: %1")
                .arg(m_deviceFile.errorString());
        port_error();
        return false;
    }
#endif

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Порт открыт").arg(m_portName), Log::LogInfo );
#endif
    emit portOpen();
    return true;
}
//==============================================================================
void LptPort::close()
{
    if(!isOpen()) return;

#ifdef Q_OS_WIN32
    FlushFileBuffers(m_handle);
    system_utils::pause(100);
    CloseHandle(m_handle);
    m_handle = INVALID_HANDLE_VALUE;
#else
    m_deviceFile.flush();
    system_utils::pause(100);
    m_deviceFile.close();
#endif

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Порт закрыт").arg(m_portName), Log::LogInfo );
#endif

    emit portClose();
}
//==============================================================================
bool LptPort::isOpen() const
{
#ifdef Q_OS_WIN32
    return m_handle != INVALID_HANDLE_VALUE;
#else
    return m_deviceFile.isOpen();
#endif
}
//==============================================================================
qint64 LptPort::write(const QByteArray &bytes)
{
    if (!isOpen()) {
        m_lastError = tr("Порт не открыт");
        port_error();
        return 0;
    }

    qint64 count = 0;

#ifdef Q_OS_WIN32
    DWORD dw = 0;
    //OVERLAPPED ov;

    if( !WriteFile(m_handle,
                   reinterpret_cast<LPCVOID>( bytes.constData() ),
                   bytes.size(), &dw, NULL) ) {
        m_lastError = tr("Ошибка записи: %1 %2")
                .arg("Error WriteFile")
                .arg(GetLastError());
        port_error();
        return 0;
    }
    FlushFileBuffers(m_handle);
    count = static_cast<qint64>( dw );

#else
    count = m_deviceFile.write(bytes);

    if( (count < 0) || !m_deviceFile.flush()) {
        m_lastError = tr("Ошибка записи: %1").arg(m_deviceFile.errorString());
        port_error();
        return 0;
    }
#endif

#if !defined (WITHOUT_LOG)
    if (count > 0) {

        emit toLog( tr("%1: %2")
                    .arg(m_portName)
                    .arg(convert::bytesToHex(bytes.left( static_cast<int>(count)))), Log::LogOut );
        emit toLog( tr("%1: Записано в буфер %2 байт")
                    .arg(m_portName)
                    .arg(count), Log::LogDbg );
    }
#endif

    port_bytesWriten(count);
    return count;
}
//==============================================================================
QString LptPort::portName() const
{
    return m_portName;
}
//==============================================================================
void LptPort::setPortName(const QString &portName)
{
    if(m_portName == portName) return;

    if(isOpen()) close();
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Установлено имя порта: %2")
                .arg(m_portName)
                .arg(portName), Log::LogDbg );
#endif

    m_portName = portName;
}
//==============================================================================
QByteArray LptPort::read(qint64 count)
{
    m_buffer.clear();

    if (!isOpen()) {
        m_lastError = tr("Порт не открыт");
        port_error();
        return m_buffer;
    }

#ifdef Q_OS_WIN32
    DWORD lpSize = (count < 0) ? MAXDWORD : static_cast<DWORD>(count);

    if(!ReadFile(m_handle, reinterpret_cast<LPVOID>(m_buffer.data()),
                 lpSize, NULL, NULL )) {

        m_lastError = tr("Ошибка чтения: %1 %2")
                .arg("Error ReadFile")
                .arg(GetLastError());
        port_error();
        return m_buffer;
    }

#else
    m_buffer = (count < 0) ? m_deviceFile.readAll() : m_deviceFile.read(count);
#endif

    if(m_buffer.isEmpty()) return m_buffer;

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: %2")
                .arg(m_portName)
                .arg(convert::bytesToHex(m_buffer)), Log::LogIn );
    emit toLog( tr("%1: Прочитано %2 байт")
                .arg(m_portName)
                .arg(m_buffer.size()), Log::LogDbg );
#endif

    emit bytesRead( m_buffer.size() );
    return m_buffer;
}
//==============================================================================
QStringList LptPort::availablePorts()
{
    QStringList list;

#ifdef Q_OS_WIN32

    list = portNamesFromHardwareDeviceMap();
#else

    QStringList namesList;
    namesList << "lpt%1" << "lp%1" << "parport%1";

    for(const QString &portName: namesList) {

        for(int i=0; i<10; ++i) {

            QString port = portName.arg(i);

            if(QFile::exists( "/dev/" + port )) {
                list.append( port );
            }

            port = "usb/" + portName.arg(i);

            if(QFile::exists( "/dev/" + port )) {

                list.append( port );
            }
        }
    }

#endif
    list.sort();
    return list;
}
//==============================================================================
#if defined (QT_GUI_LIB)
void LptPort::fillComboBoxAvailablePorts(QComboBox *comboBox, const QVariant &defaultValue)
{
    if(!comboBox) return;
    int index = -1;
    QStringList portList = availablePorts();
    for(const QString &portName: portList) {

        comboBox->addItem( portName, portName );
        if(defaultValue == portName) index = comboBox->count()-1;
    }
    if(index >= 0) comboBox->setCurrentIndex(index);
}
#endif
//==============================================================================
void LptPort::port_error()
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: %2")
                .arg(m_portName)
                .arg(m_lastError), Log::LogError );
#endif

    emit portError();
}
//==============================================================================
void LptPort::port_bytesWriten(qint64 count)
{
#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Записано в порт из буфера %2 байт")
                .arg(m_portName)
                .arg(count), Log::LogDbg );
#endif

    emit bytesWrite(count);
}
//==============================================================================

//==============================================================================

} // namespace //===============================================================
