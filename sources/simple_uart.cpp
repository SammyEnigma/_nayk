#include <QDir>
#include <stdio.h>
#include "Convert"
#include "SystemUtils"
#include "simple_uart.h"

#if defined (Q_OS_LINUX)
#   include <unistd.h>
#   include <termios.h>
#   include <fcntl.h>
#endif

namespace nayk { //=============================================================

//==============================================================================

#if defined (Q_OS_WIN32)
const int baudRates[10] = {CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_19200, CBR_38400, CBR_57600, CBR_115200};
#endif

#if defined (Q_OS_LINUX)
const int baudRates[10] = {B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200};
#endif

const int baudRatesVal[10] = {300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
const int portWaitTimeout {5000};

//==============================================================================
SimpleUart::SimpleUart(QObject *parent) : AbstractPort(parent)
{

}
//==============================================================================
SimpleUart::SimpleUart(const QString &portName, QObject *parent)
    : AbstractPort(parent), m_portName(portName)
{

}
//==============================================================================
SimpleUart::~SimpleUart()
{
    close();
}
//==============================================================================
bool SimpleUart::setPortName(const QString &portName)
{

}
//==============================================================================
bool SimpleUart::setBaudRate(qint32 baudRate)
{

}
//==============================================================================
QString SimpleUart::portName() const
{

}
//==============================================================================
qint32 SimpleUart::baudRate() const
{

}
//==============================================================================
bool SimpleUart::open(bool readOnly)
{
    close();

#ifdef Q_OS_LINUX
    QString portName = "/dev/" + m_portName;
    int fd = ::open(portName.toLatin1().constData(),
                    (readOnly ? O_RDONLY : O_RDWR) | O_NOCTTY | O_NDELAY);

    if (fd < 0) {

        m_lastError = tr("%1: Failed to open port")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        return false;
    }

    if( lockf(fd, F_TEST, 0) < 0 ) {

        m_lastError = tr("%1: Failed to locked")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        ::close(fd);
        return false;
    }
    else if( lockf(fd, F_LOCK, 0) < 0 ) {

        m_lastError = tr("%1: Failed to locked")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        ::close(fd);
        return false;
    }

    if(fcntl(fd, F_SETFL, 0) < 0) {

        m_lastError = tr("%1: Failed to setfl")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        lockf(fd, F_ULOCK, 0);
        ::close(fd);
        return false;
    }

    struct termios options;

    if (tcgetattr(fd, &options) < 0) {

        m_lastError = tr("%1: Failed to get termios")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        lockf(fd, F_ULOCK, 0);
        ::close(fd);
        return false;
    }

    // Baudrate
    if(cfsetispeed(&options, baudRates[m_baudRate]) < 0) {

        m_lastError = tr("%1: Failed to set baudrate")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        lockf(fd, F_ULOCK, 0);
        ::close(fd);
        return false;
    }

    if(cfsetospeed(&options, baudRates[m_baudRate]) < 0) {

        m_lastError = tr("%1: Failed to set baudrate")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        lockf(fd, F_ULOCK, 0);
        ::close(fd);
        return false;
    }

    // Bit size
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Parity
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~PARODD;

    // Stop bits
    options.c_cflag &= ~CSTOPB;

    // Flow Control
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    options.c_cflag |= (CLOCAL | CREAD);


    options.c_iflag &= ~INLCR;
    options.c_iflag &= ~ICRNL;
    options.c_iflag &= ~IGNCR;
    options.c_oflag &= ~OPOST;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &options) < 0) {

        m_lastError = tr("%1: Failed to set termios")
                        .arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        lockf(fd, F_ULOCK, 0);
        ::close(fd);
        return false;
    }

    m_handle = fd;
#endif //LINUX

#ifdef WINDOWS
    DCB dcb;
    BOOL fSuccess;
    portName = "\\\\.\\" + m_portName;

    m_handle = CreateFileA(reinterpret_cast<LPCSTR>(portName.toLatin1().data()),
                        GENERIC_READ | GENERIC_WRITE,
                        0,			// comm devices must be opened w/exclusive-access
                        NULL,			// no security attrs
                        OPEN_EXISTING,          // comm devices must use OPEN_EXISTING
                        0,			// FILE_FLAG_OVERLAPPED, not overlapped I/O
                        NULL			// hTemplate must be NULL for comm devices
                        );


    if ( m_handle == INVALID_HANDLE_VALUE) {
        if (logComFile) {
            logComFile->write("ERROR - INVALID_HANDLE_VALUE\n");
            logComFile->flush();
        }
        return false;
    }

