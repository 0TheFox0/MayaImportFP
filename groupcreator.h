#ifndef GROUPCREATOR_H
#define GROUPCREATOR_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
class GroupCreator : public QThread
{
    Q_OBJECT
public:
    explicit GroupCreator(QObject *parent = 0);
    void run();

    QString _nombre;
    QString _empresa;
    QString _host;
    QString _user;
    QString _pass;
    int _port;

    QSqlRecord _empresaFp;
    bool _full;
signals:
    void Error(QString s);
    void Progress(QString message, int currentValue);
    void sizeOfTask(int max);
    void insertedGrupo(QSqlRecord);
public slots:
private:
    void _createTables(bool error, QSqlDatabase db);
    void _insertMonedas(QSqlQuery q, bool error);
    void _insertIVA(QSqlQuery q, bool error);
    void _insertNivelAcesso(bool error, QSqlQuery q);
    void _insertNewGroup(QString grupo);
    void _insertAdminUser(bool error, QSqlQuery q);
    void _insertPoblaciones(bool error, QSqlQuery q);
    void _createEmpresa();
};

#endif // GROUPCREATOR_H
