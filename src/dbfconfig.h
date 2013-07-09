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

#ifndef DBFCONFIG_H
#define DBFCONFIG_H

#include <QDialog>
#include <QSvgWidget>
#include <QSqlQueryModel>

class QDialogButtonBox;
class QFileInfo;
class QTabWidget;

#include "widgets.h"
#include "structures.h"

class QDetaliiBaseTab : public QWidget
{
    Q_OBJECT

public:
    QDetaliiBaseTab(QWidget *parent = 0);

    virtual void createInterface();
    virtual void saveEditedData();

};

class QConfigAppTab : public QDetaliiBaseTab
{
    Q_OBJECT

public:
    QConfigAppTab(QWidget *parent = 0);

    virtual void createInterface();
    virtual void saveEditedData();

private:

    QRadioButton *romanaRadioButton;
    QRadioButton *magyarRadioButton;
    QRadioButton *englishRadioButton;

    QPushButton *restoreDefaultButton;
};

class QIconSizeTab : public QDetaliiBaseTab
{
    Q_OBJECT

public:
    QIconSizeTab(QWidget *parent = 0);

    virtual void createInterface();
    virtual void saveEditedData();

public slots:
    void backToDefault();

private:
    QSpinBox *toolBarSpinBox;
    QSlider *toolBarSlider;
    QSpinBox *toolButtonSpinBox;
    QSlider *toolButtonSlider;

};

class QDbfStructureTab : public QDetaliiBaseTab
{
    Q_OBJECT

public:
    QDbfStructureTab(QString &tableName, QWidget *parent = 0);

    virtual void createInterface();

private:
    QSqlQueryModel *model;
    QTableView *view;

    QString strTableName;
};

class QDbfFieldColorsTab : public QDetaliiBaseTab
{
    Q_OBJECT

public:
    QDbfFieldColorsTab(QWidget *parent = 0);
    virtual void createInterface();

public slots:
    void charFieldColor();
    void currencyFieldColor();
    void numericFieldColor();
    void dateFieldColor();
    void timeFieldColor();
    void doubleFieldColor();
    void intFieldColor();
    void logicalFieldColor();
    void memoFieldColor();
    void memo4FieldColor();
    void generalFieldColor();

private:
    QLabel *charFieldColorLabel;
    QLabel *currencyFieldColorLabel;
    QLabel *numericFieldColorLabel;
    QLabel *dateFieldColorLabel;
    QLabel *timeFieldColorLabel;
    QLabel *doubleFieldColorLabel;
    QLabel *intFieldColorLabel;
    QLabel *logicalFieldColorLabel;
    QLabel *memoFieldColorLabel;
    QLabel *memo4FieldColorLabel;
    QLabel *generalFieldColorLabel;
};

class QDetaliiTabWidget: public QTabWidget
{
    Q_OBJECT

public:
    QDetaliiTabWidget(QWidget *parent = 0);

};

class QDetaliiTabDialog : public QDialog
{
    Q_OBJECT

public:
    QDetaliiTabDialog(QString &tableName, QWidget *parent = 0);
    ~QDetaliiTabDialog();

public slots:
    void changePicture(int i);

private slots:
    void saveConfiguration();
    void loadConfiguration();

private:
    QTabWidget *tabWidget;
    QSvgWidget *picture;

    QList<QDetaliiBaseTab *> detaliiTabList;
    QStringList pictures;

    int currentTab;
};

#endif
