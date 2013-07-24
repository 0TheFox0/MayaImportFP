#include "progressdialog.h"
#include "ui_progressdialog.h"
#include <QApplication>
progressDialog::progressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progressDialog)
{
    ui->setupUi(this);
}

progressDialog::~progressDialog()
{
    delete ui;
}

void progressDialog::error(QString)
{
    ui->pushButton->setText(tr("Aceptar"));
    ui->label->setText(tr("¡Error!"));
}

void progressDialog::Progress(QString message, int currentValue)
{
    ui->label->setText(message);
    ui->progressBar->setValue(currentValue);
}

void progressDialog::sizeOfTask(int max)
{
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(max);
}

void progressDialog::end()
{
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(1);
    ui->pushButton->setText(tr("Aceptar"));
    ui->label->setText(QString::fromUtf8("¡Terminado!"));
}

void progressDialog::on_pushButton_clicked()
{
    if(ui->pushButton->text() == tr("Cancelar"))
    {
        if(QMessageBox::question(this,QString::fromUtf8("¿Está realmente seguro?"),
                              QString::fromUtf8("Cancelar la importación en medio del proceso puede tener consecuencias impredecibles.\n"
                                 "Cancele solo si está realmente seguro, y siempre bajo su responsabilidad"),tr("No"),tr("Si"))
                == QMessageBox::Accepted)
        {
            ui->label->setText(tr("Cancelando..."));
            emit stop();
        }
    }
    else
        this->hide();
}


void progressDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
