#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTime>
#include <QTimer>
namespace Ui {
class progressDialog;
}

class progressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit progressDialog(QWidget *parent = 0);
    ~progressDialog();
public slots:
    void error(QString);
    void Progress(QString message, int currentValue);
    void sizeOfTask(int max);
    void end();
private slots:
    void on_pushButton_clicked();
    void updateTime();
signals:
    void stop();
private:
    Ui::progressDialog *ui;
    void closeEvent(QCloseEvent *e );
    QTime t;
    QTimer timer;
};

#endif // PROGRESSDIALOG_H
