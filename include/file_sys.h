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
#ifndef FILE_SYS_H
#define FILE_SYS_H

#include "NaykGlobal"
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace file_sys { //=========================================================

const QChar directorySeparator = '/';

//==============================================================================

NAYK_EXPORT bool directoryExists(const QString &dirName);
NAYK_EXPORT bool fileExists(const QString &fileName);
NAYK_EXPORT bool makePath(const QString &path);
NAYK_EXPORT QString changeFileExt(const QString &fileName, const QString &newExt);
NAYK_EXPORT QString extractFileExt(const QString &fileName, bool withDot = true);
NAYK_EXPORT QString extractFileName(const QString &fileName);
NAYK_EXPORT QString extractFilePath(const QString &fileName);
NAYK_EXPORT bool readJsonFromFile(const QString &fileName, QJsonDocument &json, QString &errorString);
NAYK_EXPORT bool readJsonFromFile(const QString &fileName, QJsonArray &json, QString &errorString);
NAYK_EXPORT bool readJsonFromFile(const QString &fileName, QJsonObject &json, QString &errorString);
NAYK_EXPORT bool writeJsonToFile(const QString &fileName, const QJsonDocument &json, QString &errorString);
NAYK_EXPORT bool writeJsonToFile(const QString &fileName, const QJsonArray &json, QString &errorString);
NAYK_EXPORT bool writeJsonToFile(const QString &fileName, const QJsonObject &json, QString &errorString);
NAYK_EXPORT bool readFile(const QString &fileName, QByteArray &data, QString &errorString);
NAYK_EXPORT bool writeFile(const QString &fileName, const QByteArray &data, QString &errorString);
NAYK_EXPORT qint64 fileSize(const QString &fileName);

} // namespace file_sys //======================================================
#endif // FILE_SYS_H
