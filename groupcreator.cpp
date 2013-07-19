#include "groupcreator.h"

#include <QFile>
#include <QDate>
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

void GroupCreator::_createTables(bool error, QSqlDatabase db)
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

void GroupCreator::_insertMonedas(QSqlQuery q, bool error)
{
    QFile f("://DB/divisas.txt");
    if(f.open((QIODevice::ReadOnly | QIODevice::Text)))
    {

        QTextStream in(&f);
        in.setCodec("UTF-8");
        QString all = in.readAll();
        all.remove("\r");
        all.remove("\n");
        QStringList blocks = all.split("},",QString::SkipEmptyParts);
        int i = 0;
        emit sizeOfTask(blocks.size());
        foreach(const QString &moneda , blocks)
        {

            QStringList subBlocks = moneda.split("\",",QString::SkipEmptyParts);
            QString nombre_corto = subBlocks.at(0).mid(subBlocks.at(0).lastIndexOf("\"")+1);
            QString nombre = subBlocks.at(1).mid(subBlocks.at(1).lastIndexOf("\"")+1);
            QString simbol = subBlocks.at(4).mid(subBlocks.at(4).lastIndexOf("\"")+1);
            simbol.remove("}");
            simbol = simbol.trimmed();
            q.prepare("INSERT INTO monedas ("
                      "moneda ,"
                      "nombre_corto ,"
                      "simbolo "
                      ")"
                      "VALUES ("
                      ":moneda, :nombre_corto, :simbolo );");
            q.bindValue(":moneda",nombre);
            emit Progress("Insertando divisa "+nombre,i);
            i++;
            q.bindValue(":nombre_corto",nombre_corto);
            q.bindValue(":simbolo",simbol);
            if(!q.exec())
            {
                emit Error(q.lastError().text());
                error = true;
            }
        }
    }
}

void GroupCreator::_insertIVA(QSqlQuery q, bool error)
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

void GroupCreator::_insertNivelAcesso(bool error, QSqlQuery q)
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

void GroupCreator::_insertAdminUser(bool error, QSqlQuery q)
{
     error &= q.exec("INSERT INTO `usuarios` (`nombre`, `contrasena`, `nivel_acceso`, `categoria`) VALUES ('admin', 'jGl25bVBBBW96Qi9Te4V37Fnqchz/Eu4qB9vKrRIqRg=', '99', 'ADMINISTRADOR');");
}

