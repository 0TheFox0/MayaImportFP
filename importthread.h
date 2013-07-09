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

signals:
    
    void importClientesChanged(bool arg);

    void PathChanged(QString arg);

    void Error(QString s);

    void Progress(QString message, int currentValue);
    void sizeOfTask(int max);
    void importFormPagoChanged(bool arg);
    void importProveedoresChanged(bool arg);

    void importArticulosChanged(bool arg);

public slots:

    void importClientes(bool arg);

    void setPath(QString arg);

    void stopMe();

    void importFormPago(bool arg);

    void importProveedores(bool arg);

    void importArticulos(bool arg);

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

    QHash<QString,int> _fpago;
    QHash<QString,QString> _cCliente;
    QHash<QString,QString> _cArticulos;
    QHash<QString,double> _stockArticulos;
    QHash<QString,int> _prov;
    QHash<QString,int> _fams;
    QHash<QString,QSqlRecord> _ivas;
    QHash<QString,int> _divisas;
    QHash<QString,int> _codTarifa;
    bool _hardStop;
    bool _haveError;
    bool m_importFormPago;

    QString _error;  
    MainWindow* _mw;
    bool m_importProveedores;
    bool m_importArticulos;
    QString _ValidarCC(QString Entidad, QString Oficina, QString CC);
};

#endif // IMPORTTHREAD_H
