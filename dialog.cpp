#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setPage(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void Dialog::update_data(QString name, int pri, int sec, QString cuteqn, double cutabs, int status, double *x, double *y, int nostd)
{
    QString wave[6]={"Nil","405","450","490","630","OPN"};
    QString valid[2]={"PASS","INVALID"};

    ui->label_8->setText(name);

    ui->label_13->setText(wave[pri]);

    ui->label_14->setText(wave[sec]);

    ui->label_15->setText(cuteqn);
    ui->label_16->setText(QString::number(cutabs));

    ui->label_22->setText(QString::number(nostd));

    ui->label_17->setText(valid[status]);
    ui->label_23->setText(valid[status]);


    QLineSeries *series = new QLineSeries();

    for(int i=0;i<nostd;i++)
        series->append(x[i],y[i]);


    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout->addWidget(chartView);

//    QMainWindow window;
//    window.setCentralWidget(chartView);
//    window.resize(400, 300);
//    window.show();
}



void Dialog::on_pushButton_2_clicked()
{
    this->accept();
}
