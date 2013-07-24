#ifndef SQLCALLS_H
#define SQLCALLS_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
int SqlInsert(QHash<QString,QVariant> values, QString table, QSqlDatabase database, QString& error)
{
    if(!database.isOpen())
    {
        error = QObject::tr("Base de datos cerrada");
        return false;
    }
    /*
    if(database.driverName() == "QMYSQL")
    {
        qDebug()<< "Mysql";
    }
    else if (database.driverName() == "QSQLITE")
    {

    }*/
    QString colums;
    QTextStream s(&colums);

    QString data;
    QTextStream s1(&data);

    //INSERT INTO `world`.`cab_fac` (`serie`) VALUES ('a');
    s << "INSERT INTO `" << table << "` (";
    s1 << "VALUES (";
    QHashIterator<QString,QVariant> i(values);
    while (i.hasNext()) {
        i.next();
        s << "`" << i.key() << "`";
        s1 << ":" << i.key();
        if(i.hasNext())
        {
            s  << ",";
            s1 << ",";
        }
    }
    s1 << ");";
    s << ")" << data;

    QSqlQuery q(database);
    q.prepare(colums);

    QHashIterator<QString,QVariant> it(values);
    while (it.hasNext()) {
        it.next();
        QString aux = ":";
        aux.append(it.key());
        q.bindValue(aux,it.value());
    }


    bool b = q.exec();
    int r = -1;
    if (b)
        r = q.lastInsertId().toInt();
    else
    {
        error = q.lastError().text();
        qDebug() <<  colums;
    }
    return r;
}

bool SqlUpdate(QHash<QString,QVariant> values, QString table, QSqlDatabase database, QStringList clausulas, QString& error)
{
    if(!database.isOpen())
    {
        error = QObject::tr("Base de datos cerrada");
        return false;
    }
    /*
    if(database.driverName() == "QMYSQL")
    {
        qDebug()<< "Mysql";
    }
    else if (database.driverName() == "QSQLITE")
    {

    }*/
    //UPDATE `world`.`cab_fac` SET `serie`='b' WHERE `id`='9' and`id_cliente`='0';
    QString colums;
    QTextStream s(&colums);

    s << "UPDATE `" << table << "` SET ";

    QHashIterator<QString,QVariant> i(values);
    while (i.hasNext()) {
        i.next();
        s << "`" << i.key() << "`=:" << i.key();
        if(i.hasNext())
            s  << ",";
    }
    s << " WHERE ";
    QStringListIterator li(clausulas);
    while (li.hasNext()) {
        QString a = li.next();
        s << a;
        if(li.hasNext())
            s<< " AND ";
    }
    s << ";";

    QSqlQuery q(database);
    q.prepare(colums);

    QHashIterator<QString,QVariant> it(values);
    while (it.hasNext()) {
        it.next();
        QString aux = ":";
        aux.append(it.key());
        q.bindValue(aux,it.value());
    }

    bool b = q.exec();
    if (!b)
    {
        error = q.lastError().text();
        qDebug() <<  colums;
        qDebug() << q.boundValues();
    }
    return b;
}
bool SqlUpdate(QHash<QString,QVariant> values, QString table, QSqlDatabase database, QString clausula, QString& error)
{
    QStringList l;
    l << clausula;
    return SqlUpdate(values, table, database,l, error);
}
bool SqlDelete(QString table, QSqlDatabase database, QStringList clausulas, QString& error)
{
    //DELETE FROM `world`.`cab_fac` WHERE `id`='9' and`id_cliente`='0';
    if(!database.isOpen())
    {
        error = QObject::tr("Base de datos cerrada");
        return false;
    }
    /*
    if(database.driverName() == "QMYSQL")
    {
        qDebug()<< "Mysql";
    }
    else if (database.driverName() == "QSQLITE")
    {

    }*/
    QString colums;
    QTextStream s(&colums);

    s << "DELETE FROM `" << table << "` WHERE ";

    QStringListIterator li(clausulas);
    while (li.hasNext()) {
        QString a = li.next();
        s << a;
        if(li.hasNext())
            s<< " AND ";
    }
    s << ";";

    QSqlQuery q(database);
    q.prepare(colums);
    bool b = q.exec();
    if (!b)
        error = q.lastError().text();
    return b;
}
bool SqlDelete(QString table, QSqlDatabase database, QString clausula, QString& error)
{
    QStringList l;
    l<< clausula;
    return SqlDelete(table, database, l, error);
}

#endif // SQLCALLS_H
