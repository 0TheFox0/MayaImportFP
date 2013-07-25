#include "importdialog.h"
#include "ui_importdialog.h"
#include "importthread.h"
#include "progressdialog.h"
#include "src/mainwindow.h"
#include "setdivisas.h"
#include <QValidator>
importDialog::importDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importDialog)
{
    qRegisterMetaType<QSqlRecord>("QSqlRecord");
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("marco");
    db.open();

    connect(ui->txtHost,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(ui->txtUser,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(ui->txtPass,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    _conected = false;
    _newGroup = true;

#if _DEBUG
    ui->txtRutaBD->setText("C:/GrupoSP/GES2012/");
    ui->txtHost->setText("localhost");
    ui->txtUser->setText("root");
    ui->txtPass->setText("marco");
    ui->btnConectar->setEnabled(true);
#endif

    ui->txtNewGName->setValidator(new QRegExpValidator(QRegExp("([A-Z]*[a-z]*[0-9]*)*")));
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




void importDialog::_getRelations()
{
    QHash<QComboBox*,QString>::Iterator i;
    for(i=_combos.begin();i!=_combos.end();++i)
    {
        if(_validDivisas.value(i.key())->isChecked())
        {
            int id = i.key()->model()->data(i.key()->model()->index(i.key()->currentIndex(),0)).toInt();
            _divisas[i.value()] = id;
        }
    }

    QHash<QComboBox*,QString>::Iterator it;
    for(it=_combos2.begin();it!=_combos2.end();++it)
    {
        int id = it.key()->model()->data(it.key()->model()->index(it.key()->currentIndex(),0)).toInt();
        _ivas[it.value()] = id;
    }

    QHash<QComboBox*,QString>::Iterator it2;
    for(it2=_combos3.begin();it2!=_combos3.end();++it2)
    {
        int id = it2.key()->model()->data(it2.key()->model()->index(it2.key()->currentIndex(),0)).toInt();
        _paises[it2.value()] = id;
    }
    qDebug() << _paises;
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
    ui->btnImportar->setText("Importar");
    ui->btnImportar->setEnabled(true);
}

void importDialog::on_btnImportar_clicked()
{
    if(ui->btnImportar->text() != "Importar")
    {
        ui->btnImportar->setEnabled(false);
        switch (ui->stackedWidget->currentIndex()) {
        case 3:
            _createNewGroup();
            break;
        case 4:
        case 5:
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
            ui->btnImportar->setEnabled(true);
            break;
        case 6:
            _getRelations();
            break;
        default:
            ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
            break;
        }
    }
    else
    {
        QString s = _empDir+"/Fpago.dbf";
        MainWindow* mainWin = new MainWindow(s);
        ui->btnImportar->setEnabled(false);
        importThread * Thread = new importThread(mainWin , this);
        Thread->setPath(_empDir);
        Thread->setPathConta(_contaDir);

        Thread->setIvaRelation(_ivas);
        Thread->setPaisRelation(_paises);

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

        Thread->setEmpresa(_empresaFp);
        Thread->setDivisas(_divisas);
        Thread->start();
    }
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
        ui->btnImportar->setEnabled(_conected);

        QSqlQueryModel * q = new QSqlQueryModel(this);
        q->setQuery("SELECT * FROM mayaglobal.grupos;",QSqlDatabase::database("Global"));

        ui->listView->setModel(q);
        ui->listView->setModelColumn(1);

        connect(ui->listView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
    }
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

void importDialog::_createNewGroup()
{
    GroupCreator * g = new GroupCreator(this);

    progressDialog * d = new progressDialog (this);
    d->setModal(true);
    connect(g,SIGNAL(Error(QString)),this,SLOT(grupoError(QString)));
    connect(g,SIGNAL(Progress(QString,int)),d,SLOT(Progress(QString,int)));
    connect(g,SIGNAL(sizeOfTask(int)),d,SLOT(sizeOfTask(int)));
    connect(g,SIGNAL(finished()),d,SLOT(deleteLater()));
    connect(g,SIGNAL(finished()),this,SLOT(afterCreateGroup()));
    connect(g,SIGNAL(insertedGrupo(QSqlRecord)),this,SLOT(grupo(QSqlRecord)));
    d->show();

    g->_full = _newGroup;
    if(_newGroup)
        g->_nombre = ui->txtNewGName->text();
    else
        g->_nombre = _grupoMaya.value("nombre").toString();

    g->_empresaFp = _empresaFp;

    g->start();
}

void importDialog::on_radioButton_toggled(bool checked)
{
    ui->frameExis->setEnabled(checked);
    ui->frameNew->setEnabled(!checked);
    _newGroup = !checked;
    ui->btnImportar->setEnabled(false);
}

void importDialog::selectionChanged(QItemSelection, QItemSelection)
{
    ui->btnImportar->setEnabled(true);
    if(sender()==ui->listViewFP->selectionModel())
    {
        QSqlQueryModel * model = qobject_cast<QSqlQueryModel*>(ui->listViewFP->model());
        _empresaFp = model->record(ui->listViewFP->currentIndex().row());
        _empDir = ui->txtRutaBD->text();
        _empDir.append("/DBF");
        _empDir.append(_empresaFp.value("CODEMP").toString().trimmed());

        _contaDir = ui->txtRutaConta->text();
        _contaDir.append("/EMP");
        _contaDir.append(_empresaFp.value("CODEMP").toString().trimmed());
    }
    else
    {

        QSqlQueryModel * model = qobject_cast<QSqlQueryModel*>(ui->listView->model());
        int index = ui->listView->currentIndex().row();
        _grupoMaya = model->record(index);

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","grupo");

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
        if(!db.open())
        {
            QMessageBox::critical(this,tr("Error"),tr("No se puede conectar con la base de datos"));
        }
    }
}

void importDialog::on_btnTestNewGroup_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","grupo");

    QString bd_host = ui->txtNewGHost->text();
    QString bd_user = ui->txtNewGUser->text();
    QString bd_pass = ui->txtNewGPass->text();
    int bd_port = ui->txtNewGPort->text().toInt();

    db.setHostName(bd_host);
    db.setUserName(bd_user);
    db.setPassword(bd_pass);
    db.setPort(bd_port);
    if(db.open())
    {
        ui->btnImportar->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("No se puede conectar con la base de datos"));
    }
}

void importDialog::afterCreateGroup()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
    ui->btnImportar->setEnabled(true);

    MainWindow w(_empDir + "/Divisas.dbf");
    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Divisas"))
    {
        QMessageBox::critical(this,"Error",q.lastError().text());
        return;
    }
    q.first();

    QSqlQueryModel * q1 = new QSqlQueryModel(this);
    q1->setQuery("SELECT * FROM monedas;",QSqlDatabase::database("grupo"));

    QWidget* container = new QWidget(this);
    QVBoxLayout * _layout = new QVBoxLayout(container);
    do
    {
        QSqlRecord r = q.record();
        QString codigo = r.value("CCODDIV").toString().trimmed();
        QString desc = r.value("CDETDIV").toString().trimmed();

        QComboBox * combo = new QComboBox(this);
        combo->setModel(q1);
        combo->setModelColumn(1);

        QLabel * label = new QLabel(desc,this);

        QHBoxLayout * lay = new QHBoxLayout(this);

        QCheckBox * check = new QCheckBox(this);
        check->setChecked(true);

        lay->addWidget(check);
        lay->addWidget(label);
        lay->addWidget(combo);

        _layout->addLayout(lay);
        _combos.insert(combo,codigo);
        _validDivisas.insert(combo,check);
        combo->setCurrentIndex(0);

    }while(q.next());
    _layout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
    container->setLayout(_layout);
    ui->scrollArea->setWidget(container);

    w.openDb(_empDir + "/Ivas.dbf");
    if(!q.exec("Select * from d_Ivas"))
    {
        QMessageBox::critical(this,"Error",q.lastError().text());
        return;
    }
    q.first();

    QSqlQueryModel * q2 = new QSqlQueryModel(this);
    q2->setQuery("SELECT * FROM tiposiva;",QSqlDatabase::database("grupo"));

    QWidget* container2 = new QWidget(this);
    QVBoxLayout * _layout2 = new QVBoxLayout(container);
    do
    {
        QSqlRecord r = q.record();
        QString codigo = r.value("CTIPOIVA").toString().trimmed();
        QString desc = r.value("CDETIVA").toString().trimmed();

        QComboBox * combo = new QComboBox(this);
        combo->setModel(q2);
        combo->setModelColumn(2);

        QLabel * label = new QLabel(desc,this);

        QHBoxLayout * lay = new QHBoxLayout(this);

        lay->addWidget(label);
        lay->addWidget(combo);

        _layout2->addLayout(lay);
        _combos2.insert(combo,codigo);
        combo->setCurrentIndex(0);

    }while(q.next());
    _layout2->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
    container2->setLayout(_layout2);
    ui->scrollIva->setWidget(container2);

    w.openDb(ui->txtRutaBD->text() + "/dbf/Naciones.dbf");
    if(!q.exec("Select * from d_Naciones"))
    {
        QMessageBox::critical(this,"Error",q.lastError().text());
        return;
    }
    q.first();

    QSqlQueryModel * q3 = new QSqlQueryModel(this);
    q3->setQuery("SELECT * FROM paises;",QSqlDatabase::database("grupo"));

    QWidget* container3 = new QWidget(this);
    QVBoxLayout * _layout3 = new QVBoxLayout(container);
    do
    {
        QSqlRecord r = q.record();
        QString codigo = r.value("CCODIGO").toString().trimmed();
        QString desc = r.value("CNOMBRE").toString().trimmed();

        QComboBox * combo = new QComboBox(this);
        combo->setModel(q3);
        combo->setModelColumn(1);

        QLabel * label = new QLabel(desc,this);

        QHBoxLayout * lay = new QHBoxLayout(this);

        lay->addWidget(label);
        lay->addWidget(combo);

        _layout3->addLayout(lay);
        _combos3.insert(combo,codigo);
        combo->setCurrentIndex(0);

    }while(q.next());
    _layout3->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
    container3->setLayout(_layout3);
    ui->scrollPaises->setWidget(container3);
}

void importDialog::on_btnConectFP_clicked()
{
    MainWindow w(ui->txtRutaBD->text()+"/dbf/Empresa.dbf");
    QSqlQueryModel * q = new QSqlQueryModel(this);
    q->setQuery("SELECT * from d_Empresa;",QSqlDatabase::database("dbfEditor"));
    ui->listViewFP->setModel(q);
    ui->listViewFP->setModelColumn(2);

    connect(ui->listViewFP->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
}

void importDialog::on_btnVolver_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
    if(ui->stackedWidget->currentIndex()!=0)
        ui->btnImportar->setEnabled(false);
    else
        ui->btnImportar->setEnabled(true);
}

void importDialog::grupo(QSqlRecord r)
{
    _grupoMaya = r;
}

void importDialog::grupoError(QString s)
{
    QMessageBox::critical(this,"Error",s);
}

void importDialog::on_btnSearchBDConta_clicked()
{
    QString route ="";
    route = QFileDialog::getExistingDirectory (this, tr("Ruta de la base de datos"));
    ui->txtRutaConta->setText(route);
}