    fSuccess = GetCommState( m_handle, &dcb);

    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in GetCommState()\n");
            logComFile->flush();
        }
        return false;
    }

    dcb.BaudRate = baudRates[m_baudRate];	// current baud rate
    dcb.fBinary = 1;			// binary mode, no EOF check
    dcb.fParity = 0;			// disable parity checking
    dcb.fOutxCtsFlow = 0;		// CTS output flow control
    dcb.fOutxDsrFlow = 0;		// DSR output flow control
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    // DTR flow control type
    dcb.fDsrSensitivity = 0;	// DSR sensitivity
    dcb.fTXContinueOnXoff = 0;	// XOFF continues Tx
    dcb.fOutX = 0;		// XON/XOFF out flow control
    dcb.fInX = 0;		// XON/XOFF in flow control
    //	dcb.XoffLim=128;
    //	dcb.XonLim=256;
    dcb.fErrorChar = 0;		// enable error replacement
    dcb.fNull = 0;		// enable null stripping
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    // RTS flow control
    dcb.fAbortOnError = 1;	// abort reads/writes on error
    dcb.ByteSize = 8;           // number of bits/byte, 4-8
    dcb.Parity = 0;             // 0-4=no,odd,even,mark,space
    dcb.StopBits = 0;           // 0,1,2 = 1, 1.5, 2

    fSuccess = SetCommState( m_handle, &dcb);

    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in SetCommState()\n");
            logComFile->flush();
        }
        return false;
    }

    fSuccess = SetupComm( m_handle, 1048576, 1048576);

    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in SetupComm()\n");
            logComFile->flush();
        }
        return false;
    }

    COMMTIMEOUTS tOut;
    tOut.ReadIntervalTimeout = 3;
    tOut.ReadTotalTimeoutMultiplier = 2;
    tOut.ReadTotalTimeoutConstant = 50;
    tOut.WriteTotalTimeoutMultiplier = 2;
    tOut.WriteTotalTimeoutConstant = 50;

    fSuccess = SetCommTimeouts(m_handle, &tOut );

    if(!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in SetCommTimeouts()\n");
            logComFile->flush();
        }
        return false;
    }

    PurgeComm( m_handle, PURGE_TXCLEAR | PURGE_RXCLEAR );

#endif //WINDOWS

#if !defined (WITHOUT_LOG)
    emit toLog( tr("%1: Port is open").arg(m_portName), Log::LogInfo );
    emit toLog( tr("%1: %2")
                .arg(m_portName)
                .arg(portSettings())
                , Log::LogDbg );
