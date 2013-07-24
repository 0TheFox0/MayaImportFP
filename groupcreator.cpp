#include "groupcreator.h"

#include <QFile>
#include <QDate>
#include "Sqlcalls.h"
GroupCreator::GroupCreator(QObject *parent) :
    QThread(parent)
{
    QSqlDatabase db = QSqlDatabase::database("grupo");
    _host = db.hostName();
    _user = db.userName();
    _pass = db.password();
    _port = db.port();
    _full = true;
}

void GroupCreator::run()
{
    if(_full)
    {
        QSqlDatabase db = QSqlDatabase::database("grupo");
        bool error = false;
        if(!error)
            _createTables(error, db);

        if(!error)
        {
            QString grupo = "Grupo";
            grupo.append(_nombre);

            db.close();
            db.setHostName(_host);
            db.setUserName(_user);
            db.setPassword(_pass);
            db.setDatabaseName(grupo);
            if(_port!=0)
                db.setPort(_port);
            db.open();

            QSqlQuery q(db);

            _insertMonedas(q, error);

            _insertIVA(q, error);

            _insertNivelAcesso(error, q);

            _insertPoblaciones(error,q);

            _insertAdminUser(error, q);

            //TODO Reports por defecto

            if(!error)
            {
                _insertNewGroup(grupo);
            }

            if(!error)
            {
                _createEmpresa();
            }
        }
    }
    else
    {
        _createEmpresa();
    }
}

void GroupCreator::_createTables(bool& error, QSqlDatabase db)
{
    QFile f("://DB/DBGrupo.sql");
    f.open(QFile::ReadOnly);
    QString s = f.readAll();
    f.close();
    QString grupo = "Grupo";
    grupo.append(_nombre);
    s.replace("@grupo@",grupo);
    s.replace("\r\n"," ");
    QStringList querys = s.split(";",QString::SkipEmptyParts);
    int i;
    QSqlQuery q(db);
    emit sizeOfTask(querys.size());
    for(i = 0; i< querys.size();i++)
    {
        emit Progress("Creando tablas...",i);
        if(querys.at(i).size() > 10)
        {
            if(!q.exec(querys.at(i)))
            {
                emit Error(q.lastError().text());
                qDebug() << q.lastError().text();
                qDebug() << querys.at(i);
                error = true;
                break;
            }
        }
    }
}

void GroupCreator::_insertMonedas(QSqlQuery q, bool &error)
{
    QFile f("://DB/PaisesMonedas.sql");
    if(f.open((QIODevice::ReadOnly | QIODevice::Text)))
    {

        QTextStream in(&f);
        in.setCodec("UTF-8");
        QString all = in.readAll();
        QStringList l = all.split(";",QString::SkipEmptyParts);
        QStringListIterator it(l);
        while(it.hasNext())
        {
            QString line = it.next();
            if(!line.isEmpty())
                if(!q.exec(line))
                    qDebug() << q.lastError();
        }
    }
}

void GroupCreator::_insertIVA(QSqlQuery q, bool &error)
{
    emit sizeOfTask(4);
    emit Progress("Insertando IVA : 21%",1);
    error &= q.exec(QString("INSERT INTO `Grupo%1`.`tiposiva` (`nombre_interno`, `tipo`, `descripcion_tipo_iva`, `iva`, `recargo_equivalencia`) VALUES ('base1', 'NORMAL'       ,    '21%', 21.00, 5.20);").arg(_nombre));
    emit Progress("Insertando IVA : 10%",2);
    error &= q.exec(QString("INSERT INTO `Grupo%1`.`tiposiva` (`nombre_interno`, `tipo`, `descripcion_tipo_iva`, `iva`, `recargo_equivalencia`) VALUES ('base2', 'Reducido'     ,    '10%', 10.00, 1.40);").arg(_nombre));
    emit Progress("Insertando IVA : 4%",3);
    error &= q.exec(QString("INSERT INTO `Grupo%1`.`tiposiva` (`nombre_interno`, `tipo`, `descripcion_tipo_iva`, `iva`, `recargo_equivalencia`) VALUES ('base3', 'Superreducido',     '4%',  4.00, 0.50);").arg(_nombre));
    emit Progress("Insertando IVA : Exento",4);
    error &= q.exec(QString("INSERT INTO `Grupo%1`.`tiposiva` (`nombre_interno`, `tipo`, `descripcion_tipo_iva`, `iva`, `recargo_equivalencia`) VALUES ('base4', 'Exento'       , 'Exento',  0.00, 0.00);").arg(_nombre));
    qDebug() << q.lastError();
}

