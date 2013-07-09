/****************************************************************************
**
** Copyright (C) 2009 Hevele Hegyi Istvan.
**
** This file is part of the qtDbf project.
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtSql>
#include <QtSvg>

#include "widgets.h"
#include "structures.h"
#include "dbfconfig.h"
#include "mainwindow.h"
#include "globals.h"

extern MainWindow *mainWindow;

//QDetaliiTabWidget

QDetaliiTabWidget::QDetaliiTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
}

//QDetaliiTabDialog

QDetaliiTabDialog::QDetaliiTabDialog(QString &tableName, QWidget *parent)
    : QDialog(parent)
{
    QSettings settings;
    QSize size = settings.value("dbfdetalii/Size", QSize(870, 480)).toSize();
    resize(size);
    int  currentTab = settings.value("dbfdetalii/currenttab", 1).toInt();

    QHBoxLayout *mainLayout = new QHBoxLayout;

    picture = new QSvgWidget(":images/config.svg", this);
    picture->setFixedSize(QSize(128,128));

    QVBoxLayout *svgLayout = new QVBoxLayout;
    svgLayout->addWidget(picture);
    svgLayout->addStretch(1);

    detaliiTabList.clear();

    tabWidget = new QDetaliiTabWidget;

    int i = 0;
    QConfigAppTab *configAppTab = new QConfigAppTab;
    detaliiTabList.append(configAppTab);
    tabWidget->addTab(detaliiTabList.at(i), tr("Application language"));
    pictures.append("config.svg");
    i++;

    QIconSizeTab *iconSizeTab = new QIconSizeTab;
    detaliiTabList.append(iconSizeTab);
    tabWidget->addTab(detaliiTabList.at(i), tr("Icon sizes"));
    pictures.append("tux.svg");
    i++;

    QDbfStructureTab *dbfStructureTab = new QDbfStructureTab(tableName);
    detaliiTabList.append(dbfStructureTab);
    tabWidget->addTab(detaliiTabList.at(i), tr("DBF file structure"));
    pictures.append("structure.svg");
    i++;

    QDbfFieldColorsTab *dbfFieldColorsTab = new QDbfFieldColorsTab;
    detaliiTabList.append(dbfFieldColorsTab);
    tabWidget->addTab(detaliiTabList.at(i), tr("Field type's colors"));
    pictures.append("colors.svg");
    i++;

    QPushButton *saveButton = new QPushButton(QIcon(":images/save.svg"),tr("Save configuration"), this);
    QPushButton *loadButton = new QPushButton(QIcon(":images/load.svg"),tr("Load configuration"), this);
    QPushButton *okButton = new QPushButton(QIcon(":images/ok.svg"),tr("OK"), this);
    QPushButton *cancelButton = new QPushButton(QIcon(":images/cancel.svg"),tr("Cancel"), this);

    okButton->setDefault(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveConfiguration()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadConfiguration()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QVBoxLayout *tabLayout = new QVBoxLayout;

    tabLayout->addWidget(tabWidget);
    tabLayout->addLayout(buttonLayout);

    mainLayout->addLayout(svgLayout);
    mainLayout->addLayout(tabLayout);
    setLayout(mainLayout);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changePicture(int)));

    setWindowTitle(tr("Configure application"));

    tabWidget->setCurrentIndex(currentTab);
    changePicture(currentTab);
}

void QDetaliiTabDialog::changePicture(int i)
{
    QString iconfilename = ":images/"+pictures.at(i);
    picture->load(iconfilename);
}

QDetaliiTabDialog::~QDetaliiTabDialog()
{
    for (int i=0;i<detaliiTabList.count();i++)
        {
            detaliiTabList.at(i)->saveEditedData();
        }

    QSettings settings;
    currentTab = tabWidget->currentIndex();

    settings.setValue("dbfdetalii/currenttab", currentTab);
    settings.setValue("dbfdetalii/Size", size());
}

//QDetaliiBaseTab

QDetaliiBaseTab::QDetaliiBaseTab(QWidget *parent)
    : QWidget(parent)
{
    createInterface();
}

void QDetaliiBaseTab::createInterface()
{
}

void QDetaliiBaseTab::saveEditedData()
{
}

//QConfigAppTab

QConfigAppTab::QConfigAppTab(QWidget *parent)
    : QDetaliiBaseTab(parent)
{
    createInterface();
}

void QConfigAppTab::createInterface()
{
        QSettings settings;


        QString dbfLocal = settings.value("dbflocal", "ro").toString();

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QGroupBox *languageGroupBox = new QGroupBox(tr("Application language"), this);

        QGridLayout *languageLayout = new QGridLayout(languageGroupBox);

        romanaRadioButton = new QRadioButton(tr("Romana"),languageGroupBox);
        magyarRadioButton = new QRadioButton(tr("Magyar"),languageGroupBox);
        englishRadioButton = new QRadioButton(tr("English"),languageGroupBox);

        QLabel *romanaLabel = new QLabel(languageGroupBox);
        QLabel *magyarLabel = new QLabel(languageGroupBox);
        QLabel *englishLabel = new QLabel(languageGroupBox);

        QPixmap romanaSteag;
        QPixmap magyarZaszlo;
        QPixmap englishFlag;

        romanaSteag.load(":images/romana.png");
        magyarZaszlo.load(":images/magyar.png");
        englishFlag.load(":images/english.png");

        romanaLabel->setPixmap(romanaSteag);
        magyarLabel->setPixmap(magyarZaszlo);
        englishLabel->setPixmap(englishFlag);

        if (dbfLocal == "ro")
            {
                romanaRadioButton->setChecked(true);
                magyarRadioButton->setChecked(false);
                englishRadioButton->setChecked(false);
            }

        if (dbfLocal == "hu")
            {
                romanaRadioButton->setChecked(false);
                magyarRadioButton->setChecked(true);
                englishRadioButton->setChecked(false);
            }

        if (dbfLocal == "en")
            {
                romanaRadioButton->setChecked(false);
                magyarRadioButton->setChecked(false);
                englishRadioButton->setChecked(true);
            }

        QLabel *explicLanguage = new QLabel(tr("Requires application restart"),languageGroupBox);
        explicLanguage->setWordWrap(true);

        languageLayout->addWidget(romanaRadioButton,0,0);
        languageLayout->addWidget(magyarRadioButton,1,0);
        languageLayout->addWidget(englishRadioButton,2,0);
        languageLayout->addWidget(romanaLabel,0,1);
        languageLayout->addWidget(magyarLabel,1,1);
        languageLayout->addWidget(englishLabel,2,1);

        languageLayout->addWidget(explicLanguage,4,0,1,2);

        mainLayout->addWidget(languageGroupBox);

}

void QConfigAppTab::saveEditedData()
{
    QSettings settings;
    QString dbfLocal;

    if (romanaRadioButton->isChecked())
        dbfLocal="ro";
    if (magyarRadioButton->isChecked())
        dbfLocal="hu";
    if (englishRadioButton->isChecked())
        dbfLocal="en";

    settings.setValue("dbflocal", dbfLocal);
}

//QIconSizeTab

QIconSizeTab::QIconSizeTab(QWidget *parent)
    : QDetaliiBaseTab(parent)
{
    createInterface();
}

void QIconSizeTab::createInterface()
{
    QSettings settings;

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);

    QGroupBox *toolButtonGroupBox = new QGroupBox(tr("Tool button icon size"),this);
    QVBoxLayout *toolButtonLayout = new QVBoxLayout(toolButtonGroupBox);

    QSize toolButtonSize = settings.value("toolbuttonsize", QSize(37,37)).toSize();

    toolButtonSpinBox = new QSpinBox(this);
    toolButtonSpinBox->setMinimum(30);
    toolButtonSpinBox->setMaximum(100);
    toolButtonSpinBox->setValue(toolButtonSize.width());

    toolButtonSlider = new QSlider(this);
    toolButtonSlider->setOrientation(Qt::Horizontal);
    toolButtonSlider->setMinimum(30);
    toolButtonSlider->setMaximum(100);
    toolButtonSlider->setValue(toolButtonSize.width());

    toolButtonLayout->addWidget(toolButtonSpinBox);
    toolButtonLayout->addWidget(toolButtonSlider);

    QDbfToolButton *toolButton = new QDbfToolButton(this);
    toolButton->setIcon(QIcon(":images/edit.svg"));
    toolButtonLayout->addWidget(toolButton);

    verticalLayout->addWidget(toolButtonGroupBox);

    connect(toolButtonSpinBox, SIGNAL(valueChanged(int)), toolButton, SLOT(setIconSize1(int)));
    connect(toolButtonSpinBox, SIGNAL(valueChanged(int)), mainWindow, SIGNAL(setToolButtonIconSize(int)));

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);

    connect(toolButtonSpinBox, SIGNAL(valueChanged(int)), toolButtonSlider, SLOT(setValue(int)));
    connect(toolButtonSlider, SIGNAL(valueChanged(int)), toolButtonSpinBox, SLOT(setValue(int)));

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    QPushButton *restoreDefaultButton = new QPushButton(QIcon(":images/back.svg"),tr("Default"), this);

    horizontalLayout->addWidget(restoreDefaultButton);

    verticalLayout->addLayout(horizontalLayout);

    connect(restoreDefaultButton, SIGNAL(clicked()), this, SLOT(backToDefault()));
}

void QIconSizeTab::saveEditedData()
{
    QSettings settings;
    QSize toolButtonSize;
    toolButtonSize.setHeight(toolButtonSpinBox->value());
    toolButtonSize.setWidth(toolButtonSpinBox->value());
    settings.setValue("toolbuttonsize", toolButtonSize);
}

void QIconSizeTab::backToDefault()
{
    toolButtonSpinBox->setValue(37);
}

//QDbfStructureTab

QDbfStructureTab::QDbfStructureTab(QString &tableName, QWidget *parent)
    : QDetaliiBaseTab(parent)
{
    strTableName = tableName;
    createInterface();
}

void QDbfStructureTab::createInterface()
{
    QHBoxLayout *mainLayout;
    QString query;

    query = "SELECT * FROM ";
    query += strTableName;

    model = new QSqlQueryModel(this);
    model->setQuery(query, QSqlDatabase::database("dbfEditor"));
    if (model->lastError().isValid())
       QMessageBox::critical(this, tr("Eroare"), model->lastError().text());

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Size"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Decimals"));

    view = new QTableView(this);
    view->setModel(model);

    view->hideColumn(0);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setAlternatingRowColors(true);
    view->resizeColumnsToContents();
    view->selectColumn(0);
    view->selectRow(0);

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    setLayout(mainLayout);

    view->setFocus();

}

//QDbfFieldColorsTab

QDbfFieldColorsTab::QDbfFieldColorsTab(QWidget *parent)
    : QDetaliiBaseTab(parent)
{
    createInterface();
}

void QDbfFieldColorsTab::createInterface()
{
    QFont font;
    QHBoxLayout *mainLayout;

    QGroupBox *colorButtonGroupBox = new QGroupBox(tr("Double click on the name to change color"),this);
    QHBoxLayout *colorButtonLayout = new QHBoxLayout(colorButtonGroupBox);
    QVBoxLayout *colors1Layout = new QVBoxLayout;
    QVBoxLayout *colors2Layout = new QVBoxLayout;

    charFieldColorLabel = new QFieldColorLabel(this);
    charFieldColorLabel->setText("<font color='#"+charFieldColorString+"'>"+tr("Character</font>"));
    currencyFieldColorLabel = new QFieldColorLabel(this);
    currencyFieldColorLabel->setText("<font color='#"+currencyFieldColorString+"'>"+tr("Currency</font>"));
    numericFieldColorLabel = new QFieldColorLabel(this);
    numericFieldColorLabel->setText("<font color='#"+numericFieldColorString+"'>"+tr("Numeric</font>"));
    dateFieldColorLabel = new QFieldColorLabel(this);
    dateFieldColorLabel->setText("<font color='#"+dateFieldColorString+"'>"+tr("Date</font>"));
    timeFieldColorLabel = new QFieldColorLabel(this);
    timeFieldColorLabel->setText("<font color='#"+timeFieldColorString+"'>"+tr("DateTime</font>"));
    doubleFieldColorLabel = new QFieldColorLabel(this);
    doubleFieldColorLabel->setText("<font color='#"+doubleFieldColorString+"'>"+tr("Double</font>"));
    intFieldColorLabel = new QFieldColorLabel(this);
    intFieldColorLabel->setText("<font color='#"+intFieldColorString+"'>"+tr("Integer</font>"));
    logicalFieldColorLabel = new QFieldColorLabel(this);
    logicalFieldColorLabel->setText("<font color='#"+logicalFieldColorString+"'>"+tr("Logical</font>"));
    memoFieldColorLabel = new QFieldColorLabel(this);
    memoFieldColorLabel->setText("<font color='#"+memoFieldColorString+"'>"+tr("Memo</font>"));
    memo4FieldColorLabel = new QFieldColorLabel(this);
    memo4FieldColorLabel->setText("<font color='#"+memo4FieldColorString+"'>"+tr("Memo4</font>"));
    generalFieldColorLabel = new QFieldColorLabel(this);
    generalFieldColorLabel->setText("<font color='#"+generalFieldColorString+"'>"+tr("General</font>"));

    connect(charFieldColorLabel, SIGNAL(changeColor()), this, SLOT(charFieldColor()));
    connect(currencyFieldColorLabel, SIGNAL(changeColor()), this, SLOT(currencyFieldColor()));
    connect(numericFieldColorLabel, SIGNAL(changeColor()), this, SLOT(numericFieldColor()));
    connect(dateFieldColorLabel, SIGNAL(changeColor()), this, SLOT(dateFieldColor()));
    connect(timeFieldColorLabel, SIGNAL(changeColor()), this, SLOT(timeFieldColor()));
    connect(doubleFieldColorLabel, SIGNAL(changeColor()), this, SLOT(doubleFieldColor()));
    connect(intFieldColorLabel, SIGNAL(changeColor()), this, SLOT(intFieldColor()));
    connect(logicalFieldColorLabel, SIGNAL(changeColor()), this, SLOT(logicalFieldColor()));
    connect(memoFieldColorLabel, SIGNAL(changeColor()), this, SLOT(memoFieldColor()));
    connect(memo4FieldColorLabel, SIGNAL(changeColor()), this, SLOT(memo4FieldColor()));
    connect(generalFieldColorLabel, SIGNAL(changeColor()), this, SLOT(generalFieldColor()));


    colors1Layout->addWidget(charFieldColorLabel);
    colors1Layout->addWidget(currencyFieldColorLabel);
    colors1Layout->addWidget(numericFieldColorLabel);
    colors1Layout->addWidget(dateFieldColorLabel);
    colors1Layout->addWidget(timeFieldColorLabel);
    colors1Layout->addWidget(doubleFieldColorLabel);
    colors1Layout->addWidget(intFieldColorLabel);
    colors1Layout->addWidget(logicalFieldColorLabel);
    colors1Layout->addWidget(memoFieldColorLabel);
    colors1Layout->addWidget(memo4FieldColorLabel);
    colors1Layout->addWidget(generalFieldColorLabel);
    colorButtonLayout->addLayout(colors1Layout);
    //colorButtonLayout->addLayout(colors2Layout);
    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(colorButtonGroupBox);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

}

void QDbfFieldColorsTab::charFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(charFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    charFieldColorString = tempByteArray.toHex().toUpper();
                    charFieldColorLabel->setText("<font color='#"+charFieldColorString+"'>"+tr("Character</font>"));
                }
        }
}

void QDbfFieldColorsTab::currencyFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(currencyFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    currencyFieldColorString = tempByteArray.toHex().toUpper();
                    currencyFieldColorLabel->setText("<font color='#"+currencyFieldColorString+"'>"+tr("Currency</font>"));
                }
        }
}

void QDbfFieldColorsTab::numericFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(numericFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    numericFieldColorString = tempByteArray.toHex().toUpper();
                    numericFieldColorLabel->setText("<font color='#"+numericFieldColorString+"'>"+tr("Numeric</font>"));
                }
        }
}

void QDbfFieldColorsTab::dateFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(dateFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    dateFieldColorString = tempByteArray.toHex().toUpper();
                    dateFieldColorLabel->setText("<font color='#"+dateFieldColorString+"'>"+tr("Date</font>"));
                }
        }
}

void QDbfFieldColorsTab::timeFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(timeFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    timeFieldColorString = tempByteArray.toHex().toUpper();
                    timeFieldColorLabel->setText("<font color='#"+timeFieldColorString+"'>"+tr("DateTime</font>"));
                }
        }
}

void QDbfFieldColorsTab::doubleFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(doubleFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    doubleFieldColorString = tempByteArray.toHex().toUpper();
                    doubleFieldColorLabel->setText("<font color='#"+doubleFieldColorString+"'>"+tr("Double</font>"));
                }
        }
}

void QDbfFieldColorsTab::intFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(intFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    intFieldColorString = tempByteArray.toHex().toUpper();
                    intFieldColorLabel->setText("<font color='#"+intFieldColorString+"'>"+tr("Integer</font>"));
                }
        }
}

void QDbfFieldColorsTab::logicalFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(logicalFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    logicalFieldColorString = tempByteArray.toHex().toUpper();
                    logicalFieldColorLabel->setText("<font color='#"+logicalFieldColorString+"'>"+tr("Logical</font>"));
                }
        }
}

void QDbfFieldColorsTab::memoFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(memoFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    memoFieldColorString = tempByteArray.toHex().toUpper();
                    memoFieldColorLabel->setText("<font color='#"+memoFieldColorString+"'>"+tr("Memo</font>"));
                }
        }
}

void QDbfFieldColorsTab::memo4FieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(memo4FieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    memo4FieldColorString = tempByteArray.toHex().toUpper();
                    memo4FieldColorLabel->setText("<font color='#"+memo4FieldColorString+"'>"+tr("Memo4</font>"));
                }
        }
}

void QDbfFieldColorsTab::generalFieldColor()
{
    QSettings settings;
    quint8 r,g,b;
    QByteArray tempByteArray;
    tempByteArray.append(generalFieldColorString);
    tempByteArray = QByteArray::fromHex(tempByteArray);
    r = tempByteArray.at(0);
    g = tempByteArray.at(1);
    b = tempByteArray.at(2);
    QColor tempColor(r,g,b);
    QColorDialog *colorDialog = new QColorDialog(tempColor, this);
    if (colorDialog->exec())
        {
            if (colorDialog->result() == QDialog::Accepted)
                {
                    tempByteArray.clear();
                    tempByteArray.append(colorDialog->selectedColor().red());
                    tempByteArray.append(colorDialog->selectedColor().green());
                    tempByteArray.append(colorDialog->selectedColor().blue());
                    generalFieldColorString = tempByteArray.toHex().toUpper();
                    generalFieldColorLabel->setText("<font color='#"+generalFieldColorString+"'>"+tr("General</font>"));
                }
        }
}