#endif

    return true;
}
//==============================================================================
void SimpleUart::close()
{

}
//==============================================================================
bool SimpleUart::isOpen() const
{

}
//==============================================================================
bool SimpleUart::isReady()
{

}
//==============================================================================
qint64 SimpleUart::write(const QByteArray &bytes)
{
    return write( bytes.constData(), bytes.size() );
}
//==============================================================================
qint64 SimpleUart::write(const char *bytes, qint64 bytesCount)
{
    if (!isOpen()) {
        m_lastError = tr("%1: Port is not open").arg(m_portName);

#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        return 0;
    }

    if(bytesCount < 1) return 0;
    qint64 result;

#ifdef Q_OS_LINUX
    result = ::write(m_handle, bytes, bytesCount);
#endif
#ifdef Q_OS_WIN32
    DWORD rp;
    WriteFile(m_handle, bytes, bytesCount, &rp, NULL);
    result = rp;
#endif

    if(result < 0) {
        m_lastError = tr("%1: Write error").arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        return 0;
    }

    if (result > 0) {
#if !defined (WITHOUT_LOG)

        emit toLog( tr("%1: %2")
                    .arg(m_portName)
                    .arg(convert::bytesToHex( bytes, result, " ")),
                    Log::LogOut );
        emit toLog( tr("%1: %2 bytes written to buffer")
                    .arg(m_portName)
                    .arg(result), Log::LogDbg );
#endif

        emit bytesWrite(result);
    }

    return result;
}
//==============================================================================
int SimpleUart::write(char byte)
{
    return write(&byte, 1);
}
//==============================================================================
QByteArray SimpleUart::read(qint64 count)
{
    QByteArray buf(count, 0);
    int n = read(buf.data(), count);

    return (n > 0) ? buf.left(n) : QByteArray();
}
//==============================================================================
qint64 SimpleUart::read(char *bytes, qint64 count)
{
    m_buffer.clear();

    if (!isOpen()) {
        m_lastError = tr("%1: Port is not open").arg(m_portName);
#if !defined (WITHOUT_LOG)
        emit toLog( m_lastError, Log::LogError);
#endif
        return 0;
    }

    qint64 result {0};
    qint64 realRead {0};
    char buffer[count];
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    int timeMs {0};

    do
    {
#ifdef Q_OS_LINUX
        result = ::read(m_handle, buffer, (count - realRead));
#endif //LINUX
#ifdef Q_OS_WIN32
        DWORD rp;
        ReadFile(m_handle, buffer, (count - realRead), &rp, NULL);
        result = rp;
#endif //WINDOWS

        if (result < 1 ) {

            system_utils::pause( 50 );
        }
        else {
            for (int i=0; i < result; ++i)
                *(bytes + realRead + i) = buffer[i];

            realRead += result;
            startTime = QDateTime::currentMSecsSinceEpoch();
        }

        timeMs = QDateTime::currentMSecsSinceEpoch() - startTime;

    } while ((realRead < count) && (timeMs < portWaitTimeout) );


    m_buffer = QByteArray(bytes, realRead);

    for (int i = realRead; i < count; ++i)
        *(bytes + i) = 0;

#if !defined (WITHOUT_LOG)
        emit toLog( tr("%1: %2")
                    .arg(m_portName)
                    .arg(convert::bytesToHex(m_buffer), " "), Log::LogIn );
        emit toLog( tr("%1: Read %2 bytes")
                    .arg(m_portName)
                    .arg(realRead), Log::LogDbg );
#endif

    emit bytesRead( realRead );
    return realRead;
}
//==============================================================================
QString SimpleUart::portSettings()
{

}
//==============================================================================
QStringList SimpleUart::availablePorts()
{
#ifdef Q_OS_LINUX
    static const QStringList deviceFileNameFilterList = QStringList()
            << QString("ttyS*")
            << QString("ttyUSB*");
    QStringList result;
    QDir deviceDir(QString("/dev"));

    if (deviceDir.exists()) {

        deviceDir.setNameFilters(deviceFileNameFilterList);
        deviceDir.setFilter(QDir::Files | QDir::System | QDir::NoSymLinks);

        QStringList deviceFilePaths;
        const auto deviceFileInfos = deviceDir.entryInfoList();

        for (const QFileInfo &deviceFileInfo : deviceFileInfos) {

            const QString deviceAbsoluteFilePath = deviceFileInfo.absoluteFilePath();

            if (!deviceFilePaths.contains(deviceAbsoluteFilePath)) {

                deviceFilePaths.append(deviceAbsoluteFilePath);
                result.append(deviceAbsoluteFilePath.mid(5));
            }
        }
    }

    return result;
#endif

#ifdef Q_OS_WIN32
    HKEY hKey = nullptr;
    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        return QStringList();

    QStringList result;
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
        } else if (ret == ERROR_SUCCESS) {

            QString str = QString::fromWCharArray(&outputBuffer[0]);
            int n = str.toLower().indexOf("com");
            result.append( n < 0 ? str : str.mid(n) );
            ++index;
        } else {
            break;
        }
    }
    ::RegCloseKey(hKey);

    return result;
#endif
}
//==============================================================================
QStringList SimpleUart::availableBaudRates()
{
    QStringList baudratesList;

    for(int i=0; i<10; ++i)
        baudratesList << QString::number(baudRatesVal[i]);

    return baudratesList;
}
//==============================================================================
#if defined (QT_GUI_LIB)
void SimpleUart::fillComboBoxPortName(QComboBox *comboBox, const QVariant &defaultValue)
{
    if(!comboBox) return;

    comboBox->clear();
    QStringList ports = availablePorts();

    int index = -1;
    for(auto i=0; i<ports.size(); ++i) {

        QString portName = ports.at(i);
        comboBox->addItem( portName, portName );
        if(portName == defaultValue.toString()) index = i;
    }

    if(index >= 0) comboBox->setCurrentIndex(index);
}
#endif
//==============================================================================