void GroupCreator::_insertNivelAcesso(bool &error, QSqlQuery q)
{
    emit sizeOfTask(7);
    emit Progress("Insertando nivel de acceso : Sin Acceso",1);
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (1,'Sin Acceso');");

    emit Progress("Insertando nivel de acceso : Lectura parcial",2);
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (2,'Lectura parcial');");

    emit Progress("Insertando nivel de acceso : Lectura total",3);
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (3,'Lectura total');");

    emit Progress("Insertando nivel de acceso : Escritura parcial (editar)",4);
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (4,'Escritura parcial (editar)');");

    emit Progress("Insertando nivel de acceso : Escritura parcial (añadir)",5);
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (5,'Escritura parcial (añadir)');");

    emit Progress("Insertando nivel de acceso : Escritural total",6);
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (6,'Escritural total');");

    emit Progress("Insertando nivel de acceso : Administrador",7);
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (7,'Administrador');");
}

void GroupCreator::_insertNewGroup(QString grupo)
{
    emit sizeOfTask(1);
    emit Progress("Insertando nuevo grupo",0);
    QSqlQuery q2(QSqlDatabase::database("Global"));
    int port = _port == 0 ? 3306 : _port;

    q2.prepare("INSERT INTO `mayaglobal`.`grupos` "
               "(`nombre`, `bd_name`, `bd_driver`, "
               "`bd_host`, `bd_user`, `bd_pass`, `bd_port`) "
               "VALUES "
               "(:nombre, :bd_name, :bd_driver, "
               ":bd_host, :bd_user, :bd_pass, :bd_port) ");

    q2.bindValue(":nombre",_nombre);
    q2.bindValue(":bd_name",grupo);
    q2.bindValue(":bd_driver","QMYSQL");
    q2.bindValue(":bd_host",_host);
    q2.bindValue(":bd_user",_user);
    q2.bindValue(":bd_pass",_pass);
    q2.bindValue(":bd_port",port);

    if(!q2.exec())
    {
        qDebug() << q2.lastError();
    }
    else
    {
        int id = q2.lastInsertId().toInt();
        q2.exec("SELECT * FROM mayaglobal.grupos where id="+QString::number(id));
        if(q2.first())
        {
            QSqlRecord r = q2.record();
            emit insertedGrupo(r);
        }
    }
    emit Progress("Hecho",1);
}

void GroupCreator::_insertAdminUser(bool &error, QSqlQuery q)
{
     error &= q.exec("INSERT INTO `usuarios` (`nombre`, `contrasena`, `nivel_acceso`, `categoria`) VALUES ('admin', 'jGl25bVBBBW96Qi9Te4V37Fnqchz/Eu4qB9vKrRIqRg=', '99', 'ADMINISTRADOR');");
}

void GroupCreator::_insertPoblaciones(bool &error, QSqlQuery q)
{
    QFile f("://DB/poblaciones.sql");
    if(f.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        QTextStream in(&f);
        in.setCodec("UTF-8");
        QString all = in.readAll();
        all.remove("\r");
        all.remove("\n");
        q.prepare(all);
        if(!q.exec())
        {
            error = true;
            emit Error(q.lastError().databaseText());
        }
    }
}

