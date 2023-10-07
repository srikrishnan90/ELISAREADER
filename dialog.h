#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QMainWindow>
using namespace QtCharts;


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void setPage(int index);
    void update_data(QString name, int pri, int sec, QString cuteqn, double cutabs, int status, double *x, double *y, int nostd);

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
