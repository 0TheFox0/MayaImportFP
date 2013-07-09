#ifndef SETDIVISAS_H
#define SETDIVISAS_H

#include <QDialog>
#include <QHash>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
class setDivisas : public QDialog
{
    Q_OBJECT
public:
    explicit setDivisas(QString bdPath , QWidget *parent = 0);
    ~setDivisas();
    QHash<QString,int> getDivisas();
signals:
    
public slots:
private slots:
    void combo_changed(int i);
private:
 QHash<QString,int> _divisas;
 QHash<QComboBox*,QString> _combos;
 QVBoxLayout _layout;


};

#endif // SETDIVISAS_H
