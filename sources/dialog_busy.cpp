/****************************************************************************
** Copyright (c) 2020 Evgeny Teterin (nayk) <sutcedortal@gmail.com>
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
#include <QApplication>
#include <QIcon>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "ImagesConst"
#include "GuiUtils"
#include "dialog_busy.h"

namespace nayk { //=============================================================

//==============================================================================
void showDialogBusy(const QString &text, QWidget *parent)
{
    if(!parent)
        parent = gui_utils::mainWindowWidget();

    if(parent) {

        if(!parent->findChildren<DialogBusy*>().isEmpty())
            return;
    }
    else {
        QWidgetList list = QApplication::topLevelWidgets();

        for ( QWidget *widget : list ) {
            if ( QString(widget->metaObject()->className()) == QString("DialogBusy"))
                return;
        }
    }

    DialogBusy *dlg = new DialogBusy(parent);
    dlg->setObjectName("dialogBusy");

    if(!text.isEmpty())
        dlg->setText(text);

    dlg->show();
    QApplication::processEvents();
}
//==============================================================================
void hideDialogBusy()
{
    QWidget *parent = gui_utils::mainWindowWidget();

    if(parent) {

        QList<DialogBusy*> list = parent->findChildren<DialogBusy*>();

        if(list.isEmpty())
            return;

        DialogBusy *dlg = list.first();
        if(dlg)
            dlg->close();
    }
    else {
        QWidgetList list = QApplication::topLevelWidgets();

        for ( QWidget *widget : list ) {
            if ( QString(widget->metaObject()->className()) == QString("DialogBusy")) {

                DialogBusy *dlg = qobject_cast<DialogBusy *>(widget);
                if(dlg)
                    dlg->close();
                return;
            }
        }
    }
}
//==============================================================================

//==============================================================================
DialogBusy::DialogBusy(QWidget *parent) : QDialog(parent)
{
    initializeDialog();
}
//==============================================================================
DialogBusy::~DialogBusy()
{

}
//==============================================================================
void DialogBusy::setProgressMaximum(int value)
{
    QList<QProgressBar*> list = this->findChildren<QProgressBar*>();

    if(!list.isEmpty())
        list.first()->setMaximum(value);
}
//==============================================================================
void DialogBusy::setProgressVisible(bool visible)
{
    QList<QProgressBar*> list = this->findChildren<QProgressBar*>();

    if(!list.isEmpty())
        list.first()->setVisible(visible);
}
//==============================================================================
void DialogBusy::setCancelButtonVisible(bool visible)
{
    QList<QPushButton*> list = this->findChildren<QPushButton*>();

    if(!list.isEmpty())
        list.first()->setVisible(visible);
}
//==============================================================================
void DialogBusy::initializeDialog()
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags( Qt::Dialog
                    | Qt::WindowTitleHint
                    | Qt::WindowStaysOnTopHint
                    | Qt::CustomizeWindowHint );
    setWindowTitle(tr("Please wait..."));

    if(QFile::exists(iconHourglass)) {
        setWindowIcon( QIcon(iconHourglass) );
    }

    setMinimumSize( 400, 120 );
    setMaximumSize( 420, 180 );

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setObjectName("mainLayout");
    layout->setMargin(20);
    layout->setSpacing(20);

    QLabel *labelText = new QLabel(tr("Please wait..."), this);
    labelText->setObjectName("labelText");
    labelText->setMaximumHeight(50);
    labelText->setMaximumHeight(60);
    labelText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelText->sizePolicy().setHorizontalPolicy( QSizePolicy::Preferred );
    labelText->sizePolicy().setVerticalPolicy( QSizePolicy::Expanding );

    if(QFile::exists(iconHourglass)) {

        setWindowIcon( QIcon(iconHourglass) );

        QLabel *labelIcon = new QLabel(this);
        labelIcon->setObjectName("labelIcon");
        labelIcon->setMinimumSize( 50, 50 );
        labelIcon->setMaximumSize( 60, 60 );
        labelIcon->setAlignment( Qt::AlignCenter );
        labelIcon->setPixmap( QPixmap(iconHourglass) );
        labelIcon->sizePolicy().setHorizontalPolicy( QSizePolicy::Preferred );
        labelIcon->sizePolicy().setVerticalPolicy( QSizePolicy::Expanding );

        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->setObjectName("labelsLayout");
        hLayout->setMargin(20);
        hLayout->setSpacing(20);
        hLayout->addWidget( labelIcon );
        hLayout->addWidget( labelText );

        layout->addLayout(hLayout);
    }
    else {
        layout->addWidget(labelText);
    }

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setObjectName("bottomLayout");
    hLayout->setMargin(20);
    hLayout->setSpacing(10);

    QProgressBar *progress = new QProgressBar(this);
    progress->setObjectName("progressBar");
    progress->setTextVisible(false);
    progress->setMinimum(0);
    progress->setMaximum(0);
    progress->sizePolicy().setHorizontalPolicy( QSizePolicy::Expanding );

    QPushButton *button = new QPushButton(tr("Cancel"), this);
    button->setObjectName("button");
    connect(button, &QPushButton::clicked, this, &DialogBusy::cancelButtonClicked);

    hLayout->addWidget( progress );
    hLayout->addWidget( button );

    layout->addLayout(hLayout);
    setLayout(layout);

    button->setVisible(false);
}
//==============================================================================
void DialogBusy::setProgressValue(int value)
{
    QList<QProgressBar*> list = this->findChildren<QProgressBar*>();

    if(!list.isEmpty())
        list.first()->setValue(value);
}
//==============================================================================
void DialogBusy::addProgressValue(int value)
{
    QList<QProgressBar*> list = this->findChildren<QProgressBar*>();

    if(!list.isEmpty())
        list.first()->setValue( value + list.first()->value() );
}
//==============================================================================
void DialogBusy::setText(const QString &text)
{
    QList<QLabel*> list = this->findChildren<QLabel*>("labelText");

    if(!list.isEmpty())
        list.first()->setText(text);
}
//==============================================================================

} // namespace nayk //==========================================================
