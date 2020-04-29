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
#include "ex_controls.h"

namespace nayk { //=============================================================

//==============================================================================
// ExSpinBox
//==============================================================================
ExSpinBox::ExSpinBox(QWidget *parent) : QSpinBox(parent)
{

}
//==============================================================================
bool ExSpinBox::leadingZero() const
{
    return m_leadingZero;
}
//==============================================================================
void ExSpinBox::setLeadingZero(bool lZero)
{
    m_leadingZero = lZero;
    this->update();
    emit leadingZeroChanged(m_leadingZero);
}
//==============================================================================
QString ExSpinBox::textFromValue ( int value ) const
{
    if(m_leadingZero) {
        QString s = QString::number(maximum(), displayIntegerBase());
        return QString("%1").arg(value, s.length() , displayIntegerBase(), QChar('0')).toUpper();
    }
    else {
        return QString::number(value, displayIntegerBase()).toUpper();
    }
}
//==============================================================================
void ExSpinBox::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed( event->key() );
    if(event->key() == Qt::Key_Return) emit returnPressed();
    QSpinBox::keyPressEvent(event);
}
//==============================================================================



//==============================================================================
// HexSpinBox
//==============================================================================
HexSpinBox::HexSpinBox( QWidget *parent) : QSpinBox(parent)
{
    setPrefix("0x");
    setDisplayIntegerBase(16);
    setRange(INT_MIN, INT_MAX);
}
//==============================================================================
unsigned int HexSpinBox::hexValue() const
{
    return u(value());
}
//==============================================================================
void HexSpinBox::setHexValue(unsigned int value)
{
    setValue(i(value));
}
//==============================================================================
QString HexSpinBox::textFromValue(int value) const
{
    QString s = QString::number(u(value), 16).toUpper();
    return s.rightJustified(8, '0', true);
}
//==============================================================================
int HexSpinBox::valueFromText(const QString &text) const
{
    return i(text.toUInt(0, 16));
}
//==============================================================================
QValidator::State HexSpinBox::validate(QString &input, int &pos) const
{
    QString copy(input);
    if (copy.startsWith("0x")) copy.remove(0, 2);
    pos -= copy.size() - copy.trimmed().size();
    copy = copy.trimmed();
    if (copy.isEmpty())
        return QValidator::Intermediate;
    if (copy.length() > 8) copy = copy.left(8);
    input = QString("0x") + copy.toUpper();
    bool okay;
    unsigned int val = copy.toUInt(&okay, 16);
    Q_UNUSED(val)
    if (!okay) return QValidator::Invalid;
    return QValidator::Acceptable;
}
//==============================================================================

} // namespace nayk //==========================================================
