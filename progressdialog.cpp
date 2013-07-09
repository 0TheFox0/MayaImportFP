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
    ui->progressBar->setMaximum(max);
}

void progressDialog::end()
{
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->pushButton->setText(tr("Aceptar"));
    ui->label->setText(tr("Terminado"));
}

void progressDialog::on_pushButton_clicked()
{
    if(ui->pushButton->text() == tr("Cancelar"))
    {
        if(QMessageBox::question(this,tr("¿Está realmente seguro?"),
                              tr("Cancelar la importación en medio del proceso puede tener consecuencias impredecibles.\n"
                                 "Cancele solo si está realmente seguro, y siempre bajo su responsabilidad"),tr("Si"),tr("No"))
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
