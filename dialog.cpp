#include "dialog.h"
#include "ui_dialog.h"

static QString samp_val,pid,d_name,d_well,d_date,d_time,d_res,d_unit,d_rem,d_abs,d_avg_abs;
static QTextDocument doc;
static QChartView *chartView;


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

QString Dialog::getdiaData()
{
    return pid;
}

void Dialog::update_data(QString name, int pri, int sec, QString cuteqn, double cutabs, int status, double *x, double *y, int nostd, QString date, QString time)
{
    d_name=name;
    d_date=date;
    d_time=time;

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
    QPen black(Qt::black);
    QPen black1(Qt::black);
    black.setWidth(4);
    black1.setWidth(2);
    series->setPen(black);

    QGraphicsRectItem * pItem = new QGraphicsRectItem(0, 0, 0, 0, chart->graphicsItem());
    pItem->setPen(QPen(Qt::red));
    pItem->setZValue(3); // points on QScatterSeries covers  this item and this item covers grid lines

    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(name);
    chart->axisX()->setTitleText("Conc.");
    chart->axisY()->setTitleText("ABS");
    //chart->axisX()->setShadesPen(black);
    //chart->axisY()->setShadesPen(black);
    chart->axisX()->setGridLinePen(black1);
    chart->axisY()->setGridLinePen(black1);
    chart->axisX()->setGridLineColor(QColor(0,0,0));
    chart->axisY()->setGridLineColor(QColor(0,0,0));
    QFont labelsFont;
    labelsFont.setPixelSize(12);
    chart->axisX()->setLabelsFont(QFont( fontInfo().family(), 14, QFont::Bold ));
    chart->axisY()->setLabelsFont(labelsFont);
    chart->axisX()->setTitleFont(labelsFont);
    chart->axisY()->setTitleFont(labelsFont);




    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //chartView->setGeometry();
    ui->verticalLayout->removeWidget(chartView);
    ui->verticalLayout->addWidget(chartView);
    //    chartView->grab().save("/home/pi/123.png");
    //    QImage img("/home/pi/123.png");

    //    QPrinter printer(QPrinter::ScreenResolution);
    //    printer.setPrinterName("Internal");
    //        //printer.setOutputFormat(QPrinter::PdfFormat);
    //        printer.setFullPage(true);
    //        printer.setPageOrientation(QPageLayout::Landscape);
    //        //printer.setOutputFileName("/home/pi/test.pdf");
    //        printer.setPageMargins(QMarginsF(0,0,0,0), QPageLayout::Point);
    //        printer.setPageSize(QPageSize(QSizeF(400,700), QPageSize::Point));

    //    QPainter painter(&printer);
    //    //painter.setRenderHint(QPainter::Antialiasing);
    //    //chartView->scene()->render(&painter);
    //    painter.drawImage(QPoint(0,0),img);
    //    painter.end();


}

