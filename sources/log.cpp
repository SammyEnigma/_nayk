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
#include <QCoreApplication>
#include <QDir>

#include "AppCore"
#include "FileSys"
#include "log.h"

//==============================================================================

static nayk::Log *logger {nullptr};

//==============================================================================
void qtLogMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)
    nayk::Log::LogType logType = nayk::Log::LogOther;
    QString text = "";

    switch (type) {
    case QtDebugMsg:
        logType = nayk::Log::LogDbg;
        text = "Debug: ";
        break;
    case QtInfoMsg:
        logType = nayk::Log::LogInfo;
        text = "Info: ";
        break;
    case QtWarningMsg:
        logType = nayk::Log::LogWarning;
        text = "Warning: ";
        break;
    case QtCriticalMsg:
        logType = nayk::Log::LogError;
        text = "Critical: ";
        break;
    case QtFatalMsg:
        logType = nayk::Log::LogError;
        text = "Fatal: ";
        break;
    }

    text += msg.trimmed();

    fprintf(stderr, "%s\n", text.toLocal8Bit().constData());

    if(logger) {
        logger->saveToLog(text, logType);
    }
}
//==============================================================================

namespace nayk { //=============================================================

using namespace file_sys;

const QString errorStr_LogFileNotOpen = QObject::tr("Лог файл не открыт для записи");

//==============================================================================
QString generateLogFileName(const QString &logDir, QDateTime &dateTime)
{
    QString fileName = dateTime.toString("yyyyMMdd_HHmmss_zzz") + ".log";

    while (fileExists(logDir + fileName)) {

        dateTime = dateTime.addMSecs(1);
        fileName = dateTime.toString("yyyyMMdd_HHmmss_zzz") + ".log";
    }

    return fileName;
}
//==============================================================================
QString Log::getLogPrefix(LogType logType, const QDateTime &date)
{
    QString prefix = getLogTypeStr(logType);

    if(prefix.isEmpty())
        return prefix;

    return date.toString("[HH:mm:ss.zzz]") + prefix + " ";
}
//==============================================================================
Log::LogType Log::strToLogType(const QString &typeStr)
{
    QString str = typeStr.trimmed();

    if(str[0] != '[')
        str = "[" + str;

    if(str[str.length()-1] != ']')
        str += "]";

    if (getLogTypeStr(LogInfo) == str)
        return LogInfo;

    if (getLogTypeStr(LogWarning) == str)
        return LogWarning;

    if (getLogTypeStr(LogError) == str)
        return LogError;

    if (getLogTypeStr(LogIn) == str)
        return LogIn;

    if (getLogTypeStr(LogOut) == str)
        return LogOut;

    if (getLogTypeStr(LogText) == str)
        return LogText;

    if (getLogTypeStr(LogDbg) == str)
        return LogDbg;

    return LogOther;
}
//==============================================================================
void Log::deleteOldLogFiles(const QString &fileMask, int maxCount)
{
    saveToLog(tr("Удаление старых лог файлов из '%1', маска файлов: '%2', "
                 "количество сохраняемых логов: %3")
              .arg(m_logDir).arg(fileMask).arg(maxCount), Log::LogDbg);
    QDir dir(m_logDir);
    QStringList list = dir.entryList(QStringList(fileMask),
                                     QDir::Files | QDir::NoSymLinks);
    saveToLog(tr("Всего найдено файлов: %1").arg(list.size()), LogDbg);

    while(list.size() > maxCount) {

        QString fileName = list.takeFirst();
        if(QFile::remove( m_logDir + fileName )) {
            saveToLog(tr("Удаление файла '%1'").arg(fileName), Log::LogDbg);
        }
        else {
            saveToLog(tr("Не удалось удалить файл '%1'").arg(fileName), Log::LogDbg);
        }
    }
}
//==============================================================================
Log::Log(QObject *parent)
    : QObject(parent),
      m_startTime { QDateTime::currentDateTime() }
{
    m_logDir = app_core::applicationProfilePath() + defaultLogDirName;
    startLog();
}
//==============================================================================
Log::Log(QDateTime startTime, QObject *parent)
    : QObject(parent),
      m_startTime { startTime }
{
    m_logDir = app_core::applicationProfilePath() + defaultLogDirName;
    startLog();
}
//==============================================================================
Log::Log(const QString &logDir, QDateTime startTime, QObject *parent)
    : QObject(parent),
      m_startTime { startTime },
      m_logDir { QDir::fromNativeSeparators(logDir) }
{
    startLog();
}
//==============================================================================
Log::Log(const QString &logDir, const QString &fileName, QDateTime startTime, QObject *parent)
    : QObject(parent),
      m_startTime { startTime },
      m_fileName {fileName},
      m_logDir { QDir::fromNativeSeparators(logDir) }
{
    startLog(fileName);
}
//==============================================================================
Log::~Log()
{
    logger = nullptr;

    if (m_file.isOpen()) {

        if(!writeLastLine()) {
            emit error(m_lastError);
        }

        m_file.close();
        emit closeFile(m_file.fileName());
    }
}
//==============================================================================
void Log::setDebugSave(bool enable)
{
    m_dbgSave = enable;
    emit debugSaveChanged(m_dbgSave);
}
//==============================================================================
bool Log::debugSave() const
{
    return m_dbgSave;
}
//==============================================================================
QString Log::logDir() const
{
    return m_logDir;
}
//==============================================================================
QString Log::logFileName() const
{
    return m_fileName;
}
//==============================================================================
QString Log::lastError() const
{
    return m_lastError;
}
//==============================================================================
QString Log::getLogTypeStr(Log::LogType logType)
{
    switch (logType) {
    case LogInfo:    return "[inf]";
    case LogWarning: return "[wrn]";
    case LogError:   return "[err]";
    case LogIn:      return "[<<<]";
    case LogOut:     return "[>>>]";
    case LogText:    return "[txt]";
    case LogDbg:     return "[dbg]";
    default: break;
    }
    return "";
}
//==============================================================================
void Log::startLog(const QString &fileName)
{
    if(m_logDir.right(1) != directorySeparator)
        m_logDir += directorySeparator;

    if(fileName.isEmpty()) {
        m_fileName = generateLogFileName(m_logDir, m_startTime);
    }
    else {
        m_fileName = fileName;
    }

    if(!makePath(m_logDir)) {
        m_lastError = tr("Не удалось создать каталог '%1'").arg(m_logDir);
        emit error(m_lastError);
        return;
    }

    m_file.setFileName(m_logDir + m_fileName);

    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = tr("Не удалось создать файл '%1'").arg(m_file.fileName());
        emit error(m_lastError);
        return;
    }