void GroupCreator::_createEmpresa()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","empresa");
    db.setHostName(_host);
    db.setUserName(_user);
    db.setPassword(_pass);
    if(_port!=0)
        db.setPort(_port);
    if(db.open())
    {
        bool error = false;

        QSqlQuery q(db);

        QString nombre_empresa = _empresaFp.value("CNOMBRE").toString().trimmed();
        QString n_empresa = _empresaFp.value("CNOMBRE").toString().trimmed();
        n_empresa = n_empresa.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));
        n_empresa = n_empresa.remove(QRegExp("[^a-zA-Z0-9\\d\\s]"));

        if(n_empresa.isEmpty())//Raro pero no se sabe
        {
            qsrand(QTime::currentTime().msec());
            n_empresa = QString::number(qrand()%999999);
        }

        QString nEmpresa = QString("emp%1").arg(n_empresa);

        QString nConta = QString("emp%1conta").arg(n_empresa);

        QString nMedic = QString("emp%1med").arg(n_empresa);

        QFile f("://DB/DBEmpresa.sql");
        error = !f.open(QFile::ReadOnly);
        if(!error)
        {
            QString s = f.readAll();
            s.replace("@empresa@",nEmpresa);
            s.replace("\r\n"," ");
            QStringList querys = s.split(";",QString::SkipEmptyParts);

            emit sizeOfTask(querys.size());

            for(int i = 0; i< querys.size();i++)
            {
                if(querys.at(i).size() > 10)
                {
                    if(!q.exec(querys.at(i)))
                    {
                        qDebug() << q.lastError().text();
                        qDebug() << querys.at(i);
                        error = true;
                        emit Error(q.lastError().databaseText());
                        break;
                    }
                    emit Progress(tr("Creando tablas de empresa"),i);
                }
            }
        }
        if(!error)
        {
            QFile f("://DB/DBConta.sql");
            error = !f.open(QFile::ReadOnly);
            if(!error)
            {
                QString s = f.readAll();
                s.replace("@conta@",nConta);
                s.replace("\r\n"," ");
                QStringList querys = s.split(";",QString::SkipEmptyParts);

                emit sizeOfTask(querys.size());

                for(int i = 0; i< querys.size();i++)
                {
                    if(querys.at(i).size() > 10)
                    {
                        if(!q.exec(querys.at(i)))
                        {
                            qDebug() << q.lastError().text();
                            qDebug() << querys.at(i);
                            error = true;
                            emit Error(q.lastError().databaseText());
                            break;
                        }
                        emit Progress(tr("Creando tablas de contabilidad"),i);
                    }
                }
            }
        }

        if(!error)
        {
            QHash<QString,QVariant> v;

            v["codigo"]=_empresaFp.value("CODEMP").toString().trimmed();
            v["nombre"]=nombre_empresa;
            v["digitos_factura"]=9;
            v["nombre_bd"]=nEmpresa;
            v["nombre_db_conta"]=nConta;
            v["nombre_bd_medica"]=nMedic;
            v["direccion"]=_empresaFp.value("CDOMICILIO").toString().trimmed();
            v["cp"]=_empresaFp.value("CCODPOS").toString().trimmed();
            v["poblacion"]=_empresaFp.value("CPOBLACION").toString().trimmed();
            v["provincia"]=_empresaFp.value("CPROVINCIA").toString().trimmed();
            v["pais"]=QString::fromUtf8("ESPAÑA");
            v["telefono1"]=_empresaFp.value("CTLF").toString().trimmed();
            v["fax"]=_empresaFp.value("CFAX").toString().trimmed();
            v["mail"]=_empresaFp.value("CEMAIL").toString().trimmed();
            v["cif"]=_empresaFp.value("CNIF").toString().trimmed();

            //if(!q2.exec())
            QString error;
            QString grupo = "Grupo";
            grupo.append(_nombre);
            QString table = "@grupo@`.`empresas";
            table = table.replace("@grupo@",grupo);
            if(SqlInsert(v,table,QSqlDatabase::database("grupo"),error)<0)
            {
                qDebug() << error;
                emit Error(q.lastError().text());
            }
            else
            {
                db.close();
                db.setHostName(_host);
                db.setUserName(_user);
                db.setPassword(_pass);
                if(_port!=0)
                    db.setPort(_port);
                db.setDatabaseName(nEmpresa);
                db.open();

                QSqlDatabase d = QSqlDatabase::addDatabase("QMYSQL","conta");
                d.setHostName(_host);
                d.setUserName(_user);
                d.setPassword(_pass);
                if(_port!=0)
                    d.setPort(_port);
                d.setDatabaseName(nConta);
                d.open();
            }
        }
    }
    else
        qDebug() << db.lastError().text();
}
