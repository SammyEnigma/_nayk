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
#ifndef EX_CONTROLS_H
#define EX_CONTROLS_H

#include "NaykGlobal"
#include <QSpinBox>
#include <QKeyEvent>

namespace nayk { //=============================================================

//==============================================================================
class NAYK_EXPORT ExSpinBox: public QSpinBox
{
    Q_OBJECT
    Q_PROPERTY(int leadingZero READ leadingZero WRITE setLeadingZero NOTIFY leadingZeroChanged)

public:
    explicit ExSpinBox( QWidget * parent = nullptr);
    bool leadingZero() const;
    void setLeadingZero(bool lZero);
    virtual QString textFromValue ( int value ) const override;

signals:
    void returnPressed();
    void keyPressed(int key);
    void leadingZeroChanged(bool enabled);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    bool m_leadingZero {false};
};
//==============================================================================
class NAYK_EXPORT HexSpinBox : public QSpinBox
{
public:
    HexSpinBox( QWidget *parent = 0);
    unsigned int hexValue() const;
    void setHexValue(unsigned int value);

protected:
    QString textFromValue(int value) const;
    int valueFromText(const QString &text) const;
    QValidator::State validate(QString &input, int &pos) const;

private:

    inline unsigned int u(int i) const
    {
        return *reinterpret_cast<unsigned int *>(&i);
    }
    inline int i(unsigned int u) const
    {
        return *reinterpret_cast<int *>(&u);
    }
};
//==============================================================================

} // namespace nayk //==========================================================
#endif // EX_CONTROLS_H
