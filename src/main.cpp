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
#include <QtSvg>
#include <QtSql>
#include "mainwindow.h"
#include "../importdialog.h"
MainWindow *mainWindow;

QStringList wordList;

QString charFieldColorString;
QString currencyFieldColorString;
QString numericFieldColorString;
QString dateFieldColorString;
QString timeFieldColorString;
QString doubleFieldColorString;
QString intFieldColorString;
QString logicalFieldColorString;
QString memoFieldColorString;
QString memo4FieldColorString;
QString generalFieldColorString;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qtDbf);

    QApplication app(argc, argv);

    QSettings settings("Hevele-Hegyi-Istvan", "qtDbf");
    QString dbfLocal = settings.value("dbflocal", "en").toString();

    QCoreApplication::setOrganizationName("Hevele-Hegyi-Istvan");
    QCoreApplication::setOrganizationDomain("hevele.juniorcom.ro");
    QCoreApplication::setApplicationName("qtDbf");

    QTranslator translator;
    QString dbfDirPath;

#ifdef UNIX
    dbfDirPath += "/usr/share/qtDbf/lang/qtDbf_";
    dbfDirPath += dbfLocal;
#else
    dbfDirPath = app.applicationDirPath();
    dbfDirPath += "/lang/qtDbf_";
    dbfDirPath += dbfLocal;
#endif

    translator.load(dbfDirPath);
    app.installTranslator(&translator);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "dbfEditor");
    db.setDatabaseName(":memory:");
    db.open();
    if (db.lastError().isValid())
    {
        QMessageBox::critical(0, "Eroare", db.lastError().text());
    }

    importDialog d;
    d.show();
    return app.exec();
    QSqlDatabase::removeDatabase("dbfEditor");
}