    emit openFile(m_file.fileName());
    m_stream.setDevice(&m_file);

    if(writeFirstLine()) {
        logger = this;
        qInstallMessageHandler( qtLogMessageOutput );
    }
    else {
        emit error(m_lastError);
    }
}
//==============================================================================
bool Log::writeFirstLine()
{
    if(!m_file.isOpen()) {
        m_lastError = errorStr_LogFileNotOpen;
        return false;
    }

    QString logStr = "----- " + tr("Начало") + " -----";
    saveToLog(logStr.leftJustified(100,'-'), LogInfo);
    return true;
}
//==============================================================================
bool Log::writeLastLine()
{
    if(!m_file.isOpen()) {
        m_lastError = errorStr_LogFileNotOpen;
        return false;
    }

    QDateTime now = (m_startTime.offsetFromUtc() != 0)
            ? QDateTime::currentDateTime()
            : QDateTime::currentDateTimeUtc();
    qint64 n = now.toMSecsSinceEpoch() - m_startTime.toMSecsSinceEpoch();
    QString logStr = "----- " + tr("Окончание. Время работы: %1 мсек.").arg(n)
            + " -----";

    saveToLog(logStr.leftJustified(100,'-'), LogInfo);
    qInstallMessageHandler(0);
    return true;
}
//==============================================================================
void Log::saveToLog(const QString &text, LogType logType)
{
    if((logType == LogDbg) && !m_dbgSave) return;

    if(!m_file.isOpen()) {
        m_lastError = errorStr_LogFileNotOpen;
        emit error(m_lastError);
        return;
    }

    QDateTime now = (m_startTime.offsetFromUtc() != 0)
            ? QDateTime::currentDateTime()
            : QDateTime::currentDateTimeUtc();
    QString prefix = getLogPrefix(logType, now);
    QStringList sl = text.split("\n");

    for(int i=0; i < sl.size(); i++) {

        QString str = prefix + sl.at(i);
        m_stream << str << "\n";
        emit write(str);
    }

    m_stream.flush();
    m_file.flush();

    if (m_stream.status() != QTextStream::Ok) {
        m_file.close();
        m_lastError = tr("Ошибка при записи в файл '%1'").arg(m_file.fileName());
        emit error(m_lastError);
        emit closeFile(m_file.fileName());
    }
}
//==============================================================================

} // namespace nayk //==========================================================
