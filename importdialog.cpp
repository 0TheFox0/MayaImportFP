#include "importdialog.h"
#include "ui_importdialog.h"
#include "importthread.h"
#include "progressdialog.h"
#include "src/mainwindow.h"
#include "setdivisas.h"
importDialog::importDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importDialog)
{
    ui->setupUi(this);
    QList<QCheckBox *> cbs = this->findChildren<QCheckBox *>();
    QList<QCheckBox *>::Iterator i;
    for(i=cbs.begin();i!=cbs.end();++i)
    {
        connect((*i),SIGNAL(toggled(bool)),this,SLOT(checkBox_toggled(bool)));
    }
    connect(ui->txtHost,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(ui->txtUser,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(ui->txtPass,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    _conected = false;

#if _DEBUG
    ui->txtRutaBD->setText("C:\\Users\\marco\\Desktop\\DbAgupunt\\DBF13");
    ui->txtHost->setText("localhost");
    ui->txtUser->setText("root");
    ui->txtPass->setText("marco");
    ui->btnConectar->setEnabled(true);
#endif

}

importDialog::~importDialog()
{
    delete ui;
}

void importDialog::on_btnSearchBD_clicked()
{
    QString route ="";
    route = QFileDialog::getExistingDirectory (this, tr("Ruta de la base de datos"));
    ui->txtRutaBD->setText(route);
}

bool importDialog::anyImport()
{
    QList<QCheckBox *> cbs = this->findChildren<QCheckBox *>();
    QList<QCheckBox *>::Iterator i;
    bool anyCheck = false;
    for(i=cbs.begin();i!=cbs.end();++i)
    {
        if((*i)->isChecked())
        {
            anyCheck = true;
            break;
        }
    }
    return anyCheck;
}

void importDialog::checkBox_toggled(bool checked)
{
    if(checked)
    {
        ui->btnImportar->setEnabled(true && _conected);
        if(sender()==ui->clientCheck || sender()==ui->provCheck)
        {
            ui->fPagoCheck->setChecked(true);
            ui->fPagoCheck->setEnabled(false);
        }
        if(sender()==ui->articulosCheck)
        {
            setDivisas d(ui->txtRutaBD->text(),this);
            d.exec();
            _divisas = d.getDivisas();
            ui->provCheck->setChecked(true);
            ui->provCheck->setEnabled(false);
        }
    }
    else
    {
        if(sender()==ui->clientCheck && !ui->provCheck->isChecked())
            ui->fPagoCheck->setEnabled(true);
        if(sender()==ui->provCheck && !ui->clientCheck->isChecked())
            ui->fPagoCheck->setEnabled(true);
        if(sender()==ui->articulosCheck)
            ui->provCheck->setEnabled(true);

        bool anyCheck = anyImport();
        ui->btnImportar->setEnabled(anyCheck && _conected);
    }
}

void importDialog::on_btnImportar_clicked()
{
    QString s = ui->txtRutaBD->text()+"/FPago.dbf";
    MainWindow* mainWin = new MainWindow(s);
    ui->btnImportar->setEnabled(false);
    importThread * Thread = new importThread(mainWin , this);
    Thread->setPath(ui->txtRutaBD->text());

    Thread->importClientes(ui->clientCheck->isChecked());
    Thread->importFormPago(ui->fPagoCheck->isChecked());
    Thread->importProveedores(ui->provCheck->isChecked());
    Thread->importArticulos(ui->articulosCheck->isChecked());

    connect(Thread,SIGNAL(finished()),Thread,SLOT(deleteLater()));
    connect(Thread,SIGNAL(finished()),this,SLOT(reEnableImportar()));
    connect(Thread,SIGNAL(Error(QString)),this,SLOT(threadError(QString)));

    progressDialog * d = new progressDialog (this);
    d->setModal(true);
    connect(Thread,SIGNAL(Error(QString)),d,SLOT(error(QString)));
    connect(Thread,SIGNAL(Progress(QString,int)),d,SLOT(Progress(QString,int)));
    connect(Thread,SIGNAL(sizeOfTask(int)),d,SLOT(sizeOfTask(int)));
    connect(Thread,SIGNAL(finished()),d,SLOT(end()));
    connect(d,SIGNAL(stop()),Thread,SLOT(stopMe()));
    d->show();

    QSqlQueryModel* m = (QSqlQueryModel*)(ui->listView->model());
    Thread->setEmpresa(m->record(ui->listView->currentIndex().row()));
    Thread->setDivisas(_divisas);
    Thread->start();
}

void importDialog::reEnableImportar()
{
    ui->btnImportar->setEnabled(true);
}

void importDialog::threadError(QString s)
{
    QMessageBox::critical(this,tr("Error en la importación"),s);
}

void importDialog::on_btnConectar_clicked()
{
    if(setUpDB(ui->txtHost->text(),
                    ui->txtUser->text(),
                    ui->txtPass->text()))
    {
        _conected = true;
        bool anyCheck = anyImport();
        ui->btnImportar->setEnabled(anyCheck && _conected);

        QSqlQueryModel * q = new QSqlQueryModel(this);
        q->setQuery("SELECT * FROM mayaglobal.grupos;",QSqlDatabase::database("Global"));

        ui->comboGrupo->setModel(q);
        ui->comboGrupo->setModelColumn(1);
    }
    ui->groupBoxDatos->setEnabled(!ui->txtRutaBD->text().isEmpty() && _conected);
}

void importDialog::textChanged(QString s)
{
    Q_UNUSED(s);
    bool b = !ui->txtHost->text().isEmpty() &&
             !ui->txtUser->text().isEmpty() &&
             !ui->txtPass->text().isEmpty();
    ui->btnConectar->setEnabled(b);
}
bool importDialog::setUpDB(QString host, QString user, QString pass)
{
    QSqlDatabase writerDb(QSqlDatabase::addDatabase("QMYSQL","Global"));
    writerDb.setHostName(host);
    writerDb.setUserName(user);
    writerDb.setPassword(pass);
    if(!writerDb.open())
    {
        QSqlError err = writerDb.lastError ();
        QMessageBox::critical(this,tr("The database reported an error:"),err.databaseText());
        return false;
    }
    return writerDb.isValid();
}

void importDialog::on_txtRutaBD_textChanged(const QString &arg1)
{
    QList<QCheckBox *> cbs = this->findChildren<QCheckBox *>();
    QList<QCheckBox *>::Iterator i;
    for(i=cbs.begin();i!=cbs.end();++i)
    {
        (*i)->setChecked(false);
    }
    ui->groupBoxDatos->setEnabled(!arg1.isEmpty() && _conected);
}

void importDialog::on_comboGrupo_currentIndexChanged(int index)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","grupo");

    QAbstractItemModel * model = ui->comboGrupo->model();
    QString bd_name = model->data(model->index(index,2)).toString();
    QString bd_host = model->data(model->index(index,5)).toString();
    QString bd_user = model->data(model->index(index,6)).toString();
    QString bd_pass = model->data(model->index(index,7)).toString();
    int bd_port = model->data(model->index(index,8)).toInt();

    db.setHostName(bd_host);
    db.setUserName(bd_user);
    db.setPassword(bd_pass);
    db.setPort(bd_port);
    db.setDatabaseName(bd_name);
    if(db.open())
    {
        QSqlQueryModel * q = new QSqlQueryModel(this);
        q->setQuery("SELECT * FROM empresas;",db);

        ui->listView->setModel(q);
        ui->listView->setModelColumn(2);
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("No se puede conectar con la base de datos"));
    }
}
