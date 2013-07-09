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

#ifndef WIDGETS_H
#define WIDGETS_H

#include <QtGui>
//#include <QtWidgets>
class QTableView;
class QLineEdit;
class QAction;
class QDialog;
class QToolButton;

class QDbfLineEdit : public QLineEdit
{
    Q_OBJECT

public:
     QDbfLineEdit(const QString &contents, QWidget *parent = 0);
     ~QDbfLineEdit();

     virtual int valid();
     virtual void searchItem();

public slots:
     void search();

protected:
     void focusOutEvent(QFocusEvent *event);
     void keyPressEvent(QKeyEvent *);
     virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

class QNoEmptyLineEdit : public QDbfLineEdit
{
    Q_OBJECT

public:
     QNoEmptyLineEdit(bool show_message, const QString &contents, QWidget *parent = 0);
     ~QNoEmptyLineEdit();

     virtual int valid();
     virtual void searchItem();

private:
     bool showMessage;

};

class QDoubleLineEdit : public QDbfLineEdit
{
    Q_OBJECT

public:
     QDoubleLineEdit(const QString &contents, QWidget *parent = 0);
     ~QDoubleLineEdit();

     virtual int valid();
     virtual void searchItem();

};

class QNoZeroDoubleLineEdit : public QDbfLineEdit
{
    Q_OBJECT

public:
     QNoZeroDoubleLineEdit(const QString &contents, QWidget *parent = 0);
     ~QNoZeroDoubleLineEdit();

     virtual int valid();
     virtual void searchItem();

};

class QDateLineEdit : public QDbfLineEdit
{
    Q_OBJECT

public:
     QDateLineEdit(bool v, const QString &contents, QWidget *parent = 0);
     ~QDateLineEdit();

     virtual int valid();
     virtual void searchItem();

 private:
     bool verify;

};

class QDateTimeLineEdit : public QDbfLineEdit
{
    Q_OBJECT

public:
     QDateTimeLineEdit(bool v, const QString &contents, QWidget *parent = 0);
     ~QDateTimeLineEdit();

     virtual int valid();
     virtual void searchItem();

 private:
     bool verify;

};

class QDbfToolButton : public QToolButton
{
    Q_OBJECT

public:
    QDbfToolButton(QWidget *parent = 0);
    ~QDbfToolButton();

    QSize sizeHint() const;

public slots:
    void setIconSize1(int);
};

class QDbfTableView : public QTableView
{
    Q_OBJECT

signals:
    void editSignal();
    void insertSignal();
    void deleteSignal();
    void searchSignal();
    void quitSignal();

public:
    QDbfTableView(QWidget *parent = 0);

private:
    void keyPressEvent(QKeyEvent *);

};

class QDbfPushButton : public QPushButton
{
    Q_OBJECT

public:
    QDbfPushButton(const QIcon &, const QString & , QWidget *parent = 0);

    QSize sizeHint() const;
};

class QDbfDialog : public QDialog
{
    Q_OBJECT

public:
    QDbfDialog(const QString &title, QWidget *parent = 0);
    ~QDbfDialog();

signals:
    void endVerifyDbfInputLines();

public slots:
    void verifyDbfInputLines();

public:
    QList<QDbfLineEdit*> lineEditsToVerify;
    void insertLineEditToVerify(QDbfLineEdit*);

};

class QFieldColorLabel : public QLabel
{
    Q_OBJECT

public:
    QFieldColorLabel(QWidget *parent = 0);

signals:
    void changeColor();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

};

#endif
