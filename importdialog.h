#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>
#include "src/dbfeditor.h"
namespace Ui {
class importDialog;
}

class importDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit importDialog(QWidget *parent = 0);
    ~importDialog();
    
    bool anyImport();
private slots:
    void on_btnSearchBD_clicked();

    void checkBox_toggled(bool checked);

    void on_btnImportar_clicked();
    void reEnableImportar();

    void threadError(QString s);
    void on_btnConectar_clicked();

    void textChanged(QString s);
    void on_txtRutaBD_textChanged(const QString &arg1);

    void on_comboGrupo_currentIndexChanged(int index);

private:
    Ui::importDialog *ui;
    QDbfEditor* editor;
    bool setUpDB(QString host, QString user, QString pass);
    bool _conected;
    QHash<QString,int> _divisas;
};

#endif // IMPORTDIALOG_H
