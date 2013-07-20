#ifndef IMPORTTHREAD_H
#define IMPORTTHREAD_H

#include <QThread>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QHash>
#include <QDebug>
#include "src/mainwindow.h"
class importThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString Path READ Path WRITE setPath NOTIFY PathChanged)
    Q_PROPERTY(bool importClientes READ importingClientes WRITE importClientes NOTIFY importClientesChanged)
    Q_PROPERTY(bool importFormPago READ importingFormPago WRITE importFormPago NOTIFY importFormPagoChanged)
    Q_PROPERTY(bool importProveedores READ importingProveedores WRITE importProveedores NOTIFY importProveedoresChanged)
    Q_PROPERTY(bool importArticulos READ importingArticulos WRITE importArticulos NOTIFY importArticulosChanged)
    Q_PROPERTY(bool createNewGroup READ createNewGroup WRITE setcreateNewGroup NOTIFY createNewGroupChanged)
public:
    explicit importThread(MainWindow * mw, QObject *parent = 0);
    ~importThread();
    void run();

    bool importingClientes() const;

    QString Path() const;
    bool importingFormPago() const;


    bool importingProveedores() const;

    bool importingArticulos() const;

    QSqlRecord empresa() const;
    void setEmpresa(const QSqlRecord &empresa);

    QHash<QString, int> divisas() const;
    void setDivisas(const QHash<QString, int> &divisas);

    bool createNewGroup() const;

    QHash<QString, int> ivaRelation() const;
    void setIvaRelation(const QHash<QString, int> &ivaRelation);

signals:
    
    void importClientesChanged(bool arg);

    void PathChanged(QString arg);

    void Error(QString s);

    void Progress(QString message, int currentValue);
    void sizeOfTask(int max);
    void importFormPagoChanged(bool arg);
    void importProveedoresChanged(bool arg);

    void importArticulosChanged(bool arg);

    void createNewGroupChanged(bool arg);

public slots:

    void importClientes(bool arg);

    void setPath(QString arg);

    void stopMe();

    void importFormPago(bool arg);

    void importProveedores(bool arg);

    void importArticulos(bool arg);

    void setcreateNewGroup(bool arg);

private:
    bool m_importClientes;
    QString m_Path;
    QSqlRecord _empresa;

    void _checkIva();
    int _getIdIva(QString cod);
    double _getRIva(QString cod);
    void _importFormPago();

    void _importClients();
    void _importComenCli();

    void _importProv();
    void _importComentProv();

    void _importFamilias();
    void _importComentArticulos1();
    void _importComentArticulos2();
    void _importStocks();
    void _generarCodigosTarifa();
    void _importArticulos();

    void _importPresCli();
    void _importPedCli();
    void _importAlbCli();
    void _importFacCli();
    QHash<QString,int> _articulos;// CREF,id
    QHash<QString,QSqlRecord> _clientes;// CCODCLI,id
    QHash<QString,int> _fpago;
    QHash<QString,QString> _sFPago;
    QHash<QString,QString> _cCliente;
    QHash<QString,QString> _cArticulos;
    QHash<QString,double> _stockArticulos;
    QHash<QString,int> _prov;
    QHash<QString,int> _fams;
    QHash<QString,QSqlRecord> _ivas;
    QHash<QString,int> _ivaRelation;
    QHash<QString,int> _divisas;
    QHash<QString,int> _codTarifa;
    QHash<QString,QString> _pedidoFromAlb;
    bool _hardStop;
    bool _haveError;
    bool m_importFormPago;

    QString _error;  
    MainWindow* _mw;
    bool m_importProveedores;
    bool m_importArticulos;
    QString _ValidarCC(QString Entidad, QString Oficina, QString CC);
    bool m_createNewGroup;
};

#endif // IMPORTTHREAD_H