void GroupCreator::_insertPoblaciones(bool error, QSqlQuery q)
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
            error = true;
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

        QString n_empresa = _empresaFp.value("CNOMBRE").toString().trimmed();

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
                        break;
                    }
                    emit Progress(tr("Creando tablas"),i);
                }
            }
        }

        if(!error)
        { /*
            QString query = QString(
                        "INSERT INTO `@grupo@`.`empresas` "
                        "(`codigo`, `nombre`, `digitos_factura`, `serie`, `nombre_bd`, `nombre_db_conta`,"
                        "`nombre_bd_medica`, `direccion`, `cp`, `poblacion`, `provincia`, `pais`,"
                        "`telefono1`, `telefono2`, `fax`, `mail`, `web`, `cif`, `inscripcion`,"
                        "`comentario_albaran`, `comentario_factura`, `comentario_ticket`,"
                        "`ejercicio`, `usar_irpf`, `codigo_cuenta_clientes`,"
                        "`codigo_cuenta_proveedores`, `codigo_cuenta_acreedores`,"
                        "`digitos_cuenta`, `clave1`, `clave2`, `medica`, `internacional`,"
                        "`auto_codigo`, `tamanocodigo`, `cuenta_cobros`, `cuenta_pagos`,"
                        "`id_divisa`, `enlaceweb`, `contabilidad`, `consultas`, `primer_dia_laborable`,"
                        "`ultimo_dia_laborable`, `horario_primer_dia`, `horario_dia_normal`,"
                        "`horario_ultimo_dia`, `ticket_factura`, `margen`, `margen_minimo`, `seguimiento`,"
                        "`id_tarifa_predeterminada`, `actualizardivisas`, `cuenta_ventas_mercaderias`,"
                        "`cuenta_ventas_servicios`, `cuenta_iva_soportado1`, `cuenta_iva_soportado2`,"
                        "`cuenta_iva_soportado3`, `cuenta_iva_soportado4`, `cuenta_iva_repercutido1`,"
                        "`cuenta_iva_repercutido2`, `cuenta_iva_repercutido3`, `cuenta_iva_repercutido4`,"
                        "`cuenta_iva_repercutido1_re`, `cuenta_iva_repercutido2_re`, `cuenta_iva_repercutido3_re`,"
                        "`cuenta_iva_repercutido4_re`, `cuenta_iva_soportado1_re`, `cuenta_iva_soportado2_re`,"
                        "`cuenta_iva_soportado3_re`, `cuenta_iva_soportado4_re`) "
                        "VALUES "
                        "(:codigo, :nombre, :digitos_factura, :serie, :nombre_bd, :nombre_db_conta,"
                        ":nombre_bd_medica, :direccion, :cp, :poblacion, :provincia, :pais,"
                        ":telefono1, :telefono2, :fax, :mail, :web, :cif, :inscripcion,"
                        ":comentario_albaran, :comentario_factura, :comentario_ticket,"
                        ":ejercicio, :usar_irpf, :codigo_cuenta_clientes,"
                        ":codigo_cuenta_proveedores, :codigo_cuenta_acreedores,"
                        ":digitos_cuenta, :clave1, :clave2, :medica, :internacional,"
                        ":auto_codigo, :tamanocodigo, :cuenta_cobros, :cuenta_pagos,"
                        ":id_divisa, :enlaceweb, :contabilidad, :consultas, :primer_dia_laborable,"
                        ":ultimo_dia_laborable, :horario_primer_dia, :horario_dia_normal,"
                        ":horario_ultimo_dia, :ticket_factura, :margen, :margen_minimo, :seguimiento,"
                        ":id_tarifa_predeterminada, :actualizardivisas, :cuenta_ventas_mercaderias,"
                        ":cuenta_ventas_servicios, :cuenta_iva_soportado1, :cuenta_iva_soportado2,"
                        ":cuenta_iva_soportado3, :cuenta_iva_soportado4, :cuenta_iva_repercutido1,"
                        ":cuenta_iva_repercutido2, :cuenta_iva_repercutido3, :cuenta_iva_repercutido4,"
                        ":cuenta_iva_repercutido1_re, :cuenta_iva_repercutido2_re, :cuenta_iva_repercutido3_re,"
                        ":cuenta_iva_repercutido4_re, :cuenta_iva_soportado1_re, :cuenta_iva_soportado2_re,"
                        ":cuenta_iva_soportado3_re, :cuenta_iva_soportado4_re);"
                        );
            QString grupo = "Grupo";
            grupo.append(_nombre);

            query.replace("@grupo@",grupo);
            q.prepare(query);
            q.bindValue(":codigo",_empresaFp.value("CODEMP").toString());
            q.bindValue(":nombre",n_empresa);
            q.bindValue(":digitos_factura",9);
            q.bindValue(":serie",1);
            q.bindValue(":nombre_bd",nEmpresa);
            q.bindValue(":nombre_db_conta",nConta);
            q.bindValue(":nombre_bd_medica",nMedic);
            q.bindValue(":direccion",_empresaFp.value("CDOMICILIO").toString().trimmed());
            q.bindValue(":cp",_empresaFp.value("CCODPOS").toString().trimmed());
            q.bindValue(":poblacion",_empresaFp.value("CPOBLACION").toString().trimmed());
            q.bindValue(":provincia",_empresaFp.value("CPROVINCIA").toString().trimmed());
            q.bindValue(":pais","ESPAÑA");//TODO cambiar esto
            q.bindValue(":telefono1",_empresaFp.value("CTLF").toString().trimmed());
            q.bindValue(":telefono2","");
            q.bindValue(":fax",_empresaFp.value("CFAX").toString().trimmed());
            q.bindValue(":mail",_empresaFp.value("CEMAIL").toString().trimmed());
            q.bindValue(":web","");
            q.bindValue(":cif",_empresaFp.value("CNIF").toString().trimmed());
            q.bindValue(":inscripcion","");
            q.bindValue(":comentario_albaran","");
            q.bindValue(":comentario_factura","");
            q.bindValue(":comentario_ticket","");
            q.bindValue(":ejercicio",QDate::currentDate().year());
            q.bindValue(":usar_irpf",0);//TODO irpf empresa
            q.bindValue(":codigo_cuenta_clientes","");
            q.bindValue(":codigo_cuenta_proveedores","");
            q.bindValue(":codigo_cuenta_acreedores","");
            q.bindValue(":digitos_cuenta",9);
            q.bindValue(":clave1","");//TODO claves
            q.bindValue(":clave2","");
            q.bindValue(":medica",0);
            q.bindValue(":internacional",0);
            q.bindValue(":auto_codigo",0);
            q.bindValue(":tamanocodigo",9);
            q.bindValue(":cuenta_cobros","");//TODO c cobros
            q.bindValue(":cuenta_pagos","");//TODO c pagos
            q.bindValue(":id_divisa",0);//TODO divisa
            q.bindValue(":enlaceweb",0);//TODO Grrr
            q.bindValue(":contabilidad",0);
            q.bindValue(":consultas",0);
            q.bindValue(":primer_dia_laborable",0);
            q.bindValue(":ultimo_dia_laborable",0);
            q.bindValue(":horario_primer_dia","");
            q.bindValue(":horario_dia_normal","");
            q.bindValue(":horario_ultimo_dia","");
            q.bindValue(":ticket_factura",1);
            q.bindValue(":margen",0);
            q.bindValue(":margen_minimo",0);
            q.bindValue(":seguimiento",0);
            q.bindValue(":id_tarifa_predeterminada",0);
            q.bindValue(":actualizardivisas",0);
            q.bindValue(":cuenta_ventas_mercaderias","");
            q.bindValue(":cuenta_ventas_servicios","");
            q.bindValue(":cuenta_iva_soportado1","");
            q.bindValue(":cuenta_iva_soportado2","");
            q.bindValue(":cuenta_iva_soportado3","");
            q.bindValue(":cuenta_iva_soportado4","");
            q.bindValue(":cuenta_iva_repercutido1","");
            q.bindValue(":cuenta_iva_repercutido2","");
            q.bindValue(":cuenta_iva_repercutido3","");
            q.bindValue(":cuenta_iva_repercutido4","");
            q.bindValue(":cuenta_iva_repercutido1_re","");
            q.bindValue(":cuenta_iva_repercutido2_re","");
            q.bindValue(":cuenta_iva_repercutido3_re","");
            q.bindValue(":cuenta_iva_repercutido4_re","");
            q.bindValue(":cuenta_iva_soportado1_re","");
            q.bindValue(":cuenta_iva_soportado2_re","");
            q.bindValue(":cuenta_iva_soportado3_re","");
            q.bindValue(":cuenta_iva_soportado4_re","");*/
            QString query = QString(
                        "INSERT INTO `@grupo@`.`empresas` "
                        "(`codigo`, `nombre`, `digitos_factura`, `serie`, `nombre_bd`, `nombre_db_conta`,"
                        "`nombre_bd_medica`, `direccion`, `cp`, `poblacion`, `provincia`, `pais`,"
                        "`telefono1`,  `fax`, `mail`, `cif`) "
                        "VALUES "
                        "(:codigo, :nombre, 9 , 1, :nombre_bd, :nombre_db_conta,"
                        ":nombre_bd_medica, :direccion, :cp, :poblacion, :provincia, :pais,"
                        ":telefono1, :fax, :mail, :cif);"
                        );
            QString grupo = "Grupo";
            grupo.append(_nombre);

            query = query.replace("@grupo@",grupo);
            QSqlQuery q2(QSqlDatabase::database("grupo"));
            q2.prepare(query);
            q2.bindValue(":codigo",_empresaFp.value("CODEMP").toString().trimmed());
            q2.bindValue(":nombre",n_empresa);
            q2.bindValue(":nombre_bd",nEmpresa);
            q2.bindValue(":nombre_db_conta",nConta);
            q2.bindValue(":nombre_bd_medica",nMedic);
            q2.bindValue(":direccion",_empresaFp.value("CDOMICILIO").toString().trimmed());
            q2.bindValue(":cp",_empresaFp.value("CCODPOS").toString().trimmed());
            q2.bindValue(":poblacion",_empresaFp.value("CPOBLACION").toString().trimmed());
            q2.bindValue(":provincia",_empresaFp.value("CPROVINCIA").toString().trimmed());
            q2.bindValue(":pais","ESPAÑA");//TODO cambiar esto
            q2.bindValue(":telefono1",_empresaFp.value("CTLF").toString().trimmed());
            q2.bindValue(":fax",_empresaFp.value("CFAX").toString().trimmed());
            q2.bindValue(":mail",_empresaFp.value("CEMAIL").toString().trimmed());
            q2.bindValue(":cif",_empresaFp.value("CNIF").toString().trimmed());

            if(!q2.exec())
            {
                qDebug() << q2.lastError().text();
                qDebug() << q2.boundValues();
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
            }
        }
    }
    else
        qDebug() << db.lastError().text();
}
