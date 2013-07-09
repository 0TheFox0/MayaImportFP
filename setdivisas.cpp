#include "setdivisas.h"
#include "src/mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlRecord>
setDivisas::setDivisas(QString bdPath, QWidget *parent) :
    QDialog(parent),
    _layout(this)
{
    MainWindow w(bdPath + "\\Divisas.dbf");
    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    if(!q.exec("Select * from d_Divisas"))
    {
        QMessageBox::critical(this,"Error",q.lastError().text());
        return;
    }
    int rows = 0;
    while(q.next())
        rows++;

    if(rows==0)
        return;

    rows = 0;
    q.first();

    QSqlQueryModel * q1 = new QSqlQueryModel(this);
    q1->setQuery("SELECT * FROM monedas;",QSqlDatabase::database("grupo"));

    QLabel * l = new QLabel(tr("Selecciona la relacion entre divisas"),this);
    _layout.addWidget(l);
    _layout.addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum));
    do
    {
        rows++;
        QSqlRecord r = q.record();
        QString codigo = r.value("CCODDIV").toString().trimmed();
        QString desc = r.value("CDETDIV").toString().trimmed();

        QComboBox * combo = new QComboBox(this);
        combo->setModel(q1);
        combo->setModelColumn(1);
        connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(combo_changed(int)));

        QLabel * label = new QLabel(desc,this);

        QHBoxLayout * lay = new QHBoxLayout(this);

        lay->addWidget(label);
        lay->addWidget(combo);
        //lay->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));


        _layout.addLayout(lay);
        _combos.insert(combo,codigo);
        combo->setCurrentIndex(0);

    }while(q.next());

    _layout.addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding));
    QPushButton * btn = new QPushButton(tr("Aceptar"),this);
    connect(btn,SIGNAL(clicked()),this,SLOT(accept()));
    _layout.addWidget(btn);
}

setDivisas::~setDivisas()
{
    QSqlQuery q(QSqlDatabase::database("dbfEditor"));
    q.exec("DROP TABLE IF EXISTS d_Divisas");
}

QHash<QString, int> setDivisas::getDivisas()
{
    QHash<QComboBox*,QString>::Iterator i;
    for(i=_combos.begin();i!=_combos.end();++i)
    {
        //const QComboBox * c = i.key();
        //const QAbstractItemModel * m = c->model();
        int id = i.key()->model()->data(i.key()->model()->index(i.key()->currentIndex(),0)).toInt();
        _divisas[i.value()] = id;
    }
    return _divisas;
}

void setDivisas::combo_changed(int i)
{
    QComboBox * combo = qobject_cast<QComboBox*>(sender());
    int id = combo->model()->data(combo->model()->index(i,0)).toInt();
    _divisas[_combos.value(combo)] = id;
}
