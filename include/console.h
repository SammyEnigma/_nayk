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
#ifndef CONSOLE_H
#define CONSOLE_H

#include "NaykGlobal"
#include <QtCore>
#include <QVariant>
#include <QRect>
#include <QSize>

namespace console { //==========================================================

enum Color {
    ColorNone          = -1,
    ColorBlack         = 0,
    ColorRed           = 1,
    ColorGreen         = 2,
    ColorYellow        = 3,
    ColorBlue          = 4,
    ColorMagenta       = 5,
    ColorCyan          = 6,
    ColorWhite         = 7,
    ColorBrightBlack   = 8,
    ColorBrightRed     = 9,
    ColorBrightGreen   = 10,
    ColorBrightYellow  = 11,
    ColorBrightBlue    = 12,
    ColorBrightMagenta = 13,
    ColorBrightCyan    = 14,
    ColorBrightWhite   = 15
};

//==============================================================================
NAYK_EXPORT void clear();
NAYK_EXPORT void clearUp();
NAYK_EXPORT void clearDown();
NAYK_EXPORT void setPos(int row, int col);
NAYK_EXPORT void setPosUp(int step = 1);
NAYK_EXPORT void setPosDown(int step = 1);
NAYK_EXPORT void setPosForward(int step = 1);
NAYK_EXPORT void setPosBack(int step = 1);
NAYK_EXPORT void toNextLine(int col = 1);
NAYK_EXPORT void toPrevLine(int col = 1);
NAYK_EXPORT void eraseLine();
NAYK_EXPORT void eraseLineLeft();
NAYK_EXPORT void eraseLineRight();
NAYK_EXPORT void savePos();
NAYK_EXPORT void restorePos();
NAYK_EXPORT void hideCursor();
NAYK_EXPORT void showCursor();
NAYK_EXPORT void setCursorVisible(bool visible = true);
NAYK_EXPORT void write(int row, int col, const QString &text);
NAYK_EXPORT void write(const QString &text);
NAYK_EXPORT QVariant read(const QVariant &defaultValue = QVariant());
NAYK_EXPORT void resetAttributes();
NAYK_EXPORT void setColors(Color textColor = ColorWhite, Color bgColor = ColorBlack);
NAYK_EXPORT void setTextColor(Color color = ColorWhite);
NAYK_EXPORT void setBgColor(Color color = ColorBlack);
NAYK_EXPORT void blink(bool on = true);
NAYK_EXPORT void frame(int row, int col, int width, int height, bool doubleBorder = false, Color color = ColorNone);
NAYK_EXPORT void rectangle(int row, int col, int width, int height, Color color = ColorNone);
NAYK_EXPORT void frame(const QRect &rect, bool doubleBorder = false, Color color = ColorNone);
NAYK_EXPORT void rectangle(const QRect &rect, Color color = ColorNone);
NAYK_EXPORT QSize getSize(const QSize &defaultSize = QSize(80, 25));
NAYK_EXPORT int width();
NAYK_EXPORT int height();

} // namespace console //=======================================================
#endif // CONSOLE_H
