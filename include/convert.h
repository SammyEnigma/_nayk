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
#ifndef CONVERT_H
#define CONVERT_H

#include "NaykGlobal"
#include <QtCore>
#include <QString>
#include <QDateTime>
#include <QLocale>

namespace convert { //==========================================================

typedef union Value8bits {
    qint8 val_int;
    quint8 val_uint;
} ByteUnion;

typedef union Value16bits {
    qint16 val_int;
    quint16 val_uint;
    quint8 val_byte[2];
    qint8 val_sbyte[2];
} ShortUnion;

typedef union Value32bits {
    qint32 val_int;
    quint32 val_uint;
    float val_float;
    quint8 val_byte[4];
    qint8 val_sbyte[4];
} SingleUnion;

typedef union Value64bits {
    qint64 val_int;
    quint64 val_uint;
    double val_float;
    quint8 val_byte[8];
    qint8 val_sbyte[8];
} DoubleUnion;

//==============================================================================

NAYK_EXPORT quint8 bcdEncode(qint8 value);
NAYK_EXPORT quint16 bcdEncode(qint16 value);
NAYK_EXPORT quint32 bcdEncode(qint32 value);
NAYK_EXPORT quint64 bcdEncode(qint64 value);
NAYK_EXPORT quint8 bcdEncode(quint8 value);
NAYK_EXPORT quint16 bcdEncode(quint16 value);
NAYK_EXPORT quint32 bcdEncode(quint32 value);
NAYK_EXPORT quint64 bcdEncode(quint64 value);
NAYK_EXPORT qint8 bcdDecode(quint8 bcdValue, bool *ok = nullptr);
NAYK_EXPORT qint16 bcdDecode(quint16 bcdValue, bool *ok = nullptr);
NAYK_EXPORT qint32 bcdDecode(quint32 bcdValue, bool *ok = nullptr);
NAYK_EXPORT qint64 bcdDecode(quint64 bcdValue, bool *ok = nullptr);
NAYK_EXPORT quint8 bcdDecodeUnsigned(quint8 bcdValue, bool *ok = nullptr);
NAYK_EXPORT quint16 bcdDecodeUnsigned(quint16 bcdValue, bool *ok = nullptr);
NAYK_EXPORT quint32 bcdDecodeUnsigned(quint32 bcdValue, bool *ok = nullptr);
NAYK_EXPORT quint64 bcdDecodeUnsigned(quint64 bcdValue, bool *ok = nullptr);
NAYK_EXPORT QString intToHex(qint8  val, int len = 2, bool withPrefix = false);
NAYK_EXPORT QString intToHex(quint8  val, int len = 2, bool withPrefix = false);
NAYK_EXPORT QString intToHex(qint16 val, int len = 4, bool withPrefix = false);
NAYK_EXPORT QString intToHex(quint16 val, int len = 4, bool withPrefix = false);
NAYK_EXPORT QString intToHex(qint32 val, int len = 8, bool withPrefix = false);
NAYK_EXPORT QString intToHex(quint32 val, int len = 8, bool withPrefix = false);
NAYK_EXPORT QString intToHex(qint64 val, int len = 16, bool withPrefix = false);
NAYK_EXPORT QString intToHex(quint64 val, int len = 16, bool withPrefix = false);
NAYK_EXPORT qint8 strToIntDef(const QString &str, qint8 defaultValue);
NAYK_EXPORT quint8 strToIntDef(const QString &str, quint8 defaultValue);
NAYK_EXPORT qint16 strToIntDef(const QString &str, qint16 defaultValue);
NAYK_EXPORT quint16 strToIntDef(const QString &str, quint16 defaultValue);
NAYK_EXPORT qint32 strToIntDef(const QString &str, qint32 defaultValue);
NAYK_EXPORT quint32 strToIntDef(const QString &str, quint32 defaultValue);
NAYK_EXPORT qint64 strToIntDef(const QString &str, qint64 defaultValue);
NAYK_EXPORT quint64 strToIntDef(const QString &str, quint64 defaultValue);
NAYK_EXPORT QString doubleToStr(qreal val, int precise = 2,
                    QLocale::NumberOption numberOptions = QLocale::OmitGroupSeparator);
NAYK_EXPORT qreal strToDouble(const QString &str, bool *ok = nullptr );
NAYK_EXPORT QDateTime strToDateTime(const QString &str);
NAYK_EXPORT quint8  lo(quint16 val);
NAYK_EXPORT quint16 lo(quint32 val);
NAYK_EXPORT quint32 lo(quint64 val);
NAYK_EXPORT qint8  lo(qint16 val);
NAYK_EXPORT qint16 lo(qint32 val);
NAYK_EXPORT qint32 lo(qint64 val);
NAYK_EXPORT quint8  hi(quint16 val);
NAYK_EXPORT quint16 hi(quint32 val);
NAYK_EXPORT quint32 hi(quint64 val);
NAYK_EXPORT qint8  hi(qint16 val);
NAYK_EXPORT qint16 hi(qint32 val);
NAYK_EXPORT qint32 hi(qint64 val);
NAYK_EXPORT quint16 reverseBytes(quint16 val);
NAYK_EXPORT quint32 reverseBytes(quint32 val);
NAYK_EXPORT quint64 reverseBytes(quint64 val);
NAYK_EXPORT qint16 reverseBytes(qint16 val);
NAYK_EXPORT qint32 reverseBytes(qint32 val);
NAYK_EXPORT qint64 reverseBytes(qint64 val);
NAYK_EXPORT QString bytesToHex(const QByteArray &data, const QString &splitter = QString(" "));
NAYK_EXPORT QString bytesToHex(const char *data, qint64 count, const QString &splitter = QString(" "));

} // namespace convert //=======================================================
#endif // CONVERT_H