void Dialog::update_results(QString well, QString samp, double abs, double avg_abs, QString result, QString unit, QString remarks, QString sid)
{
    samp_val=samp;
    pid=sid;
    d_well=well;
    d_abs=QString::number(abs,'f',3);
    d_avg_abs=QString::number(avg_abs,'f',3);
    d_res=result;
    d_unit=unit;
    d_rem=remarks;
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



void Dialog::on_pushButton_4_clicked()
{
    keyboard *keyb=new keyboard(this);
    keyb->setModal(true);
    keyb->setPage(0);
    keyb->setData("Enter PID/SID for "+samp_val,ui->pushButton_4->text());
    keyb->exec();
    pid = keyb->getData();
    ui->pushButton_4->setText(pid);
}

void Dialog::on_pushButton_5_clicked()
{
    keyboard *keyb=new keyboard(this);
    keyb->setModal(true);
    keyb->setPage(0);
    keyb->setData("Enter PID/SID for "+samp_val,ui->pushButton_5->text());
    keyb->exec();
    pid = keyb->getData();
    ui->pushButton_5->setText(pid);
}

void Dialog::on_pushButton_6_clicked()
{
    keyboard *keyb=new keyboard(this);
    keyb->setModal(true);
    keyb->setPage(0);
    keyb->setData("Enter PID/SID for "+samp_val,ui->pushButton_6->text());
    keyb->exec();
    pid = keyb->getData();
    ui->pushButton_6->setText(pid);
}

void Dialog::on_pushButton_3_clicked()
{
    QStringList head,val;
    head<<"WELL"<<"SAMP"<<"ABS"<<"AVG"<<"RESULT"<<"UNIT"<<"REM"<<"PID";
    val<<d_well<<samp_val<<d_abs<<d_avg_abs<<d_res<<d_unit<<d_rem<<pid;
    doc.clear();
    QString text("<head><style>table, th, td {border: 1px solid black;border-collapse: collapse;text-align:left;}table.center { margin-left: auto; margin-right: auto;}</style></head>");
    text.append("<body>");
    text.append("<h1 style='font-size:10px;text-align:center'>");
    text.append(d_name).append("<br>").append(d_date).append("<br>").append(d_time);
    text.append("</h1>");
    text.append("<table class='center' style='width:30%'>");
    for (int i = 0; i < head.length(); i++)
    {
        text.append("<tr>");
        //text.append("<th style='text-align: center'>").append(head[i]).append(" ").append("</th>");
        //text.append("<td style='text-align: center'>").append(val[i]).append(" ").append("</td>");
        text.append("<th>").append(head[i]).append(" ").append("</th>");
        if(val[i][0]=="<")
            val[i].replace("<","&lt;");//since html not considering < as character, it affect the printer
        text.append("<td>").append(val[i]).append(" ").append("</td>");
        text.append("</tr>");
    }
    text.append("</table></body>");
    doc.setHtml(text);
    print_process(65,1,0);
}

void Dialog::print_process(int paper_length, int individual, int graph)
{
    QPrinter printer;
    QString printername;
    QSqlQuery Query;
    Query.prepare("select * FROM settings WHERE sno = 1");
    Query.exec();
    while(Query.next())
    {
        printername=Query.value("printer").toString();
    }

    printer.setPrinterName(printername);
    if(printername=="Internal")
    {
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageOrder(QPrinter::LastPageFirst);//not working, need to check
        printer.setPaperSize(QSize(58, paper_length),QPrinter::Millimeter);//paper_length 84 for table and 125 for matrix
        if(individual==1)
        {
            printer.setOrientation(QPrinter::Portrait);
            printer.setPaperSize(QSize(58, paper_length),QPrinter::Millimeter);//paper_length 84 for table and 125 for matrix
        }
        QFont font;
        font.setPointSize(6);
        font.setBold(QFont::DemiBold);
        font.setFamily("Calibri");
        font.setLetterSpacing(QFont::PercentageSpacing,100);
        doc.setDefaultFont(font);
    }
    else
    {
        printer.setOrientation(QPrinter::Portrait);
        printer.setPaperSize(QPrinter::A4);
        printer.setPageOrder(QPrinter::FirstPageFirst);//not working, need to check
        QFont font;
        font.setPointSize(10);
        font.setBold(QFont::DemiBold);
        font.setFamily("Calibri");
        font.setLetterSpacing(QFont::PercentageSpacing,100);
        doc.setDefaultFont(font);
        if(printername=="Print to PDF")
        {
            printer.setOutputFormat(QPrinter::PdfFormat);
            QString path="/home/pi/reader/PDF/"+d_name+" "+d_date+" "+d_time+".pdf";
            printer.setOutputFileName(path);
        }
    }
    if(graph==0)
    {
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);
    }
    else
    {
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        chartView->scene()->render(&painter);
    }

}

void Dialog::on_pushButton_clicked()
{
    print_process(65,0,1);

}
