#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>
#include "groupcreator.h"
#include <QItemSelectionModel>
#include <QItemSelection>
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

private slots:
    void on_btnSearchBD_clicked();

    void on_btnImportar_clicked();
    void reEnableImportar();

    void threadError(QString s);

    void on_btnConectar_clicked();

    void textChanged(QString s);

    void on_radioButton_toggled(bool checked);

    void selectionChanged(QItemSelection,QItemSelection);
    void on_btnTestNewGroup_clicked();
    void afterCreateGroup();

    void on_btnConectFP_clicked();

    void on_btnVolver_clicked();
    void grupo(QSqlRecord);
private:
    Ui::importDialog *ui;
    QDbfEditor* editor;
    bool setUpDB(QString host, QString user, QString pass);
    void _createNewGroup();
    bool _conected;
    bool _newGroup;

    QSqlRecord _empresaFp;
    QSqlRecord _grupoMaya;

    QHash<QString,int> _divisas;
    QHash<QComboBox*,QString> _combos;

    QHash<QString,int> _ivas;

    QString _empDir;
};

#endif // IMPORTDIALOG_H
