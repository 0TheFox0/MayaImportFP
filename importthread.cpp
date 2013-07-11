#include "importthread.h"

#include <QScopedPointer>
importThread::importThread(MainWindow * mw, QObject *parent) :
    QThread(parent)
{
    _hardStop  = false;
    _haveError = false;
    _error = "";
    _mw = mw;
}

importThread::~importThread()
{
 _mw->deleteLater();
}

void importThread::_importClients()
{
    _importComenCli();

    emit Progress(tr("Abriendo Archivo: Clientes"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Clientes.dbf";
     _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("SELECT * FROM d_Clientes"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    QSqlQuery wq(QSqlDatabase::database("grupo"));
    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;

        QSqlRecord r=q.record();        

        QString nomCli =r.value("CNOMCLI").toString().trimmed() ;
        QString prg = tr("Clientes : %1").arg(nomCli);
        emit Progress(prg,rows);

        QStringList nameList = nomCli.split(" ");
        QString name , apell1 , apell2;
        if(!nameList.isEmpty())
            name = nameList.at(0);
        if(nameList.size()>=2)
            apell1= nameList.at(1);
        if(nameList.size()>=3)
        {
            for(int i = 2;i<nameList.size();i++)
                apell1.append(nameList.at(i));
        }
        /*
      -->" 0:" QSqlField("CCODCLI", QString, length: 6, required: no, generated: yes, typeID: 12) "000198"
      -->" 1:" QSqlField("CNOMCLI", QString, length: 100, required: no, generated: yes, typeID: 12) "Ana Dorador Mu±oz"
      -->" 2:" QSqlField("CNOMCOM", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         " 3:" QSqlField("CDIRCLI", QString, length: 100, required: no, generated: yes, typeID: 12) "C/ Valencia 1, 3¦3¦"
         " 4:" QSqlField("CPOBCLI", QString, length: 100, required: no, generated: yes, typeID: 12) "Castelldefels"
         " 5:" QSqlField("CCODPROV", QString, length: 4, required: no, generated: yes, typeID: 12) "0008"
         " 6:" QSqlField("CPTLCLI", QString, length: 10, required: no, generated: yes, typeID: 12) "08860"
         " 7:" QSqlField("CTFO1CLI", QString, length: 17, required: no, generated: yes, typeID: 12) "936641676"
         " 8:" QSqlField("CTFO2CLI", QString, length: 17, required: no, generated: yes, typeID: 12) "670635339"
         " 9:" QSqlField("CFAXCLI", QString, length: 17, required: no, generated: yes, typeID: 12) ""
         "10:" QSqlField("CDNICIF", QString, length: 15, required: no, generated: yes, typeID: 12) ""
         "11:" QSqlField("CCONTACTO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "12:" QSqlField("CNBRBCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "13:" QSqlField("CDIRBCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "14:" QSqlField("CPOBBCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "15:" QSqlField("CPROVBCO", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "16:" QSqlField("CENTIDAD", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "17:" QSqlField("CAGENCIA", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "18:" QSqlField("CCUENTA", QString, length: 10, required: no, generated: yes, typeID: 12) ""
         "19:" QSqlField("COBSCLI", QString, length: 50, required: no, generated: yes, typeID: 12) ""
         "20:" QSqlField("CCODPAGO", QString, length: 2, required: no, generated: yes, typeID: 12) "CO"
         "21:" QSqlField("NDIA1PAGO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "22:" QSqlField("NDIA2PAGO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "23:" QSqlField("CCODAGE", QString, length: 3, required: no, generated: yes, typeID: 12) "1"
         "24:" QSqlField("NDTO", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "25:" QSqlField("NDPP", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "26:" QSqlField("NMOROSOS", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "27:" QSqlField("NRIESGO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "28:" QSqlField("NCOPFAC", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "29:" QSqlField("CCODRAP", QString, length: 3, required: no, generated: yes, typeID: 12) ""
         "30:" QSqlField("CSERIEFACT", QString, length: 1, required: no, generated: yes, typeID: 12) "A"
         "31:" QSqlField("CCTAREM", QString, length: 2, required: no, generated: yes, typeID: 12) ""
         "32:" QSqlField("NIVA", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "33:" QSqlField("LREQ", uint, length: 1, required: no, generated: yes) "0"
         "34:" QSqlField("CSUBCTA", QString, length: 12, required: no, generated: yes, typeID: 12) "4300198"
         "35:" QSqlField("CCTAVTAS", QString, length: 3, required: no, generated: yes, typeID: 12) "700"
         "36:" QSqlField("LMAYORISTA", uint, length: 1, required: no, generated: yes) "0"
         "37:" QSqlField("LSELECT", uint, length: 1, required: no, generated: yes) "1"
         "38:" QSqlField("NRIESGOALC", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "39:" QSqlField("NETIQUETAS", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "40:" QSqlField("CDIRECCION", QString, length: 2, required: no, generated: yes, typeID: 12) "0"
         "41:" QSqlField("EMAIL", QString, length: 50, required: no, generated: yes, typeID: 12) "doradorana@eresmas.com"
         "42:" QSqlField("PASSWORD", QString, length: 8, required: no, generated: yes, typeID: 12) ""
         "43:" QSqlField("CNACCLI", QString, length: 4, required: no, generated: yes, typeID: 12) "ESPA"
         "44:" QSqlField("CNACBCO", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "45:" QSqlField("CCODGRUP", QString, length: 4, required: no, generated: yes, typeID: 12) "GEN"
         "46:" QSqlField("CCODDIV", QString, length: 3, required: no, generated: yes, typeID: 12) "EUR"
         "47:" QSqlField("NDIA3PAGO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "48:" QSqlField("CCODTRAN", QString, length: 3, required: no, generated: yes, typeID: 12) ""
         "49:" QSqlField("CCTAVENS", QString, length: 3, required: no, generated: yes, typeID: 12) "705"
         "50:" QSqlField("CSCTASUPL", QString, length: 12, required: no, generated: yes, typeID: 12) "7050000"
         "51:" QSqlField("SCTAANTCLI", QString, length: 12, required: no, generated: yes, typeID: 12) ""
         "52:" QSqlField("LIVAINCL", uint, length: 1, required: no, generated: yes) "0"
         "53:" QSqlField("DINIVACA", QDate, length: 10, required: no, generated: yes, typeID: 9) ""
         "54:" QSqlField("DFINVACA", QDate, length: 10, required: no, generated: yes, typeID: 9) ""
         "55:" QSqlField("CCODEDI", QString, length: 17, required: no, generated: yes, typeID: 12) ""
         "56:" QSqlField("NDTOESP", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "57:" QSqlField("NCOPPRE", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "58:" QSqlField("NCOPPED", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "59:" QSqlField("NCOPALB", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "60:" QSqlField("NCOPABO", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "61:" QSqlField("NCOPREC", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "62:" QSqlField("CTFOBCO", QString, length: 17, required: no, generated: yes, typeID: 12) ""
         "63:" QSqlField("CFAXBCO", QString, length: 17, required: no, generated: yes, typeID: 12) ""
         "64:" QSqlField("CCONTACBCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "65:" QSqlField("COBSER1BCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "66:" QSqlField("COBSER2BCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "67:" QSqlField("LBLOQUEADO", uint, length: 1, required: no, generated: yes) "0"
         "68:" QSqlField("CDESCBLOQU", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "69:" QSqlField("CRINFORMA", QString, length: 1, required: no, generated: yes, typeID: 12) ""
         "70:" QSqlField("LEFACTURA", uint, length: 1, required: no, generated: yes) "0"
         "71:" QSqlField("LENVIOELEC", uint, length: 1, required: no, generated: yes) "0"
         "72:" QSqlField("SCTAANTNIC", QString, length: 12, required: no, generated: yes, typeID: 12) "438"
         "73:" QSqlField("CPTLBCO", QString, length: 10, required: no, generated: yes, typeID: 12) ""
         "74:" QSqlField("NCLAVEIDEN", double, length: 15, required: no, generated: yes, typeID: 8) "2"
         "75:" QSqlField("CREPLEGAL", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "76:" QSqlField("CNIFRLEGAL", QString, length: 9, required: no, generated: yes, typeID: 12) ""
         "77:" QSqlField("CTIMESTAMP", QString, length: 19, required: no, generated: yes, typeID: 12) ""
         "78:" QSqlField("CUSRTMSTMP", QString, length: 2, required: no, generated: yes, typeID: 12) "" */


        wq.prepare(
         "INSERT INTO clientes "
         "(codigo_cliente, apellido1, apellido2, nombre, nombre_fiscal, nombre_comercial,"
         "persona_contacto, cif_nif, direccion1, direccion2, cp, poblacion, provincia,"
         "id_pais, telefono1, telefono2, fax, movil, email, web, "
         "acumulado_ventas, ventas_ejercicio, riesgo_maximo, deuda_actual, comentarios, bloqueado,"
         "comentario_bloqueo, porc_dto_cliente, recargo_equivalencia, cuenta_contable, cuenta_iva_repercutido,"
         "cuenta_deudas, cuenta_cobros, id_forma_pago, dia_pago1, dia_pago2, tarifa_cliente, importe_a_cuenta,"
         "vales, entidad_bancaria, oficina_bancaria, dc, cuenta_corriente,"
         "importe_pendiente,acceso_web,password_web,observaciones) "
         "VALUES "
         "(:cCodigoCliente, :cApellido1, :cApellido2, :cNombre, :cNombreFiscal, :cNombreComercial,"
         ":cPersonaContacto, :cCifNif, :cDireccion1, :cDireccion2, :cCP, :cPoblacion, :cProvincia,"
         ":idpais, :cTelefono1, :cTelefono2, :cFax, :cMovil, :cEmail, :cWeb, "
         ":rAcumuladoVentas, :rVentasEjercicio, :rRiesgoMaximo, :rDeudaActual, :tComentarios, :lBloqueado,"
         ":tComentarioBloqueo, :nPorcDtoCliente, :lRecargoEquivalencia, :cCuentaContable, :cCuentaIvaRepercutido,"
         ":cCuentaDeudas, :cCuentaCobros, :idFormaPago, :nDiapago1, :nDiaPago2, :nTarifaCliente, :rImporteACuenta,"
         ":rVales, :cCentidadBancaria, :cOficinaBancaria, :cDC, :cCuentaCorriente,"
         ":rImportePendiente,:acceso_web,:password_web,:observaciones);" );

        wq.bindValue(":cCodigoCliente",r.value("CSUBCTA").toString().trimmed().toUpper());
        wq.bindValue(":cApellido1",apell1.trimmed().toUpper());
        wq.bindValue(":cApellido2",apell2.trimmed().toUpper());
        wq.bindValue(":cNombre",name.toUpper());
        wq.bindValue(":cNombreFiscal",r.value("CNOMCLI").toString().trimmed().toUpper());
        wq.bindValue(":cNombreComercial",r.value("CNOMCOM").toString().trimmed());
        wq.bindValue(":cPersonaContacto",r.value("CCONTACTO").toString().trimmed());
        wq.bindValue(":cCifNif",r.value("CDNICIF").toString().trimmed());
        wq.bindValue(":cDireccion1",r.value("CDIRCLI").toString().trimmed());
        wq.bindValue(":cDireccion2","");
        wq.bindValue(":cCP",r.value("CPTLCLI").toString().trimmed());
        wq.bindValue(":cPoblacion",r.value("CPOBCLI").toString().trimmed().toUpper());
        wq.bindValue(":cProvincia",r.value("CCODPROV").toString().trimmed()); // TODO hay que poner el nombre, no el código.
        wq.bindValue(":idpais",-1);//TODO id_pais
        wq.bindValue(":cTelefono1",r.value("CTFO1CLI").toString().trimmed());
        wq.bindValue(":cTelefono2",r.value("CTFO2CLI").toString().trimmed());
        wq.bindValue(":cFax",r.value("CFAXCLI").toString().trimmed());
        wq.bindValue(":cMovil","");
        wq.bindValue(":cEmail",r.value("EMAIL").toString().trimmed());
        wq.bindValue(":cWeb","");
        //wq.bindValue(":dFechaAlta",r.value("CTIMESTAMP").toString().trimmed());
        //wq.bindValue(":dFechaUltimaCompra","");
        wq.bindValue(":rAcumuladoVentas",0);
        wq.bindValue(":rVentasEjercicio",0);
        wq.bindValue(":rRiesgoMaximo",0);
        wq.bindValue(":rDeudaActual",r.value("NMOROSOS").toDouble());

        QString com = r.value("COBSCLI").toString().trimmed();
        com.append("\n");
        com.append(_cCliente.value(r.value("CCODCLI").toString().trimmed()));
        wq.bindValue(":tComentarios",com);
        wq.bindValue(":observaciones",r.value("COBSCLI").toString().trimmed());

        wq.bindValue(":lBloqueado",r.value("LBLOQUEADO").toUInt());
        wq.bindValue(":tComentarioBloqueo",r.value("CDESCBLOQU").toString().trimmed());
        wq.bindValue(":nPorcDtoCliente",0);
        wq.bindValue(":lRecargoEquivalencia",r.value("LREC").toUInt());
        wq.bindValue(":cCuentaContable","");
        wq.bindValue(":cCuentaIvaRepercutido","");
        wq.bindValue(":cCuentaDeudas","");
        wq.bindValue(":cCuentaCobros","");
        wq.bindValue(":idFormaPago",_fpago.value(r.value("CCODPAGO").toString().trimmed()));
        wq.bindValue(":nDiapago1",r.value("NDIA1PAGO").toString().trimmed());
        wq.bindValue(":nDiaPago2",r.value("NDIA2PAGO").toString().trimmed());
        wq.bindValue(":nTarifaCliente",0);
        wq.bindValue(":rImporteACuenta",0);
        wq.bindValue(":rVales",0);

        QString ent = r.value("CENTIDAD").toString().trimmed();
        wq.bindValue(":cCentidadBancaria",ent);
        QString ofi = r.value("CAGENCIA").toString().trimmed();
        wq.bindValue(":cOficinaBancaria",ofi);
        QString cc = r.value("CCUENTA").toString().trimmed();
        wq.bindValue(":cDC",_ValidarCC(ent,ofi,cc));
        wq.bindValue(":cCuentaCorriente",cc);
        //wq.bindValue(":dFechaNacimiento","");
        wq.bindValue(":rImportePendiente",0);
        wq.bindValue(":acceso_web",r.value("CSUBCTA").toString().trimmed().toUpper());
        wq.bindValue(":password_web",r.value("CDNICIF").toString().trimmed().toUpper());

        if(!wq.exec())
        {
            _haveError = true;
            _error = wq.lastError().text();
        }

    }while(q.next());
    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Clientes");
    _cCliente.clear();
}

void importThread::_importComenCli()
{
    emit Progress(tr("Abriendo Archivo: Notas de clientes"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Clientec.dbf";
     _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("SELECT * FROM d_Clientec"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        emit Progress(tr("Leyendo notas de clientes"),rows);

        QSqlRecord r=q.record();
        QString Cod = r.value("CCODCLI").toString().trimmed();
        QString nota = r.value("NOTAS").toString().trimmed();
        if(_cCliente[Cod].isEmpty())
            _cCliente[Cod].append(nota);
        else
        {
            _cCliente[Cod].append("\n");
            _cCliente[Cod].append(nota);
        }

    }while(q.next());

    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Clientec");
}

void importThread::_importFormPago()
{
    emit Progress(tr("Abriendo Archivo: Formas de pago"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Fpago.dbf";
    _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Fpago"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();    

    QSqlQuery wq(QSqlDatabase::database("grupo"));
    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        QSqlRecord r = q.record();
        QString ccod = r.value("CCODPAGO").toString().trimmed();
        QString desc = r.value("CDESPAGO").toString().trimmed();
        QString prg = tr("Forma de pago: %1").arg(desc);
        emit Progress(prg,rows);
        /*
      -->" 0:" QSqlField("CCODPAGO", QString, length: 2, required: no, generated: yes, typeID: 12) "R6"
      -->" 1:" QSqlField("CDESPAGO", QString, length: 30, required: no, generated: yes, typeID: 12) "Recibo Domiciliado 90 dÝas"
         " 2:" QSqlField("NIMPAPL", double, length: 15, required: no, generated: yes, typeID: 8) "0"
      -->" 3:" QSqlField("NDIASAPL", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         " 4:" QSqlField("NPORC1", double, length: 15, required: no, generated: yes, typeID: 8) "100"
         " 5:" QSqlField("NAPL1", double, length: 15, required: no, generated: yes, typeID: 8) "90"
         " 6:" QSqlField("NPORC2", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         " 7:" QSqlField("NAPL2", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         " 8:" QSqlField("NPORC3", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         " 9:" QSqlField("NAPL3", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "10:" QSqlField("NPORC4", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "11:" QSqlField("NAPL4", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "12:" QSqlField("NPORC5", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "13:" QSqlField("NAPL5", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "14:" QSqlField("CCTACOBRO", QString, length: 12, required: no, generated: yes, typeID: 12) "5720001"
       -->"15:" QSqlField("NPLAZOS", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "16:" QSqlField("LPAGADO", uint, length: 1, required: no, generated: yes) "0"
         "17:" QSqlField("LPAGVCTO", uint, length: 1, required: no, generated: yes) "0"
         "18:" QSqlField("LMESCOMP", uint, length: 1, required: no, generated: yes) "0"
         "19:" QSqlField("NINTERNET", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "20:" QSqlField("NMODALIDAD", double, length: 15, required: no, generated: yes, typeID: 8) "3"
         "21:" QSqlField("NCOMISION", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "22:" QSqlField("CSCTACOM", QString, length: 12, required: no, generated: yes, typeID: 12) ""
         "23:" QSqlField("NDPP", double, length: 15, required: no, generated: yes, typeID: 8) "0"
      -->"24:" QSqlField("CCTAPAGO", QString, length: 12, required: no, generated: yes, typeID: 12) "5720000"
         "25:" QSqlField("LPASACONTA", uint, length: 1, required: no, generated: yes) "0"
         "26:" QSqlField("CIDMPAGO", QString, length: 2, required: no, generated: yes, typeID: 12) ""
         "27:" QSqlField("CIMAGEN", QString, length: 128, required: no, generated: yes, typeID: 12) ""
         "28:" QSqlField("LBMPTACTIL", uint, length: 1, required: no, generated: yes) "0"
         "29:" QSqlField("CDES_TAC", QString, length: 10, required: no, generated: yes, typeID: 12) ""
         "30:" QSqlField("LIMPORMETA", uint, length: 1, required: no, generated: yes) "0"
         "31:" QSqlField("NVENCAPART", double, length: 15, required: no, generated: yes, typeID: 8) "2"
         "32:" QSqlField("CIMAGENORI", QString, length: 128, required: no, generated: yes, typeID: 12) ""
        */


        wq.prepare(
        "INSERT INTO formpago "
        "(codigo, forma_pago, dia_pago1, cuenta_cont_pago, numero_plazos) "
        "VALUES (:cCodigo, :cFormapago, :nDiaPago1, :cuenta_cont_pago, :numero_plazos);"
                    );

        wq.bindValue(":cCodigo",ccod);
        wq.bindValue(":cFormapago",desc);
        wq.bindValue(":nDiaPago1",r.value("NDIASAPL").toDouble());
        wq.bindValue(":cuenta_cont_pago",r.value("CCTAPAGO").toString().trimmed());
        wq.bindValue(":numero_plazos",r.value("NPLAZOS").toDouble());

        if(wq.exec())
            _fpago[ccod] = wq.lastInsertId().toInt();
        else
        {
            _haveError = true;
            _error = wq.lastError().text();
        }
    }while(q.next());
    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Fpago");
}

void importThread::_importProv()
{
    emit Progress(tr("Abriendo Archivo: Proveedores"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Proveedo.dbf";
    _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Proveedo"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    QSqlQuery wq(QSqlDatabase::database("grupo"));
    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        QSqlRecord r = q.record();
        QString desc = r.value("CNOMPRO").toString().trimmed();
        QString prg = tr("Proveedores: %1").arg(desc);
        emit Progress(prg,rows);
        /*
         " 0:" QSqlField("CCODPRO", QString, length: 6, required: no, generated: yes, typeID: 12) "000001"
         " 1:" QSqlField("CNOMPRO", QString, length: 100, required: no, generated: yes, typeID: 12) "DISTRAUMA S.L."
         " 2:" QSqlField("CDIRPRO", QString, length: 100, required: no, generated: yes, typeID: 12) "C/ Monolit 17"
         " 3:" QSqlField("CPOBPRO", QString, length: 100, required: no, generated: yes, typeID: 12) "Palau-Solita i Plegamans"
         " 4:" QSqlField("CCODPROV", QString, length: 4, required: no, generated: yes, typeID: 12) "0008"
         " 5:" QSqlField("CPTLPRO", QString, length: 10, required: no, generated: yes, typeID: 12) "08184"
         " 6:" QSqlField("CTFO1PRO", QString, length: 17, required: no, generated: yes, typeID: 12) "938644623"
         " 7:" QSqlField("CTFO2PRO", QString, length: 17, required: no, generated: yes, typeID: 12) "902117967"
         " 8:" QSqlField("CFAX", QString, length: 17, required: no, generated: yes, typeID: 12) "938644294"
         " 9:" QSqlField("CNIFDNI", QString, length: 15, required: no, generated: yes, typeID: 12) "B61435079"
         "10:" QSqlField("NDTO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "11:" QSqlField("CCODPAGO", QString, length: 2, required: no, generated: yes, typeID: 12) "30"
         "12:" QSqlField("NDIAPAGO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "13:" QSqlField("CNBRBCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "14:" QSqlField("CDIRBCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "15:" QSqlField("CPOBBCO", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "16:" QSqlField("CPROVBCO", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "17:" QSqlField("CENTIDAD", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "18:" QSqlField("CAGENCIA", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "19:" QSqlField("CCUENTA", QString, length: 10, required: no, generated: yes, typeID: 12) ""
         "20:" QSqlField("CSUBCTA", QString, length: 12, required: no, generated: yes, typeID: 12) "4000001"
         "21:" QSqlField("LSELECT", uint, length: 1, required: no, generated: yes) "0"
         "22:" QSqlField("NETIQUETAS", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "23:" QSqlField("CNACPRO", QString, length: 4, required: no, generated: yes, typeID: 12) "ESPA"
         "24:" QSqlField("CNACBCO", QString, length: 4, required: no, generated: yes, typeID: 12) ""
         "25:" QSqlField("CPERCONT", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "26:" QSqlField("CCODDIV", QString, length: 3, required: no, generated: yes, typeID: 12) "EUR"
         "27:" QSqlField("NIVA", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "28:" QSqlField("CSERIEFACT", QString, length: 1, required: no, generated: yes, typeID: 12) "A"
         "29:" QSqlField("NIVAPORTES", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "30:" QSqlField("NIRPFA", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "31:" QSqlField("NIRPFB", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "32:" QSqlField("NRECFINANC", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "33:" QSqlField("LIRPFAGR", uint, length: 1, required: no, generated: yes) "0"
         "34:" QSqlField("NDPP", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "35:" QSqlField("NDIA2PAGO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "36:" QSqlField("NDIA3PAGO", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "37:" QSqlField("DINIVACA", QDate, length: 10, required: no, generated: yes, typeID: 9) ""
         "38:" QSqlField("DFINVACA", QDate, length: 10, required: no, generated: yes, typeID: 9) ""
         "39:" QSqlField("CCTACPRAS", QString, length: 3, required: no, generated: yes, typeID: 12) "600"
         "40:" QSqlField("CCTACPRASS", QString, length: 3, required: no, generated: yes, typeID: 12) "629"
         "41:" QSqlField("EMAIL", QString, length: 50, required: no, generated: yes, typeID: 12) "distrauma@arrakis.es"
         "42:" QSqlField("CRINFORMA", QString, length: 1, required: no, generated: yes, typeID: 12) ""
         "43:" QSqlField("NCLAVEIDEN", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "44:" QSqlField("CREPLEGAL", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "45:" QSqlField("CNIFRLEGAL", QString, length: 9, required: no, generated: yes, typeID: 12) ""
         "46:" QSqlField("CTIMESTAMP", QString, length: 19, required: no, generated: yes, typeID: 12) "2012-09-05T10:34:00"
         "47:" QSqlField("CUSRTMSTMP", QString, length: 2, required: no, generated: yes, typeID: 12) "0O"
         */

//INSERT INTO `mayageneral`.`proveedores` (`codigo`, `proveedor`, `cif`, `direccion1`, `cp`, `poblacion`, `provincia`, `telefono1`, `telefono2`, `fax`, `email`, `persona_contacto`, `dia_cobro`, `id_forma_pago`, `importe_acumulado_compras`, `entidad_bancaria_proveedor`, `oficina_bancaria_proveedor`, `dc_proveedor`, `cc_proveedor`, `retencion_irpf`, `tipo_retencion`, `Comentarios`, `dto`, `deuda_maxima`, `deuda_actual`, `recargo_equivalencia`, `texto_para_pedidos`, `entregado_a_cuenta`, `id_pais`, `id_monedas`) VALUES ('cod', 'prov', 'cif', 'd1', 'cp', 'pob', 'provi', 'tl1', 'tl2', 'fax', 'mail', 'pconrta', '0', '0', '0', 'ba', 'of', 'dc', 'cc', '0', '0', 'comen', '0', '0', '0', '0', 'tex', '0', '0', '0');

        wq.prepare(
        "INSERT INTO `proveedores` (`codigo`, `proveedor`, `cif`, `direccion1`,"
        "`cp`, `poblacion`, `provincia`, `telefono1`, `telefono2`, `fax`, `email`,"
        "`persona_contacto`, `dia_cobro`, `id_forma_pago`, `importe_acumulado_compras`,"
        "`entidad_bancaria_proveedor`, `oficina_bancaria_proveedor`, `dc_proveedor`,"
        "`cc_proveedor`, `retencion_irpf`, `tipo_retencion`, `Comentarios`, `dto`,"
        "`deuda_maxima`, `deuda_actual`, `recargo_equivalencia`, `texto_para_pedidos`,"
        "`entregado_a_cuenta`, `id_pais`, `id_monedas`) "
        "VALUES ("
        ":codigo, :proveedor, :cif, :direccion1,"
        ":cp, :poblacion, :provincia, :telefono1, :telefono2, :fax, :email,"
        ":persona_contacto, :dia_cobro, :id_forma_pago, :importe_acumulado_compras,"
        ":entidad_bancaria_proveedor, :oficina_bancaria_proveedor, :dc_proveedor,"
        ":cc_proveedor, :retencion_irpf, :tipo_retencion, :Comentarios, :dto,"
        ":deuda_maxima, :deuda_actual, :recargo_equivalencia, :texto_para_pedidos,"
        ":entregado_a_cuenta, :id_pais, :id_monedas )"
                    );

        wq.bindValue(":codigo",r.value("CCODPRO").toString().trimmed());
        wq.bindValue(":proveedor",r.value("CNOMPRO").toString().trimmed());
        wq.bindValue(":cif",r.value("CNIFDNI").toString().trimmed());
        wq.bindValue(":direccion1",r.value("CDIRPRO").toString().trimmed());
        wq.bindValue(":cp",r.value("CPTLPRO").toString().trimmed());
        wq.bindValue(":poblacion",r.value("CPOBPROV").toString().trimmed());
        wq.bindValue(":provincia",r.value("CCODPROV").toString().trimmed());
        wq.bindValue(":telefono1",r.value("CTFO1PRO").toString().trimmed());
        wq.bindValue(":telefono2",r.value("CTFO2PRO").toString().trimmed());
        wq.bindValue(":fax",r.value("CFAX").toString().trimmed());
        wq.bindValue(":email",r.value("EMAIL").toString().trimmed());
        wq.bindValue(":persona_contacto",r.value("CPERCONT").toString().trimmed());
        wq.bindValue(":dia_cobro",/*r.value("NDIAPAGO").toInt()*/0);
        wq.bindValue(":id_formaPago",_fpago.value(r.value("CCODPAGO").toString().trimmed()));
        wq.bindValue(":importe_acumulado_compras",0);

        QString ent = r.value("CENTIDAD").toString().trimmed();
        wq.bindValue(":entidad_bancaria_proveedor",ent);
        QString ofi = r.value("CAGENCIA").toString().trimmed();
        wq.bindValue(":oficina_bancaria_proveedor",ofi);
        QString cc = r.value("CCUENTA").toString().trimmed();
        wq.bindValue(":dc_proveedor",_ValidarCC(ent,ofi,cc));
        wq.bindValue(":cc_proveedor",cc);

        wq.bindValue(":retencion_irpf",0);
        wq.bindValue(":tipo_retencion",r.value("NIRPFA").toString().trimmed());
        wq.bindValue(":Comentarios","");
        wq.bindValue(":dto",r.value("NDTO").toString().trimmed());
        wq.bindValue(":deuda_maxima",0);
        wq.bindValue(":deuda_actual",0);
        wq.bindValue(":recargo_equivalencia",0);
        wq.bindValue(":texto_para_pedidos","");
        wq.bindValue(":entregado_a_cuenta",0);
        wq.bindValue(":id_pais",-1);
        wq.bindValue(":id_monedas",-1);

       // qDebug() << wq.boundValues();
        if(!wq.exec())
        {
            _haveError = true;
            _error = wq.lastError().text();
        }
        else
        {
            _prov.insert(r.value("CCODPRO").toString().trimmed(),wq.lastInsertId().toInt());
        }

    }while(q.next());

    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Proveedo");

    _importComentProv();
}

void importThread::_importComentProv()
{
    emit Progress(tr("Abriendo Archivo: Comentarios de proveedor"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Proveedc.dbf";
    _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Proveedc"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    QSqlQuery wq(QSqlDatabase::database("grupo"));
    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        QSqlRecord r = q.record();
        QString nota = r.value("NOTAS").toString().trimmed();
        QString codigo = r.value("CCODPRO").toString().trimmed();
        emit Progress("Notas de proveedor",rows);

        QString query = QString("UPDATE proveedores SET comentarios ='%1' WHERE codigo='%2';").arg(nota).arg(codigo);

        if(!wq.exec(query))
        {
            _haveError = true;
            _error = wq.lastError().text();
        }
    }while(q.next());

    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Proveedc");
}

void importThread::_importFamilias()
{
    emit Progress(tr("Abriendo Archivo: Familias"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Familias.dbf";
     _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("SELECT * FROM d_Familias"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    QSqlQuery wq(QSqlDatabase::database("grupo"));

    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        emit Progress(tr("Leyendo familias de articulos"),rows);

        QSqlRecord r=q.record();
        QString cod = r.value("CCODFAM").toString().trimmed();
        QString fam = r.value("CNOMFAM").toString().trimmed();

        wq.prepare("INSERT INTO familias (codigo, familia) VALUES (:cod, :fam);");
        wq.bindValue(":cod",cod);
        wq.bindValue(":fam",fam);

        if(wq.exec())
            _fams[cod] = wq.lastInsertId().toInt();
        else
        {
            _haveError = true;
            _error = wq.lastError().text();
        }


    }while(q.next());

    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Familias");
}

void importThread::_importComentArticulos1()
{
    emit Progress(tr("Abriendo Archivo: Comentarios de Articulos"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Artcom.dbf";
    _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Artcom"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        QSqlRecord r = q.record();
        QString nota = r.value("CCOMENT").toString().trimmed();
        QString codigo = r.value("CREF").toString().trimmed();
        emit Progress("Notas de articulos",rows);

        if(_cArticulos[codigo].isEmpty())
            _cArticulos[codigo].append(nota);
        else
        {
            _cArticulos[codigo].append("\n");
            _cArticulos[codigo].append(nota);
        }

    }while(q.next());

    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Artcom");
}

void importThread::_importComentArticulos2()
{
    emit Progress(tr("Abriendo Archivo: Comentarios de Articulos"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Articulc.dbf";
    _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Articulc"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        QSqlRecord r = q.record();
        QString nota = r.value("NOTAS").toString().trimmed();
        QString codigo = r.value("CREF").toString().trimmed();
        emit Progress("Notas de articulos",rows);

        if(_cArticulos[codigo].isEmpty())
            _cArticulos[codigo].append(nota);
        else
        {
            _cArticulos[codigo].append("\n");
            _cArticulos[codigo].append(nota);
        }

    }while(q.next());

    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Articulc");
}

void importThread::_importStocks()
{
    emit Progress(tr("Abriendo Archivo: Stocks de Articulos"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Stocks.dbf";
    _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Stocks"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;
        QSqlRecord r = q.record();
        double stck = r.value("NSTOCKS").toDouble();
        QString codigo = r.value("CREF").toString().trimmed();
        emit Progress("Stocks de articulos",rows);

        //NOTE Stock consolidado de almacenes, revisar cuando implementemos multialmacen
        if(_stockArticulos.contains(codigo))
            _stockArticulos[codigo]+= stck;
        else
            _stockArticulos[codigo] = stck;

    }while(q.next());

    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);

    q.exec("DROP TABLE IF EXISTS d_Stocks");
}

void importThread::_generarCodigosTarifa()
{
    emit Progress(tr("Abriendo Archivo: Divisas"),0);
    emit sizeOfTask(0);

    QSqlQuery wq(QSqlDatabase::database("grupo"));

    QHash<QString,int>::Iterator it;

    int row = 0;
    emit sizeOfTask(_divisas.size());
    for(it = _divisas.begin();it!=_divisas.end();++it)
    {
        row++;

        QString k = it.key();
        emit Progress(tr("Divisas: %1").arg(k),row);

        QString des = QString("facturaplus_%1").arg(k);
        QString cod = QString("F_%1").arg(k).toUpper();
        cod.truncate(15);
        wq.prepare("INSERT INTO codigotarifa "
                   "(descripcion, codigo_tarifa, dto, desc_promo, cant_promo,"
                   "cant_base, importe_dto, id_pais, id_monedas, margen, margen_min) VALUES "
                   "(:desc, :codigo, 0, 0, 0, 0, 0, 0, :moneda, 0, 0);");

        wq.bindValue(":desc",des);
        wq.bindValue(":codigo",cod);
        wq.bindValue(":moneda",it.value());

        if(wq.exec())
            _codTarifa.insert(k,wq.lastInsertId().toInt());
        else
        {
            _haveError = true;
            _error = wq.lastError().text();
        }
    }

    emit sizeOfTask(0);
}

void importThread::_importArticulos()
{
    _importFamilias();
    if(!_haveError)
        _generarCodigosTarifa();
    if(!_haveError)
        _importComentArticulos1();
    if(!_haveError)
        _importComentArticulos2();
    if(!_haveError)
        _importStocks();

    if(_haveError)
        return;

    emit Progress(tr("Abriendo Archivo: Articulos"),0);
    emit sizeOfTask(0);
    QString file = Path()+"/Articulo.dbf";
     _mw->openDb(file);

    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("SELECT * FROM d_Articulo"))
    {
        emit Error(q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;
    emit sizeOfTask(rows);

    if(rows==0)
        return;

    rows = 0;
    q.first();

    QSqlQuery wq(QSqlDatabase::database("grupo"));
    QSqlQuery wq1(QSqlDatabase::database("grupo"));
    do
    {
        if(_hardStop)
            return;
        if(_haveError)
            return;

        rows++;

        QSqlRecord r=q.record();
        QString prg = QString("Articulos: %1").arg(r.value("CDETALLE").toString().trimmed());
        emit Progress(prg,rows);

        /*
         " 0:" QSqlField("CREF", QString, length: 18, required: no, generated: yes, typeID: 12) "AL14011"
         " 1:" QSqlField("CDETALLE", QString, length: 100, required: no, generated: yes, typeID: 12) "Diagnostico por la lengua en MTC (Autor: G. Maciocia)"
         " 2:" QSqlField("CCODFAM", QString, length: 5, required: no, generated: yes, typeID: 12) "00014"
         " 3:" QSqlField("CFORMATO", QString, length: 10, required: no, generated: yes, typeID: 12) ""
         " 4:" QSqlField("NPZOREP", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         " 5:" QSqlField("NCOSTEPROM", double, length: 15, required: no, generated: yes, typeID: 8) "30.384"
         " 6:" QSqlField("NCOSTEDIV", double, length: 15, required: no, generated: yes, typeID: 8) "29.45"
         " 7:" QSqlField("NSTOCKMIN", double, length: 15, required: no, generated: yes, typeID: 8) "3"
         " 8:" QSqlField("NSTOCKMAX", double, length: 15, required: no, generated: yes, typeID: 8) "10"
         " 9:" QSqlField("CTIPOIVA", QString, length: 1, required: no, generated: yes, typeID: 12) "S"
         "10:" QSqlField("CGRPCONTA", QString, length: 9, required: no, generated: yes, typeID: 12) "0000"
         "11:" QSqlField("NPVP", double, length: 15, required: no, generated: yes, typeID: 8) "41.548"
         "12:" QSqlField("NPREMAYOR", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "13:" QSqlField("NDTO1", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "14:" QSqlField("NDTO2", double, length: 15, required: no, generated: yes, typeID: 8) "5"
         "15:" QSqlField("NDTO3", double, length: 15, required: no, generated: yes, typeID: 8) "10"
         "16:" QSqlField("NDTO4", double, length: 15, required: no, generated: yes, typeID: 8) "10"
         "17:" QSqlField("NDTO5", double, length: 15, required: no, generated: yes, typeID: 8) "10"
         "18:" QSqlField("NDTO6", double, length: 15, required: no, generated: yes, typeID: 8) "10"
         "19:" QSqlField("NPENDSER", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "20:" QSqlField("NPENDREC", double, length: 15, required: no, generated: yes, typeID: 8) "8"
         "21:" QSqlField("CCODDIV", QString, length: 3, required: no, generated: yes, typeID: 12) "EUR"
         "22:" QSqlField("NENTACU", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "23:" QSqlField("CCODPRO", QString, length: 6, required: no, generated: yes, typeID: 12) "000020"
         "24:" QSqlField("CREFPROV", QString, length: 18, required: no, generated: yes, typeID: 12) "D.L.M.CH."
         "25:" QSqlField("LSELECT", uint, length: 1, required: no, generated: yes) "0"
         "26:" QSqlField("NUNIDADES", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "27:" QSqlField("NETIQUETAS", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "28:" QSqlField("CIMAGEN", QString, length: 128, required: no, generated: yes, typeID: 12) ""
         "29:" QSqlField("CPREDEF1", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "30:" QSqlField("CPREDEF2", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "31:" QSqlField("CPREDEF3", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "32:" QSqlField("CPREDEF4", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "33:" QSqlField("CPREDEF5", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "34:" QSqlField("CPREDEF6", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "35:" QSqlField("CPREDEF7", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "36:" QSqlField("CPREDEF8", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "37:" QSqlField("CPREDEF9", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "38:" QSqlField("CPREDEF10", QString, length: 100, required: no, generated: yes, typeID: 12) ""
         "39:" QSqlField("LINTERNET", uint, length: 1, required: no, generated: yes) "0"
         "40:" QSqlField("LUNIDADES", uint, length: 1, required: no, generated: yes) "0"
         "41:" QSqlField("CCODEBAR", QString, length: 18, required: no, generated: yes, typeID: 12) "9789729822803"
         "42:" QSqlField("NTIPOCODE", double, length: 15, required: no, generated: yes, typeID: 8) "3"
         "43:" QSqlField("LKIT", uint, length: 1, required: no, generated: yes) "0"
         "44:" QSqlField("LPESO", uint, length: 1, required: no, generated: yes) "0"
         "45:" QSqlField("NTARA", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "46:" QSqlField("NBENEFPVP", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "47:" QSqlField("NBENEFMAY", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "48:" QSqlField("LPRECPROP1", uint, length: 1, required: no, generated: yes) "0"
         "49:" QSqlField("LPRECPROP2", uint, length: 1, required: no, generated: yes) "0"
         "50:" QSqlField("NPCONIVA", double, length: 15, required: no, generated: yes, typeID: 8) "43.20992"
         "51:" QSqlField("NPMCONIVA", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "52:" QSqlField("LCTRLSTOCK", uint, length: 1, required: no, generated: yes) "1"
         "53:" QSqlField("NPREPV", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "54:" QSqlField("LACTUALIZA", uint, length: 1, required: no, generated: yes) "1"
         "55:" QSqlField("DACTUALIZA", QDate, length: 10, required: no, generated: yes, typeID: 9) "2012-11-07"
         "56:" QSqlField("CGRPCONTAC", QString, length: 9, required: no, generated: yes, typeID: 12) "0000"
         "57:" QSqlField("NSALACU", double, length: 15, required: no, generated: yes, typeID: 8) "1"
         "58:" QSqlField("CSCTAVTAS", QString, length: 12, required: no, generated: yes, typeID: 12) ""
         "59:" QSqlField("CSCTACPRAS", QString, length: 12, required: no, generated: yes, typeID: 12) ""
         "60:" QSqlField("LANTICIPO", uint, length: 1, required: no, generated: yes) "0"
         "61:" QSqlField("CCODPAGO", QString, length: 2, required: no, generated: yes, typeID: 12) ""
         "62:" QSqlField("CSCTAVABON", QString, length: 12, required: no, generated: yes, typeID: 12) ""
         "63:" QSqlField("CSCTACABON", QString, length: 12, required: no, generated: yes, typeID: 12) ""
         "64:" QSqlField("LBMPTACTIL", uint, length: 1, required: no, generated: yes) "0"
         "65:" QSqlField("NCANT_TAC", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "66:" QSqlField("CDES_TAC", QString, length: 10, required: no, generated: yes, typeID: 12) ""
         "67:" QSqlField("NPERIGARAN", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         "68:" QSqlField("CENTALM", QString, length: 1, required: no, generated: yes, typeID: 12) "N"
         "69:" QSqlField("DENTALM", QDate, length: 10, required: no, generated: yes, typeID: 9) ""
         "70:" QSqlField("CLOTE", QString, length: 20, required: no, generated: yes, typeID: 12) ""
         "71:" QSqlField("DFECTRAZA", QDate, length: 10, required: no, generated: yes, typeID: 9) ""
         "72:" QSqlField("CIMAGENORI", QString, length: 128, required: no, generated: yes, typeID: 12) ""
         "73:" QSqlField("CTIMESTAMP", QString, length: 19, required: no, generated: yes, typeID: 12) "2013-04-26T17:06:36"
         "74:" QSqlField("CUSRTMSTMP", QString, length: 2, required: no, generated: yes, typeID: 12) "0E"
         "75:" QSqlField("CCODLEGAL", QString, length: 2, required: no, generated: yes, typeID: 12) ""
         "76:" QSqlField("NIVAANTER", double, length: 15, required: no, generated: yes, typeID: 8) "0"
         */



        wq.prepare(
         "INSERT INTO `articulos` "
         "(`codigo`, `codigo_barras`, `codigo_fabricante`, `descripcion`,"
         "`descripcion_reducida`, `id_proveedor`, `id_familia`, `tipo_iva`,"
         "`coste`, `comentario`, `stock_maximo`, `stock_minimo`, `stock_real`,"
                    "`tipo_unidad`, `controlar_stock`, `pvp_incluye_iva`, `etiquetas`, `id_tipos_iva`, `coste_real`)"
         "VALUES "
         "(:codigo, :codigo_barras, :codigo_fabricante, :descripcion,"
         ":descripcion_reducida, :id_proveedor, :id_familia, :tipo_iva,"
         ":coste, :comentario, :stock_maximo, :stock_minimo, :stock_real,"
         ":tipo_unidad, :controlar_stock, :pvp_incluye_iva, :etiquetas, :id_tipos_iva,:coste_real);"
                    );

        QString ref = r.value("CREF").toString().trimmed();
        QString cod_iva = r.value("CTIPOIVA").toString().trimmed();
        wq.bindValue(":codigo",ref);
        wq.bindValue(":codigo_barras",r.value("CCODEBAR").toString().trimmed());
        wq.bindValue(":codigo_fabricante",r.value("CREFPROV").toString().trimmed());
        wq.bindValue(":descripcion",r.value("CDETALLE").toString().trimmed());
        wq.bindValue(":descripcion_reducida",r.value("CDETALLE").toString().trimmed());
        wq.bindValue(":id_proveedor",_prov.value(r.value("CCODPRO").toString().trimmed()));
        wq.bindValue(":id_familia",_fams.value(r.value("CCODFAM").toString().trimmed()));
        wq.bindValue(":tipo_iva",_getRIva(cod_iva));
        wq.bindValue(":coste",r.value("NCOSTEDIV").toString().trimmed());
        wq.bindValue(":comentario",_cArticulos.value(ref));
        wq.bindValue(":stock_maximo",r.value("NSTOCKMAX").toString().trimmed());
        wq.bindValue(":stock_minimo",r.value("NSTOCKMIN").toString().trimmed());
        wq.bindValue(":stock_real",_stockArticulos.value(ref));
        wq.bindValue(":tipo_unidad",r.value("CFORMATO").toString().trimmed());
        wq.bindValue(":controlar_stock",r.value("LCTRLSTOCK").toUInt());
        wq.bindValue(":pvp_incluye_iva",0);
        wq.bindValue(":etiquetas",r.value("NETIQUETAS").toString().trimmed());
        wq.bindValue(":id_tipos_iva",_getIdIva(cod_iva));
        wq.bindValue(":coste_real",r.value("NCOSTEDIV").toString().trimmed());

        if(!wq.exec())
        {
            _haveError = true;
            _error = wq.lastError().text();
        }
        else
        {
            int art = wq.lastInsertId().toInt();
            QString CCodDiv = r.value("CCODDIV").toString().trimmed();
            double pvp = r.value("NPVP").toDouble();
            double base = r.value("NCOSTEDIV").toDouble();
            double margen = pvp - base;
            wq1.prepare(
            "INSERT INTO `tarifas` "
            "(`id_articulo`, `id_pais`, `id_monedas`, `margen`,"
            "`margen_minimo`, `pvp`, `id_codigo_tarifa`)"
            "VALUES "
            "(:id_articulo, 1, :id_monedas, :margen, :margen_minimo, :pvp, :id_codigo_tarifa);"
                        );
            wq1.bindValue(":id_articulo",art);
            wq1.bindValue(":id_monedas",_divisas.value(CCodDiv));
            wq1.bindValue(":margen",margen);
            wq1.bindValue(":margen_minimo",0);
            wq1.bindValue(":pvp",pvp);
            wq1.bindValue(":id_codigo_tarifa",_codTarifa.value(CCodDiv));

            if(!wq1.exec())
            {
                _haveError = true;
                _error = wq1.lastError().text();
                qDebug() << wq1.boundValues();
                qDebug() << wq1.lastError().databaseText();
                qDebug() << wq1.lastQuery();
                qDebug() << wq1.lastError().text();
                qDebug() << wq1.executedQuery();
            }
        }

    }while(q.next());
    emit sizeOfTask(0);
    emit Progress(tr("Borrando datos temporales"),0);
    q.exec("DROP TABLE IF EXISTS d_Articulo");
    _cArticulos.clear();
    _stockArticulos.clear();
    _fams.clear();
    _prov.clear();
    _codTarifa.clear();
}

void importThread::run()
{
    _checkIva();
    if(importingFormPago()&& !_hardStop && !_haveError) { _importFormPago();}
    if(importingClientes() && !_hardStop && !_haveError){ _importClients();}
    if(importingProveedores() && !_hardStop && !_haveError){ _importProv();}
    if(importingArticulos() && !_hardStop && !_haveError){ _importArticulos();}

    if(_haveError && _error!="")
        emit Error(_error);
    //Reset vars after end
    emit sizeOfTask(1);
    emit Progress(tr("¡Terminado!"),1);
    _hardStop  = false;
    _haveError = false;
    _error = "";
}

bool importThread::importingClientes() const {
    return m_importClientes;
}

void importThread::importClientes(bool arg) {
    if (m_importClientes != arg) {
        m_importClientes = arg;
        emit importClientesChanged(arg);
    }
}

QString importThread::Path() const {
    return m_Path;
}

void importThread::setPath(QString arg) {
    if (m_Path != arg) {
        m_Path = arg;
        emit PathChanged(arg);
    }
}

void importThread::stopMe()
{
    _hardStop = true;
}

bool importThread::importingFormPago() const {
    return m_importFormPago;
}

void importThread::importFormPago(bool arg) {
    if (m_importFormPago != arg) {
        m_importFormPago = arg;
        emit importFormPagoChanged(arg);
    }
}

bool importThread::importingProveedores() const {
    return m_importProveedores;
}

void importThread::importProveedores(bool arg) {
    if (m_importProveedores != arg) {
        m_importProveedores = arg;
        emit importProveedoresChanged(arg);
    }
}

bool importThread::importingArticulos() const {
    return m_importArticulos;
}


void importThread::importArticulos(bool arg) {
    if (m_importArticulos != arg) {
        m_importArticulos = arg;
        emit importArticulosChanged(arg);
    }
}

void importThread::_checkIva()
{
    QSqlQuery query(QSqlDatabase::database("grupo"));
    if(query.exec("SELECT * FROM tiposiva"))
    {
        while(query.next())
        {
            QString key = query.record().value("nombre_interno").toString();
            _ivas.insert(key,query.record());
        }
    }
    if(_ivas.size()<4)
    {
        query.prepare("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base1', 'NORMAL'       ,    '21%', 21.00, 5.20);");
        query.exec();
        qDebug() << query.lastError();
        query.prepare("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base2', 'Reducido'     ,    '10%', 10.00, 1.40);");
        query.exec();
        qDebug() << query.lastError();
        query.prepare("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base3', 'Superreducido',     '4%',  4.00, 0.50);");
        query.exec();
        qDebug() << query.lastError();
        query.prepare("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base4', 'Exento'       , 'Exento',  0.00, 0.00);");

        if(query.exec())
            _checkIva();
        else
        {
            qDebug() << query.lastError();
            _haveError = true;
            _error = query.lastError().text();
        }
    }
}

int importThread::_getIdIva(QString cod)
{
    if(cod == "A")
        return _ivas.value("base2").value("id").toInt();
    if(cod == "S")
        return _ivas.value("base3").value("id").toInt();
    if(cod == "0" || cod == "O")
        return _ivas.value("base4").value("id").toInt();
    return _ivas.value("base1").value("id").toInt();
}

double importThread::_getRIva(QString cod)
{
    if(cod == "A")
        return _ivas.value("base2").value("nIVA").toDouble();
    if(cod == "S")
        return _ivas.value("base3").value("nIVA").toDouble();
    if(cod == "0" || cod == "O")
        return _ivas.value("base3").value("nIva").toDouble();
    return _ivas.value("base1").value("nIVA").toDouble();
}
QString importThread::_ValidarCC(QString Entidad, QString Oficina, QString CC)
{
    // Código Entidad & Código Oficina
    QString cEntidadOficina = Entidad + Oficina;
    int longitud = cEntidadOficina.length();
    if (longitud != 8)
        return "XX";
    int suma;
    suma = cEntidadOficina.mid(1, 1).toInt() * 4 + cEntidadOficina.mid(2, 1).toInt() * 8 + cEntidadOficina.mid(3, 1).toInt() * 5 +
            cEntidadOficina.mid(4, 1).toInt() * 10;
    suma = suma + cEntidadOficina.mid(5, 1).toInt() * 9 + cEntidadOficina.mid(6, 1).toInt() * 7 + cEntidadOficina.mid(7, 1).toInt() * 3 +
            cEntidadOficina.mid(8, 1).toInt() * 6;
    int resto = suma % 11;
    int primerdigito = 11 - resto;
    if (primerdigito == 10)
            primerdigito = 1;
    if (primerdigito == 11)
        primerdigito = 0;

    longitud = CC.length();
    if(longitud != 10)
        return "XX";
    suma = CC.mid( 1, 1).toInt() * 1 + CC.mid( 2, 1).toInt() * 2 + CC.mid( 3, 1).toInt() * 4 + CC.mid(4, 1).toInt() * 8;
    suma = suma + CC.mid(5, 1).toInt() * 5 + CC.mid(6, 1).toInt() * 10 + CC.mid(7, 1).toInt() * 9 + CC.mid( 8, 1).toInt() * 7;
    suma = suma + CC.mid(9, 1).toInt() * 3 + CC.mid(10, 1).toInt() * 6;
    resto = suma%11;
    int segundodigito = 11 - resto;
    if (segundodigito == 10)
        segundodigito = 1;
    if (segundodigito == 11)
        segundodigito = 0;

    //Digitos de Control
    QString cdc = QString::number(primerdigito) + QString::number(segundodigito);
    return cdc;
}

QSqlRecord importThread::empresa() const
{
    return _empresa;
}

void importThread::setEmpresa(const QSqlRecord &empresa)
{
    _empresa = empresa;
}

QHash<QString, int> importThread::divisas() const
{
    return _divisas;
}

void importThread::setDivisas(const QHash<QString, int> &divisas)
{
    _divisas = divisas;
}