/*

int ComAPI::openCom(const QString &portName, int speedIndex)
{
    closeCom();

#ifdef LINUX
    m_portName = "/dev/" + portName;
    int fd = open(m_portName.toLatin1().constData(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd < 0) {

        if (logComFile) {
            logComFile->write("ERROR - open\n");
            logComFile->flush();
        }

        return -1;
    }

    if( lockf(fd, F_TEST, 0) < 0 ) {

        if (logComFile) {
            logComFile->write("ERROR - lockf(fd, F_TEST, 0)\n");
            logComFile->flush();
        }

        close(fd);
        return -1;
    }
    else if( lockf(fd, F_LOCK, 0) < 0 ) {

        if (logComFile) {
            logComFile->write("ERROR - lockf(fd, F_LOCK, 0)\n");
            logComFile->flush();
        }

        close(fd);
        return -1;
    }

    if(fcntl(fd, F_SETFL, 0) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - fcntl(fd, F_SETFL, 0)\n");
            logComFile->flush();
        }

        lockf(fd, F_ULOCK, 0);
        close(fd);
        return -1;
    }

    struct termios options;

    if (tcgetattr(fd, &options) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - tcgetattr(fd, &options)\n");
            logComFile->flush();
        }

        lockf(fd, F_ULOCK, 0);
        close(fd);
        return -1;
    }

    // Baudrate
    if(cfsetispeed(&options, Baudrates[speedIndex]) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - cfsetispeed(&options, Baudrates[speed])\n");
            logComFile->flush();
        }

        lockf(fd, F_ULOCK, 0);
        close(fd);
        return -1;
    }

    if(cfsetospeed(&options, Baudrates[speedIndex]) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - cfsetospeed(&options, Baudrates[speed])\n");
            logComFile->flush();
        }

        lockf(fd, F_ULOCK, 0);
        close(fd);
        return -1;
    }

    // Bit size
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Parity
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~PARODD;

    // Stop bits
    options.c_cflag &= ~CSTOPB;

    // Flow Control
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    options.c_cflag |= (CLOCAL | CREAD);


    options.c_iflag &= ~INLCR;
    options.c_iflag &= ~ICRNL;
    options.c_iflag &= ~IGNCR;
    options.c_oflag &= ~OPOST;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &options) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - tcsetattr(fd, TCSANOW, &options)\n");
            logComFile->flush();
        }

        lockf(fd, F_ULOCK, 0);
        close(fd);
        return -1;
    }

    comId = fd;
#endif //LINUX

#ifdef WINDOWS
    DCB dcb;
    BOOL fSuccess;
    m_portName = "\\\\.\\" + portName;

    comId = CreateFileA(reinterpret_cast<LPCSTR>(m_portName.toLatin1().data()),
                        GENERIC_READ | GENERIC_WRITE,
                        0,			// comm devices must be opened w/exclusive-access
                        NULL,			// no security attrs
                        OPEN_EXISTING,          // comm devices must use OPEN_EXISTING
                        0,			// FILE_FLAG_OVERLAPPED, not overlapped I/O
                        NULL			// hTemplate must be NULL for comm devices
                        );


    if ( comId == INVALID_HANDLE_VALUE) {
        if (logComFile) {
            logComFile->write("ERROR - INVALID_HANDLE_VALUE\n");
            logComFile->flush();
        }
        return -1;
    }

    fSuccess = GetCommState( comId, &dcb);

    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in GetCommState()\n");
            logComFile->flush();
        }
        return -1;
    }

    dcb.BaudRate = Baudrates[speedIndex];	// current baud rate
    dcb.fBinary = 1;			// binary mode, no EOF check
    dcb.fParity = 0;			// disable parity checking
    dcb.fOutxCtsFlow = 0;		// CTS output flow control
    dcb.fOutxDsrFlow = 0;		// DSR output flow control
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    // DTR flow control type
    dcb.fDsrSensitivity = 0;	// DSR sensitivity
    dcb.fTXContinueOnXoff = 0;	// XOFF continues Tx
    dcb.fOutX = 0;		// XON/XOFF out flow control
    dcb.fInX = 0;		// XON/XOFF in flow control
    //	dcb.XoffLim=128;
    //	dcb.XonLim=256;
    dcb.fErrorChar = 0;		// enable error replacement
    dcb.fNull = 0;		// enable null stripping
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    // RTS flow control
    dcb.fAbortOnError = 1;	// abort reads/writes on error
    dcb.ByteSize = 8;           // number of bits/byte, 4-8
    dcb.Parity = 0;             // 0-4=no,odd,even,mark,space
    dcb.StopBits = 0;           // 0,1,2 = 1, 1.5, 2

    fSuccess = SetCommState( comId, &dcb);

    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in SetCommState()\n");
            logComFile->flush();
        }
        return -2;
    }

    fSuccess = SetupComm( comId, 1048576, 1048576);

    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in SetupComm()\n");
            logComFile->flush();
        }
        return -3;
    }

    COMMTIMEOUTS tOut;
    tOut.ReadIntervalTimeout = 3;
    tOut.ReadTotalTimeoutMultiplier = 2;
    tOut.ReadTotalTimeoutConstant = 50;
    tOut.WriteTotalTimeoutMultiplier = 2;
    tOut.WriteTotalTimeoutConstant = 50;

    fSuccess = SetCommTimeouts(comId, &tOut );

    if(!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in SetCommTimeouts()\n");
            logComFile->flush();
        }
        return -4;
    }

    PurgeComm( comId, PURGE_TXCLEAR | PURGE_RXCLEAR );

#endif //WINDOWS

    if (logComFile) {

        QString log = QString("%1\tPort is open: %2\n")
                .arg(QDateTime::currentDateTime().toString("hh:mm:ss:zzz"))
                .arg(portSettings());
        logComFile->write(log.toLatin1());
        logComFile->flush();
    }

    return 0;
}
//==============================================================================
int ComAPI::closeCom()
{
    if(!isOpen()) return 0;

#ifdef LINUX
    lockf(comId, F_ULOCK, 0);
    close(comId);
    comId = -1;
#endif
#ifdef WINDOWS
    PurgeComm( comId, PURGE_TXCLEAR | PURGE_RXCLEAR );
    CloseHandle(comId);
    comId = INVALID_HANDLE_VALUE;
#endif

    if (logComFile) {
        QString log = QString("%1\tPort is close\n")
                .arg(QDateTime::currentDateTime().toString("hh:mm:ss:zzz"));
        logComFile->write(log.toLatin1());
        logComFile->flush();
    }

    return 0;
}
//==============================================================================
int ComAPI::changeSpeedCom(int speedIndex)
{
#ifdef LINUX
    struct termios options;

    if( tcgetattr(comId, &options) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - tcgetattr(comId, &options)\n");
            logComFile->flush();
        }

        return -1;
    }

    if(cfsetispeed(&options, Baudrates[speedIndex]) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - cfsetispeed(&options, Baudrates[speed])\n");
            logComFile->flush();
        }

        return -1;
    }

    if(cfsetospeed(&options, Baudrates[speedIndex]) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - cfsetospeed(&options, Baudrates[speed])\n");
            logComFile->flush();
        }

        return -1;
    }

    if (tcsetattr(comId, TCSANOW, &options) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - set baudrate\n");
            logComFile->flush();
        }

        return -1;
    }
#endif

#ifdef WINDOWS
    DCB dcb;
    BOOL fSuccess;

    fSuccess = GetCommState(comId, &dcb);
    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in GetCommState()\n");
            logComFile->flush();
        }
        return -1;
    }
    dcb.BaudRate = Baudrates[speedIndex];	// current baud rate

    fSuccess = SetCommState(comId, &dcb);
    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in SetCommState()\n");
            logComFile->flush();
        }
        return -3;
    }
#endif

    if (logComFile) {
        QString log = QString("%1\tPort change speed: %2\n")
                .arg(QDateTime::currentDateTime().toString("hh:mm:ss:zzz"))
                .arg(portSettings());
        logComFile->write(log.toLatin1());
        logComFile->flush();
    }
    return 0;
}
//==============================================================================
QString ComAPI::portSettings()
{
    if(!isOpen()) return QString();

    int speed = 0;

#ifdef LINUX
    struct termios options;

    if( tcgetattr(comId, &options) < 0) {

        if (logComFile) {
            logComFile->write("ERROR - tcgetattr(comId, &options)\n");
            logComFile->flush();
        }
    }
    else
        speed = cfgetispeed(&options);
#endif

#ifdef WINDOWS
    DCB dcb;
    BOOL fSuccess;

    fSuccess = GetCommState(comId, &dcb);
    if (!fSuccess) {
        if (logComFile) {
            logComFile->write("ERROR - in GetCommState()\n");
            logComFile->flush();
        }
    }
    else
        speed = dcb.BaudRate;
#endif

    for(int i=0; i<10; ++i) {

        if(Baudrates[i] == speed) {
            speed = BaudratesVal[i];
            break;
        }
    }

    return QString("%1, %2").arg(m_portName).arg(speed);
}
//==============================================================================
bool ComAPI::isOpen()
{
#ifdef LINUX
    return (comId != -1);
#endif
#ifdef WINDOWS
    return (comId != INVALID_HANDLE_VALUE);
#endif
}
//==============================================================================
void ComAPI::pauseMsec(int msec)
{
    QTimer timer;
    timer.setInterval(msec == 0 ? 1 : qAbs(msec));
    timer.setSingleShot(true);
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start();
    loop.exec();
}
//==============================================================================
*/
} // namespace nayk //==========================================================
