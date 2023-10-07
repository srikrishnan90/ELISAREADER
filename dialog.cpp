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

    ui->label_8->clear();
    ui->label_13->clear();
    ui->label_14->clear();
    ui->label_15->clear();
    ui->label_16->clear();
    ui->label_22->clear();
    ui->label_17->clear();

    ui->label_8->setText(name);
    ui->label_13->setText(wave[pri]);
    ui->label_14->setText(wave[sec]);
    ui->label_15->setText(cuteqn);
    ui->label_16->setText(QString::number(cutabs));
    ui->label_22->setText(QString::number(nostd));
    ui->label_17->setText(valid[status]);
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
    ui->verticalLayout->removeWidget(chartView);
    ui->verticalLayout->addWidget(chartView);
}

void Dialog::update_results(QString well, QString samp, double abs, double avg_abs, QString result, QString unit, QString remarks, QString sid)
{

    ui->label_66->clear();
    ui->label_53->clear();
    ui->label_18->clear();
    ui->label_66->setText(well);
    ui->label_53->setText(well);
    ui->label_18->setText(well);

    ui->label_20->clear();
    ui->label_55->clear();
    ui->label_23->clear();
    ui->label_20->setText(samp);
    ui->label_55->setText(samp);
    ui->label_23->setText(samp);

    ui->label_45->clear();
    ui->label_37->clear();
    ui->label_59->clear();
    ui->label_45->setText(QString::number(abs,'f',3));
    ui->label_37->setText(QString::number(abs,'f',3));
    ui->label_59->setText(QString::number(abs,'f',3));

    ui->label_58->clear();
    ui->label_44->clear();
    ui->label_25->clear();
    ui->label_58->setText(QString::number(avg_abs,'f',3));
    ui->label_44->setText(QString::number(avg_abs,'f',3));
    ui->label_25->setText(QString::number(avg_abs,'f',3));

    ui->label_30->clear();
    ui->label_38->clear();
    ui->label_39->clear();
    ui->label_30->setText(result);
    ui->label_38->setText(result);
    ui->label_39->setText(result);

    ui->label_46->clear();
    ui->label_32->clear();
    ui->label_60->clear();
    ui->label_46->setText(unit);
    ui->label_32->setText(unit);
    ui->label_60->setText(unit);

    ui->label_62->clear();
    ui->label_48->clear();
    ui->label_34->clear();
    ui->label_62->setText(remarks);
    ui->label_48->setText(remarks);
    ui->label_34->setText(remarks);

    ui->pushButton_4->text().clear();
    ui->pushButton_5->text().clear();
    ui->pushButton_6->text().clear();
    ui->pushButton_4->setText(sid);
    ui->pushButton_5->setText(sid);
    ui->pushButton_6->setText(sid);

}



void Dialog::on_pushButton_2_clicked()
{
    this->accept();
}
