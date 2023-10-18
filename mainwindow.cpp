#include "mainwindow.h"
#include "ui_mainwindow.h"

#define steps 6
#define dir 16
#define en 5
#define hm_sen 23
#define REF 500000

static int test_mode=0;
static int test_entry=0;
static QString btn_name="";
static int blank=0,cal=0,dup_cal=1,nc=0,pc=0,cc=0,lc=0,samp=0,dup_samp=1,total=0,total_cal=0,total_samp=0,nostd=0;
static int led_freq=10000;
static double offset[8], blnk[8], od[8];
static int pri_wave=0,sec_wave=0;
static double pri_res[12][8],sec_res[12][8],fin_res[12][8],pri[96],sec[96],abs_res[96],abs_avg[96],x_conc[10],y_abs[10],cutabs=0;
static QString dis[96],res[96], rem[96],pid[96];
static Pi2c arduino(7);
static QString unit, cuteqn;
static int invalid=0, save=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DEV_Module_Init();
    wiringPiSetup();
    ADS1263_SetMode(0);
    ADS1263_init_ADC1(ADS1263_100SPS);
    on_pushButton_14_clicked();
    pinMode (en, OUTPUT) ;
    pinMode (dir, OUTPUT) ;
    pinMode (steps, OUTPUT) ;
    pinMode (hm_sen, INPUT) ;
    digitalWrite(en,HIGH);

    mainLayout=new QGridLayout();
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    ui->scrollAreaWidgetContents->setLayout(mainLayout);
    QSqlDatabase sqdb = QSqlDatabase::addDatabase("QSQLITE");
    sqdb.setDatabaseName("/home/pi/reader/Database/er.db");
    if(!sqdb.open())
        qDebug() << "Can't Connect to DB !";
    else
        qDebug() << "Connected Successfully to DB !";
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
    test_menu();
    timer = new QTimer(this);
    dttimer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(delete_label()));
    connect(dttimer, SIGNAL(timeout()), this, SLOT(update_time()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->toolButton_12->setText("Save");
    ui->toolButton_14->setText("Save");
    ui->toolButton_16->setText("Save");
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
    test_entry=0;
}

void MainWindow::on_toolButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    dttimer->stop();
}

void MainWindow::on_toolButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->stackedWidget->raise();
    ui->pushButton_46->setDisabled(false);
    test_mode=1;
}

void MainWindow::on_toolButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget->raise();
    ui->pushButton->setDisabled(false);
    test_mode=2;
}

void MainWindow::on_toolButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->stackedWidget->raise();
    ui->pushButton_39->setDisabled(false);
    test_mode=3;
}

void MainWindow::on_toolButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
}

void MainWindow::on_toolButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
}

void MainWindow::on_toolButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
}

void MainWindow::on_pushButton_46_clicked()
{

    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Experiment Name",ui->pushButton_46->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_46->setText(data);
}

void MainWindow::on_pushButton_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Experiment Name",ui->pushButton->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton->setText(data);
}

void MainWindow::on_pushButton_3_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of Calibrators",ui->pushButton_3->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_3->setText(data);
}

void MainWindow::on_pushButton_4_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Low Range",ui->pushButton_4->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_4->setText(data);
}

void MainWindow::on_pushButton_5_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("High Range",ui->pushButton_5->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_5->setText(data);
}

void MainWindow::on_pushButton_6_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Unit",ui->pushButton_6->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_6->setText(data);
}

void MainWindow::on_c1_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-1 Conc.",ui->c1->text());
    key->exec();
    QString data = key->getData();
    ui->c1->setText(data);
}

void MainWindow::on_c2_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-2 Conc.",ui->c2->text());
    key->exec();
    QString data = key->getData();
    ui->c2->setText(data);
}

void MainWindow::on_c3_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-3 Conc.",ui->c3->text());
    key->exec();
    QString data = key->getData();
    ui->c3->setText(data);
}

void MainWindow::on_c4_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-4 Conc.",ui->c4->text());
    key->exec();
    QString data = key->getData();
    ui->c4->setText(data);
}

void MainWindow::on_c5_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-5 Conc.",ui->c5->text());
    key->exec();
    QString data = key->getData();
    ui->c5->setText(data);
}

void MainWindow::on_c6_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-6 Conc.",ui->c6->text());
    key->exec();
    QString data = key->getData();
    ui->c6->setText(data);
}

void MainWindow::on_c7_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-7 Conc.",ui->c7->text());
    key->exec();
    QString data = key->getData();
    ui->c7->setText(data);
}

void MainWindow::on_c8_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-8 Conc.",ui->c8->text());
    key->exec();
    QString data = key->getData();
    ui->c8->setText(data);
}

void MainWindow::on_c9_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-9 Conc.",ui->c9->text());
    key->exec();
    QString data = key->getData();
    ui->c9->setText(data);
}

void MainWindow::on_c10_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-10 Conc.",ui->c10->text());
    key->exec();
    QString data = key->getData();
    ui->c10->setText(data);
}

void MainWindow::on_a1_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-1 ABS",ui->a1->text());
    key->exec();
    QString data = key->getData();
    ui->a1->setText(data);
}

void MainWindow::on_a2_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-2 ABS",ui->a2->text());
    key->exec();
    QString data = key->getData();
    ui->a2->setText(data);
}

void MainWindow::on_a3_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-3 ABS",ui->a3->text());
    key->exec();
    QString data = key->getData();
    ui->a3->setText(data);
}

void MainWindow::on_a4_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-4 ABS",ui->a4->text());
    key->exec();
    QString data = key->getData();
    ui->a4->setText(data);
}

void MainWindow::on_a5_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-5 ABS",ui->a5->text());
    key->exec();
    QString data = key->getData();
    ui->a5->setText(data);
}

void MainWindow::on_a6_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-6 ABS",ui->a6->text());
    key->exec();
    QString data = key->getData();
    ui->a6->setText(data);
}

void MainWindow::on_a7_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-7 ABS",ui->a7->text());
    key->exec();
    QString data = key->getData();
    ui->a7->setText(data);
}

void MainWindow::on_a8_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-8 ABS",ui->a8->text());
    key->exec();
    QString data = key->getData();
    ui->a8->setText(data);
}

void MainWindow::on_a9_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-9 ABS",ui->a9->text());
    key->exec();
    QString data = key->getData();
    ui->a9->setText(data);
}

void MainWindow::on_a10_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-10 ABS",ui->a10->text());
    key->exec();
    QString data = key->getData();
    ui->a10->setText(data);
}

void MainWindow::on_pushButton_39_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Experiment Name",ui->pushButton_39->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_39->setText(data);
}

void MainWindow::on_pushButton_40_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of NC",ui->pushButton_40->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_40->setText(data);
}

void MainWindow::on_pushButton_41_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of PC",ui->pushButton_41->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_41->setText(data);
}

void MainWindow::on_pushButton_42_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of LC",ui->pushButton_42->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_42->setText(data);
}

void MainWindow::on_pushButton_67_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of CC",ui->pushButton_67->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_67->setText(data);
}

void MainWindow::on_pushButton_64_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of NC",ui->pushButton_64->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_64->setText(data);
}

void MainWindow::on_pushButton_65_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of PC",ui->pushButton_65->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_65->setText(data);
}

void MainWindow::on_pushButton_66_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of LC",ui->pushButton_66->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_66->setText(data);
}

void MainWindow::on_pushButton_68_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of CC",ui->pushButton_68->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_68->setText(data);
}

void MainWindow::on_pushButton_43_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Cutoff Equation",ui->pushButton_43->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_43->setText(data);
}

void MainWindow::on_pushButton_44_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Positive Index",ui->pushButton_44->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_44->setText(data);
}

void MainWindow::on_pushButton_45_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Negative Index",ui->pushButton_45->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_45->setText(data);
}

void MainWindow::on_pushButton_47_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("% Greyzone",ui->pushButton_47->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_47->setText(data);
}

void MainWindow::on_toolButton_16_clicked()
{
    if(abs_check()==false)
    {
        return;
    }

    QString name=ui->pushButton_46->text();
    int pri=ui->comboBox_8->currentIndex();
    int sec=ui->comboBox_9->currentIndex();
    QSqlQuery insertQuery;
    if (test_entry==0)
        insertQuery.prepare("insert into tests(name,mode,pri,sec) values(:name,:mode,:pri,:sec)");
    else if(test_entry==1)
        insertQuery.prepare("update tests set name=:name,mode=:mode,pri=:pri,sec=:sec where name=:name");
    insertQuery.bindValue(":name",name);
    insertQuery.bindValue(":mode",test_mode);
    insertQuery.bindValue(":pri",pri);
    insertQuery.bindValue(":sec",sec);
    insertQuery.exec();
    //    if (!insertQuery.exec())
    //    {
    //        qDebug() << "Failed to insert data into the database!";
    //        qDebug() << "Error details:" << insertQuery.lastError().text();
    //    }


    //update test menu
    while(mainLayout->count() > 0)
    {
        QWidget* widget = mainLayout->itemAt(0)->widget();
        mainLayout->removeWidget(widget);
        delete widget;
    }
    test_menu();

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();

}


void MainWindow::on_toolButton_12_clicked()
{
    if(multi_check()==false)
    {
        return;
    }
    QString name=ui->pushButton->text();
    int pri=ui->comboBox->currentIndex();
    int sec=ui->comboBox_2->currentIndex();
    int nocal=ui->pushButton_3->text().toInt();
    int graph=ui->comboBox_3->currentIndex();
    int axis=ui->comboBox_4->currentIndex();
    double nor_low=ui->pushButton_4->text().toDouble();
    double nor_high=ui->pushButton_5->text().toDouble();
    unit=ui->pushButton_6->text();

    QPushButton* concb[] = { ui->c1,ui->c2,ui->c3,ui->c4,ui->c5,ui->c6,ui->c7,ui->c8,ui->c9,ui->c10};
    QPushButton* absb[] = { ui->a1,ui->a2,ui->a3,ui->a4,ui->a5,ui->a6,ui->a7,ui->a8,ui->a9,ui->a10};
    QString qry;
    if(test_entry==0)
        qry="insert into tests(name,mode,pri,sec,std,graph,axis,norl,norh,unit,conc1,conc2,conc3,conc4,conc5,conc6,conc7,conc8,conc9,conc10,abs1,abs2,abs3,abs4,abs5,abs6,abs7,abs8,abs9,abs10) values(:name,:mode,:pri,:sec,:std,:graph,:axis,:norl,:norh,:unit,:c1,:c2,:c3,:c4,:c5,:c6,:c7,:c8,:c9,:c10,:a1,:a2,:a3,:a4,:a5,:a6,:a7,:a8,:a9,:a10)";
    else if(test_entry==1)
        qry="update tests set name=:name,mode=:mode,pri=:pri,sec=:sec,std=:std,graph=:graph,axis=:graph,norl=:norl,norh=:norh,unit=:unit,conc1=:c1,conc2=:c2,conc3=:c3,conc4=:c4,conc5=:c5,conc6=:c6,conc7=:c7,conc8=:c8,conc9=:c9,conc10=:c10,abs1=:a1,abs2=:a2,abs3=:a3,abs4=:a4,abs5=:a5,abs6=:a6,abs7=:a7,abs8=:a8,abs9=:a9,abs10=:a10 where name=:name";
    double cal[10], abs[10];
    for(int i=0;i<10;i++)
    {
        cal[i]=concb[i]->text().toDouble();
        abs[i]=absb[i]->text().toDouble();
    }
    QSqlQuery insertQuery;
    insertQuery.prepare(qry);
    insertQuery.bindValue(":name",name);
    insertQuery.bindValue(":mode",test_mode);
    insertQuery.bindValue(":pri",pri);
    insertQuery.bindValue(":sec",sec);
    insertQuery.bindValue(":std",nocal);
    insertQuery.bindValue(":graph",graph);
    insertQuery.bindValue(":axis",axis);
    insertQuery.bindValue(":norl",nor_low);
    insertQuery.bindValue(":norh",nor_high);
    insertQuery.bindValue(":unit",unit);
    insertQuery.bindValue(":c1",cal[0]);
    insertQuery.bindValue(":c2",cal[1]);
    insertQuery.bindValue(":c3",cal[2]);
    insertQuery.bindValue(":c4",cal[3]);
    insertQuery.bindValue(":c5",cal[4]);
    insertQuery.bindValue(":c6",cal[5]);
    insertQuery.bindValue(":c7",cal[6]);
    insertQuery.bindValue(":c8",cal[7]);
    insertQuery.bindValue(":c9",cal[8]);
    insertQuery.bindValue(":c10",cal[9]);
    insertQuery.bindValue(":a1",abs[0]);
    insertQuery.bindValue(":a2",abs[1]);
    insertQuery.bindValue(":a3",abs[2]);
    insertQuery.bindValue(":a4",abs[3]);
    insertQuery.bindValue(":a5",abs[4]);
    insertQuery.bindValue(":a6",abs[5]);
    insertQuery.bindValue(":a7",abs[6]);
    insertQuery.bindValue(":a8",abs[7]);
    insertQuery.bindValue(":a9",abs[8]);
    insertQuery.bindValue(":a10",abs[9]);
    insertQuery.exec();


    //update test menu
    while(mainLayout->count() > 0)
    {
        QWidget* widget = mainLayout->itemAt(0)->widget();
        mainLayout->removeWidget(widget);
        delete widget;
    }
    test_menu();

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
}


void MainWindow::on_toolButton_8_clicked()
{
    blank=cal=nc=pc=cc=lc=samp=total=total_cal=total_samp=0;
    dup_cal=dup_samp=1;
    double abs1=0,abs2=0,cutabs=0;
    ui->stackedWidget->setCurrentIndex(5);
    ui->stackedWidget->raise();
    ui->label_4->setText(btn_name);
    ui->pushButton_20->setText(btn_name);
    ui->pushButton_21->setText(btn_name);
    ui->label_15->setDisabled(false);
    ui->label_16->setDisabled(false);
    ui->comboBox_11->setDisabled(false);
    ui->comboBox_12->setDisabled(false);
    ui->comboBox_11->setCurrentIndex(0);
    ui->comboBox_10->setCurrentIndex(0);
    ui->pushButton_48->setText("");
    ui->pushButton_49->setText("");
    ui->textBrowser_4->setText("");
    ui->textBrowser_5->setText("");
    ui->textBrowser_6->setText("");
    ui->toolButton_22->setDisabled(false);
    ui->toolButton_27->setDisabled(false);

    QSqlQuery Query;
    Query.prepare("select mode,std,abs1,abs2,nc,pc,lc,cc,pri,sec,cutabs,unit FROM tests WHERE name = :bname");
    Query.bindValue(":bname", btn_name);
    Query.exec();
    while(Query.next())
    {
        test_mode=Query.value("mode").toInt();
        nostd=cal=Query.value("std").toInt();
        abs1=Query.value("abs1").toDouble();
        abs2=Query.value("abs2").toDouble();
        nc=Query.value("nc").toInt();
        pc=Query.value("pc").toInt();
        lc=Query.value("lc").toInt();
        cc=Query.value("cc").toInt();
        pri_wave=Query.value("pri").toInt();
        sec_wave=Query.value("sec").toInt();
        cutabs=Query.value("cutabs").toDouble();
        unit=Query.value("unit").toString();
    }
    led_control(pri_wave);

    if(test_mode==1)
    {
        ui->label_15->setDisabled(true);
        ui->label_16->setDisabled(true);
        ui->comboBox_11->setDisabled(true);
        ui->comboBox_12->setDisabled(true);
        ui->toolButton_32->setDisabled(true);
        ui->toolButton_33->setDisabled(true);

    }
    if(test_mode==2)
    {
        if(abs1==0.0 && abs2==0.0)
        {
            ui->label_15->setDisabled(true);
            ui->comboBox_11->setDisabled(true);
        }
        ui->label_15->setText("Cal.");
        ui->toolButton_32->setDisabled(false);
        ui->toolButton_33->setDisabled(false);
        ui->toolButton_23->setDisabled(false);
        ui->toolButton_29->setDisabled(false);
    }
    if(test_mode==3)
    {
        if(!(cutabs>0.0))
        {
            ui->label_15->setDisabled(true);
            ui->comboBox_11->setDisabled(true);
        }
        ui->label_16->setDisabled(true);
        ui->comboBox_12->setDisabled(true);
        ui->label_15->setText("Cont.");
        ui->toolButton_32->setDisabled(false);
        ui->toolButton_33->setDisabled(false);
        ui->toolButton_23->setDisabled(false);
        ui->toolButton_29->setDisabled(false);
        unit="S/Co";
    }

    update_sample_page();
}

void MainWindow::on_toolButton_14_clicked()
{
    if(cut_check()==false)
    {
        return;
    }
    QString name=ui->pushButton_39->text();
    int pri=ui->comboBox_5->currentIndex();
    int sec=ui->comboBox_6->currentIndex();
    int nc=ui->pushButton_40->text().toInt();
    int pc=ui->pushButton_41->text().toInt();
    int lc=ui->pushButton_42->text().toInt();
    int cc=ui->pushButton_67->text().toInt();
    QString ncq=ui->pushButton_64->text();
    QString pcq=ui->pushButton_65->text();
    QString lcq=ui->pushButton_66->text();
    QString ccq=ui->pushButton_68->text();
    QString cut=ui->pushButton_43->text();
    int thresh=ui->comboBox_7->currentIndex();
    double pos=ui->pushButton_44->text().toDouble();
    double neg=ui->pushButton_45->text().toDouble();
    double grey=ui->pushButton_47->text().toDouble();
    double cutabs=ui->pushButton_50->text().toDouble();
    QString qry;
    if(test_entry==0)
        qry="insert into tests(name,mode,pri,sec,threshold,nc,pc,lc,cc,ncqc,pcqc,lcqc,ccqc,cutoff,pos,neg,grey,cutabs) values(:name,:mode,:pri,:sec,:thresh,:nc,:pc,:lc,:cc,:ncq,:pcq,:lcq,:ccq,:cut,:pos,:neg,:grey,:cuta)";
    else if(test_entry==1)
        qry="update tests set name=:name,mode=:mode,pri=:pri,sec=:sec,threshold=:thresh,nc=:nc,pc=:pc,lc=:lc,cc=:cc,ncqc=:ncq,pcqc=:pcq,lcqc=:lcq,ccqc=:ccq,cutoff=:cut,pos=:pos,neg=:neg,grey=:grey,cutabs=:cuta where name=:name";
    QSqlQuery insertQuery;
    insertQuery.prepare(qry);
    insertQuery.bindValue(":name",name);
    insertQuery.bindValue(":mode",test_mode);
    insertQuery.bindValue(":pri",pri);
    insertQuery.bindValue(":sec",sec);
    insertQuery.bindValue(":thresh",thresh);
    insertQuery.bindValue(":nc",nc);
    insertQuery.bindValue(":pc",pc);
    insertQuery.bindValue(":lc",lc);
    insertQuery.bindValue(":cc",cc);
    insertQuery.bindValue(":ncq",ncq);
    insertQuery.bindValue(":pcq",pcq);
    insertQuery.bindValue(":lcq",lcq);
    insertQuery.bindValue(":ccq",ccq);
    insertQuery.bindValue(":cut",cut);
    insertQuery.bindValue(":pos",pos);
    insertQuery.bindValue(":neg",neg);
    insertQuery.bindValue(":grey",grey);
    insertQuery.bindValue(":cuta",cutabs);
    insertQuery.exec();

    //update test menu
    while(mainLayout->count() > 0)
    {
        QWidget* widget = mainLayout->itemAt(0)->widget();
        mainLayout->removeWidget(widget);
        delete widget;
    }
    test_menu();

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
}

int MainWindow::abs_check()
{
    if(ui->pushButton_46->text().length()==0)
    {
        ui->textBrowser->setText("Empty Test Name");
        return false;
    }
    if(ui->comboBox_8->currentIndex()==0)
    {
        ui->textBrowser->setText("Select primary wavelength");
        return false;
    }
    if(test_entry==0)
    {
        QSqlQuery query;
        query.prepare("SELECT name FROM tests");
        query.exec();
        while(query.next())
        {
            if(query.value(0)==ui->pushButton_46->text())
            {
                ui->textBrowser->setText("Test name already exists");
                return false;
            }
        }
    }

    return true;
}

int MainWindow::multi_check()
{
    if(ui->pushButton->text().length()==0)
    {
        ui->textBrowser_2->setText("Empty Test Name");
        return false;
    }
    if(ui->comboBox->currentIndex()==0)
    {
        ui->textBrowser_2->setText("Select primary wavelength");
        return false;
    }
    for(int i=1;i<=10;i++)
    {
        if(ui->pushButton_3->text().toInt()==i)
        {
            break;
        }
        if(i==10)
        {
            ui->textBrowser_2->setText("Select proper number of calibrator");
            return false;
        }
    }
    QPushButton* concb[] = { ui->c1,ui->c2,ui->c3,ui->c4,ui->c5,ui->c6,ui->c7,ui->c8,ui->c9,ui->c10};
    //QPushButton* absb[] = { ui->a1,ui->a2,ui->a3,ui->a4,ui->a5,ui->a6,ui->a7,ui->a8,ui->a9,ui->a10};
    int nocal=ui->pushButton_3->text().toInt();
    for(int i=0;i<nocal;i++)
    {
        if(concb[i]->text().length()==0)
        {
            ui->textBrowser_2->setText("Input proper calibrator value");
            return false;
        }
    }
    int inc=0,dec=0;
    for(int i=0;i<nocal-1;i++)
    {
        if(!(concb[i]->text().toDouble()<concb[i+1]->text().toDouble()))
            inc=1;
        if(!(concb[i]->text().toDouble()>concb[i+1]->text().toDouble()))
            dec=1;
    }
    if(inc==1 && dec==1)
    {
        ui->textBrowser_2->setText("Calibrator Values not in Order");
        return false;
    }
    if(test_entry==0)
    {
        QSqlQuery query;
        query.prepare("SELECT name FROM tests");
        query.exec();
        while(query.next())
        {
            if(query.value(0)==ui->pushButton->text())
            {
                ui->textBrowser_2->setText("Test name already exists");
                return false;
            }
        }
    }

    return true;
}

int MainWindow::cut_check()
{
    if(ui->pushButton_39->text().length()==0)
    {
        ui->textBrowser_3->setText("Empty Test Name");
        return false;
    }
    if(ui->comboBox_5->currentIndex()==0)
    {
        ui->textBrowser_3->setText("Select primary wavelength");
        return false;
    }
    int nc=ui->pushButton_40->text().toInt();
    int pc=ui->pushButton_41->text().toInt();
    int lc=ui->pushButton_42->text().toInt();
    int cc=ui->pushButton_67->text().toInt();
    qDebug()<<nc;
    if(!(nc>=1 || pc>=1 || lc>=1 || cc>=1))
    {
        ui->textBrowser_3->setText("Should contain atleast one control value");
        return false;
    }
    if(ui->pushButton_43->text().length()==0)
    {
        ui->textBrowser_3->setText("Invalid cutoff equation");
        return false;
    }
    if(ui->pushButton_44->text().length()==0)
    {
        ui->textBrowser_3->setText("Invalid Positive Index");
        return false;
    }
    if(ui->pushButton_45->text().length()==0)
    {
        ui->textBrowser_3->setText("Invalid Negative Index");
        return false;
    }
    if(test_entry==0)
    {
        QSqlQuery query;
        query.prepare("SELECT name FROM tests");
        query.exec();
        while(query.next())
        {
            if(query.value(0)==ui->pushButton_39->text())
            {
                ui->textBrowser_3->setText("Test name already exists");
                return false;
            }
        }
    }

    return true;
}

void MainWindow::test_menu()
{
    QSqlQuery query;
    query.prepare("SELECT name FROM tests");
    query.exec();
    int row=0,col=0;
    while(query.next())
    {
        QPushButton *button = new QPushButton();
        button->setText(query.value(0).toString());
        button->setFixedSize(135,50);
        button->setStyleSheet("QPushButton:hover:!pressed{background-color: rgb(224, 255, 0);}");
        button->setObjectName(query.value(0).toString());
        disconnect(button, &QPushButton::clicked, this, &MainWindow::sig_button);
        connect(button, &QPushButton::clicked, this, &MainWindow::sig_button);
        mainLayout->addWidget(button, row, col);
        mainLayout->setSpacing(10);
        mainLayout->setAlignment(Qt::AlignTop);
        col++;
        if(col%4==0)
        {
            row++;
            col=0;
        }
    }
}

void MainWindow::sig_button()
{
    QObject *senderObj = sender();
    QString senderObjName = senderObj->objectName();
    //qDebug()<< "Button: " << senderObjName;
    btn_name = senderObjName;
}


void MainWindow::on_toolButton_7_clicked()
{
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM tests WHERE name = :bname");
    deleteQuery.bindValue(":bname", btn_name);
    deleteQuery.exec();
    while(mainLayout->count() > 0)
    {
        QWidget* widget = mainLayout->itemAt(0)->widget();
        mainLayout->removeWidget(widget);
        delete widget;
    }
    test_menu();
}

void MainWindow::on_toolButton_6_clicked()
{
    test_entry=1;
    ui->toolButton_12->setText("Update");
    ui->toolButton_14->setText("Update");
    ui->toolButton_16->setText("Update");
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
    QSqlQuery Query;
    Query.prepare("select mode FROM tests WHERE name = :bname");
    Query.bindValue(":bname", btn_name);
    Query.exec();
    while(Query.next())
    {
        test_mode=Query.value("mode").toInt();
    }
    if(test_mode==1)
    {
        on_toolButton_9_clicked();
        ui->pushButton_46->setDisabled(true);
        int pri=0, sec=0;
        Query.prepare("select pri, sec FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        while(Query.next())
        {
            pri=Query.value("pri").toInt();
            sec=Query.value("sec").toInt();
            qDebug()<<Query.value("pri");
        }
        ui->pushButton_46->setText(btn_name);
        ui->comboBox_8->setCurrentIndex(pri);
        ui->comboBox_9->setCurrentIndex(sec);
        qDebug()<<pri<<sec;
    }

    if(test_mode==2)
    {
        on_toolButton_10_clicked();
        ui->pushButton->setDisabled(true);
        int pri=0,sec=0,std=0,graph=0,axis=0;
        double norl=0,norh=0,c[10],a[10];
        QString constr[10]={"conc1","conc2","conc3","conc4","conc5","conc6","conc7","conc8","conc9","conc10"};
        QString absstr[10]={"abs1","abs2","abs3","abs4","abs5","abs6","abs7","abs8","abs9","abs10"};
        QPushButton* concb[] = { ui->c1,ui->c2,ui->c3,ui->c4,ui->c5,ui->c6,ui->c7,ui->c8,ui->c9,ui->c10};
        QPushButton* absb[] = { ui->a1,ui->a2,ui->a3,ui->a4,ui->a5,ui->a6,ui->a7,ui->a8,ui->a9,ui->a10};

        Query.prepare("select * FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        while(Query.next())
        {
            pri=Query.value("pri").toInt();
            sec=Query.value("sec").toInt();
            std=Query.value("std").toInt();
            graph=Query.value("graph").toInt();
            axis=Query.value("axis").toInt();
            unit=Query.value("unit").toString();
            norl=Query.value("norl").toDouble();
            norh=Query.value("norh").toDouble();
            for(int i=0;i<std;i++)
            {
                c[i]=Query.value(constr[i]).toDouble();
                a[i]=Query.value(absstr[i]).toDouble();
            }

        }
        ui->pushButton->setText(btn_name);
        ui->comboBox->setCurrentIndex(pri);
        ui->comboBox_2->setCurrentIndex(sec);
        ui->pushButton_3->setText(QString::number(std));
        ui->comboBox_3->setCurrentIndex(graph);
        ui->comboBox_4->setCurrentIndex(axis);
        ui->pushButton_4->setText(QString::number(norl));
        ui->pushButton_5->setText(QString::number(norh));
        ui->pushButton_6->setText(unit);
        for(int i=0;i<std;i++)
        {
            concb[i]->setText(QString::number(c[i]));
            absb[i]->setText(QString::number(a[i]));
        }

    }
    if(test_mode==3)
    {
        on_toolButton_11_clicked();
        ui->pushButton_39->setDisabled(true);
        int pri=0,sec=0,nc=0,pc=0,lc=0,cc=0,thresh=0;
        double pos=0,neg=0,grey=0,cuta=0;;
        QString cut,ncq,pcq,lcq,ccq;

        Query.prepare("select * FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        while(Query.next())
        {
            pri=Query.value("pri").toInt();
            sec=Query.value("sec").toInt();
            nc=Query.value("nc").toInt();
            pc=Query.value("pc").toInt();
            lc=Query.value("lc").toInt();
            cc=Query.value("cc").toInt();
            thresh=Query.value("threshold").toInt();
            ncq=Query.value("ncqc").toString();
            pcq=Query.value("pcqc").toString();
            lcq=Query.value("lcqc").toString();
            ccq=Query.value("ccqc").toString();
            pos=Query.value("pos").toDouble();
            neg=Query.value("neg").toDouble();
            grey=Query.value("grey").toDouble();
            cut=Query.value("cutoff").toString();
            cuta=Query.value("cutabs").toDouble();
        }

        ui->pushButton_39->setText(btn_name);
        ui->comboBox_5->setCurrentIndex(pri);
        ui->comboBox_6->setCurrentIndex(sec);
        ui->pushButton_40->setText(QString::number(nc));
        ui->pushButton_41->setText(QString::number(pc));
        ui->pushButton_42->setText(QString::number(lc));
        ui->pushButton_67->setText(QString::number(cc));
        ui->pushButton_64->setText(ncq);
        ui->pushButton_65->setText(pcq);
        ui->pushButton_66->setText(lcq);
        ui->pushButton_68->setText(ccq);
        ui->pushButton_43->setText(cut);
        ui->comboBox_7->setCurrentIndex(thresh);
        ui->pushButton_44->setText(QString::number(pos));
        ui->pushButton_45->setText(QString::number(neg));
        ui->pushButton_47->setText(QString::number(grey));
        ui->pushButton_50->setText(QString::number(cuta));
    }

}

void MainWindow::on_toolButton_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
    led_control(0);
}

void MainWindow::on_pushButton_48_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Number of Blank",ui->pushButton_48->text());
    key->exec();
    QString data = key->getData();
    blank=data.toInt();
    ui->pushButton_48->setText(data);
    update_sample_page();
}


void MainWindow::on_pushButton_49_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Number of Blank",ui->pushButton_49->text());
    key->exec();
    QString data = key->getData();
    samp=data.toInt();
    ui->pushButton_49->setText(data);
    update_sample_page();
}

void MainWindow::on_comboBox_10_currentIndexChanged(int index)
{
    dup_samp=index+1;
    update_sample_page();
}

void MainWindow::on_comboBox_12_currentIndexChanged(int index)
{
    dup_cal=index+1;
    update_sample_page();
}

void MainWindow::update_sample_page()
{
    total_cal=cal*dup_cal;
    total_samp=samp*dup_samp;
    total=blank+nc+pc+lc+cc+total_cal+total_samp;
    if(total==0)
    {
        ui->toolButton_18->setDisabled(true);
    }
    else
    {
        ui->toolButton_18->setDisabled(false);
    }
    if (total>96)
    {
        total=96;
        ui->textBrowser_4->setText("Total count cannot be more than 96");
    }
    QPushButton* samp_buttons[96] = { ui->A1,ui->B1,ui->C1,ui->D1,ui->E1,ui->F1,ui->G1,ui->H1,
                                      ui->A2,ui->B2,ui->C2,ui->D2,ui->E2,ui->F2,ui->G2,ui->H2,
                                      ui->A3,ui->B3,ui->C3,ui->D3,ui->E3,ui->F3,ui->G3,ui->H3,
                                      ui->A4,ui->B4,ui->C4,ui->D4,ui->E4,ui->F4,ui->G4,ui->H4,
                                      ui->A5,ui->B5,ui->C5,ui->D5,ui->E5,ui->F5,ui->G5,ui->H5,
                                      ui->A6,ui->B6,ui->C6,ui->D6,ui->E6,ui->F6,ui->G6,ui->H6,
                                      ui->A7,ui->B7,ui->C7,ui->D7,ui->E7,ui->F7,ui->G7,ui->H7,
                                      ui->A8,ui->B8,ui->C8,ui->D8,ui->E8,ui->F8,ui->G8,ui->H8,
                                      ui->A9,ui->B9,ui->C9,ui->D9,ui->E9,ui->F9,ui->G9,ui->H9,
                                      ui->A10,ui->B10,ui->C10,ui->D10,ui->E10,ui->F10,ui->G10,ui->H10,
                                      ui->A11,ui->B11,ui->C11,ui->D11,ui->E11,ui->F11,ui->G11,ui->H11,
                                      ui->A12,ui->B12,ui->C12,ui->D12,ui->E12,ui->F12,ui->G12,ui->H12,};
    //QString dis[96];
    for(int i=0;i<96;i++)
    {
        samp_buttons[i]->setText("");
        samp_buttons[i]->setStyleSheet("background-color: rgb(255, 255, 255)");
    }
    for(int i=0,j=1;i<blank;i++,j++)
    {
        dis[i]='B'+QString::number(j);

    }
    for(int i=blank,j=1;i<blank+nc;i++,j++)
    {
        dis[i]="NC"+QString::number(j);

    }
    for(int i=blank+nc,j=1;i<blank+nc+pc;i++,j++)
    {
        dis[i]="PC"+QString::number(j);

    }
    for(int i=blank+nc+pc,j=1;i<blank+nc+pc+lc;i++,j++)
    {
        dis[i]="LC"+QString::number(j);

    }
    for(int i=blank+nc+pc+lc,j=1;i<blank+nc+pc+lc+cc;i++,j++)
    {
        dis[i]="CC"+QString::number(j);

    }
    for(int i=blank+nc+pc+lc+cc,j=1;i<blank+nc+pc+lc+cc+total_cal;i++,j++)
    {
        if(dup_cal==1)
        {
            dis[i]='C'+QString::number(j);
        }
        else
        {
            dis[i]=dis[i+1]='C'+QString::number(j);
            i++;
        }
    }
    for(int i=blank+nc+pc+lc+cc+total_cal,j=1;i<total;i++,j++)
    {
        if(dup_samp==1)
        {
            dis[i]='S'+QString::number(j);
        }
        else
        {
            dis[i]=dis[i+1]='S'+QString::number(j);
            i++;
        }
    }
    for(int i=0;i<total;i++)
    {
        qDebug()<<dis[i]<<total;
    }
    for(int i=0;i<total;i++)
    {
        samp_buttons[i]->setText(dis[i]);
        if(i<blank)
            samp_buttons[i]->setStyleSheet("background-color: rgb(235, 235, 235)");
        else if(i<blank+nc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(40, 230, 100)");
        else if(i<blank+nc+pc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(245, 30, 20)");
        else if(i<blank+nc+pc+lc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(170, 20, 240)");
        else if(i<blank+nc+pc+lc+cc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(240, 20, 120)");
        else if(i<blank+nc+pc+lc+cc+total_cal)
            samp_buttons[i]->setStyleSheet("background-color: rgb(245, 235, 20)");
        else if(i<total)
            samp_buttons[i]->setStyleSheet("background-color: rgb(20, 160, 10)");
    }
}





void MainWindow::on_pushButton_50_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Cutoff Equation",ui->pushButton_50->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_50->setText(data);
}

void MainWindow::on_comboBox_11_currentIndexChanged(int index)
{
    qDebug()<<index;
    if(index==1)
    {
        nc=pc=lc=cc=cal=0;
        ui->toolButton_32->setDisabled(true);
        ui->toolButton_33->setDisabled(true);
    }
    else
    {
        QSqlQuery Query;
        Query.prepare("select std,nc,pc,lc,cc FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        while(Query.next())
        {
            cal=Query.value("std").toInt();
            nc=Query.value("nc").toInt();
            pc=Query.value("pc").toInt();
            lc=Query.value("lc").toInt();
            cc=Query.value("cc").toInt();
        }
        ui->toolButton_32->setDisabled(false);
        ui->toolButton_33->setDisabled(false);
    }
    update_sample_page();
}

void MainWindow::on_pushButton_2_clicked()
{
    int pwm[10]={500,500,500,500,500,500,500,500,1,led_freq};
    pwm[9]=ui->pushButton_10->text().toInt();
    QString My_String;
    for(int i=0; i<10; i++)
    {
        My_String.append(QString::number(pwm[i]));
        if(i==9)
            My_String.append('\0');
        else
            My_String.append(" ");
    }
    qDebug()<<My_String;
    int len=My_String.length();
    char* ch;
    QByteArray ba=My_String.toLatin1();
    ch=ba.data();
    arduino.i2cWrite(ch,len);
}

void MainWindow::on_pushButton_7_clicked()
{
    int pwm[10]={500,500,500,500,500,500,500,500,2,led_freq};
    pwm[9]=ui->pushButton_10->text().toInt();
    QString My_String;
    for(int i=0; i<10; i++)
    {
        My_String.append(QString::number(pwm[i]));
        if(i==9)
            My_String.append('\0');
        else
            My_String.append(" ");
    }
    qDebug()<<My_String;
    int len=My_String.length();
    char* ch;
    QByteArray ba=My_String.toLatin1();
    ch=ba.data();
    arduino.i2cWrite(ch,len);
}

void MainWindow::on_pushButton_8_clicked()
{
    int pwm[10]={500,500,500,500,500,500,500,500,3,led_freq};
    pwm[9]=ui->pushButton_10->text().toInt();
    QString My_String;
    for(int i=0; i<10; i++)
    {
        My_String.append(QString::number(pwm[i]));
        if(i==9)
            My_String.append('\0');
        else
            My_String.append(" ");
    }
    qDebug()<<My_String;
    int len=My_String.length();
    char* ch;
    QByteArray ba=My_String.toLatin1();
    ch=ba.data();
    arduino.i2cWrite(ch,len);
}

void MainWindow::on_pushButton_9_clicked()
{
    int pwm[10]={500,500,500,500,500,500,500,500,4,led_freq};
    pwm[9]=ui->pushButton_10->text().toInt();
    QString My_String;
    for(int i=0; i<10; i++)
    {
        My_String.append(QString::number(pwm[i]));
        if(i==9)
            My_String.append('\0');
        else
            My_String.append(" ");
    }
    qDebug()<<My_String;
    int len=My_String.length();
    char* ch;
    QByteArray ba=My_String.toLatin1();
    ch=ba.data();
    arduino.i2cWrite(ch,len);
}

void MainWindow::on_pushButton_11_clicked()
{
    int pwm[10]={500,500,500,500,500,500,500,500,0,led_freq};
    pwm[9]=ui->pushButton_10->text().toInt();
    QString My_String;
    for(int i=0; i<10; i++)
    {
        My_String.append(QString::number(pwm[i]));
        if(i==9)
            My_String.append('\0');
        else
            My_String.append(" ");
    }
    qDebug()<<My_String;
    int len=My_String.length();
    char* ch;
    QByteArray ba=My_String.toLatin1();
    ch=ba.data();
    arduino.i2cWrite(ch,len);
}

void MainWindow::on_toolButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    dttimer->stop();
}

void MainWindow::on_pushButton_14_clicked()
{
    ADS1263_init_ADC1(ADS1263_100SPS);
    int samples = 4;//should be in even number
    uint32_t ADC[8][4];
    for(int i=0;i<8;i++)
    {
        offset[i]=0;
    }
    for(uint8_t j=0;j<8;j++)
    {
        for(int i=0;i<samples;i++)
        {
            if(i==0)
            {
                for(uint buf=0;buf<4;buf++)
                    buf=ADS1263_GetChannalValue(j);
            }
            ADC[j][i]=ADS1263_GetChannalValue(j);
            if((ADC[j][i]>>31) == 1)
                ADC[j][i]=(REF*2 - ADC[j][i]/2147483648.0 * REF);
            else
                ADC[j][i]=(ADC[j][i]/2147483648.0 * REF);
        }
    }
    for (int i=0;i<samples;i++)
    {
        offset[7]+=ADC[0][i];
        offset[6]+=ADC[1][i];
        offset[5]+=ADC[2][i];
        offset[4]+=ADC[3][i];
        offset[3]+=ADC[4][i];
        offset[2]+=ADC[5][i];
        offset[1]+=ADC[6][i];
        offset[0]+=ADC[7][i];
        if(i==samples-1)
        {
            offset[0]=offset[0]/(samples);
            offset[1]=offset[1]/(samples);
            offset[2]=offset[2]/(samples);
            offset[3]=offset[3]/(samples);
            offset[4]=offset[4]/(samples);
            offset[5]=offset[5]/(samples);
            offset[6]=offset[6]/(samples);
            offset[7]=offset[7]/(samples);
        }
    }
    qDebug()<<offset[7]<<offset[6]<<offset[5]<<offset[4]<<offset[3]<<offset[2]<<offset[1]<<offset[0];
}

void MainWindow::on_pushButton_13_clicked()
{
    ADS1263_init_ADC1(ADS1263_100SPS);
    int samples = 4;
    uint32_t ADC[8][4];
    for(int i=0;i<8;i++)
    {
        blnk[i]=0;
    }
    for(uint8_t j=0;j<8;j++)
    {
        for(int i=0;i<samples;i++)
        {
            if(i==0)
            {
                for(uint buf=0;buf<4;buf++)
                    buf=ADS1263_GetChannalValue(j);
            }
            ADC[j][i]=ADS1263_GetChannalValue(j);
            if((ADC[j][i]>>31) == 1)
                ADC[j][i]=(REF*2 - ADC[j][i]/2147483648.0 * REF);
            else
                ADC[j][i]=(ADC[j][i]/2147483648.0 * REF);
        }
    }
    for (int i=0;i<samples;i++)
    {
        blnk[7]+=ADC[0][i];
        blnk[6]+=ADC[1][i];
        blnk[5]+=ADC[2][i];
        blnk[4]+=ADC[3][i];
        blnk[3]+=ADC[4][i];
        blnk[2]+=ADC[5][i];
        blnk[1]+=ADC[6][i];
        blnk[0]+=ADC[7][i];
        if(i==samples-1)
        {
            blnk[0]=(blnk[0]/samples)-offset[0];
            blnk[1]=(blnk[1]/samples)-offset[1];
            blnk[2]=(blnk[2]/samples)-offset[2];
            blnk[3]=(blnk[3]/samples)-offset[3];
            blnk[4]=(blnk[4]/samples)-offset[4];
            blnk[5]=(blnk[5]/samples)-offset[5];
            blnk[6]=(blnk[6]/samples)-offset[6];
            blnk[7]=(blnk[7]/samples)-offset[7];
        }
    }
    qDebug()<<blnk[7]<<blnk[6]<<blnk[5]<<blnk[4]<<blnk[3]<<blnk[2]<<blnk[1]<<blnk[0];
}

void MainWindow::on_pushButton_12_clicked()
{

    ADS1263_init_ADC1(ADS1263_100SPS);
    int samples = 4;
    uint32_t ADC[8][4];
    for(int i=0;i<8;i++)
    {
        od[i]=0;
    }
    for(uint8_t j=0;j<8;j++)
    {
        for(int i=0;i<samples;i++)
        {
            if(i==0)
            {
                for(uint buf=0;buf<4;buf++)
                    buf=ADS1263_GetChannalValue(j);
            }
            ADC[j][i]=ADS1263_GetChannalValue(j);
            if((ADC[j][i]>>31) == 1)
                ADC[j][i]=(REF*2 - ADC[j][i]/2147483648.0 * REF);
            else
                ADC[j][i]=(ADC[j][i]/2147483648.0 * REF);
        }
    }
    for (int i=0;i<samples;i++)
    {
        od[7]+=ADC[0][i];
        od[6]+=ADC[1][i];
        od[5]+=ADC[2][i];
        od[4]+=ADC[3][i];
        od[3]+=ADC[4][i];
        od[2]+=ADC[5][i];
        od[1]+=ADC[6][i];
        od[0]+=ADC[7][i];
        if(i==samples-1)
        {
            od[0]=(od[0]/samples)-offset[0];
            od[1]=(od[1]/samples)-offset[1];
            od[2]=(od[2]/samples)-offset[2];
            od[3]=(od[3]/samples)-offset[3];
            od[4]=(od[4]/samples)-offset[4];
            od[5]=(od[5]/samples)-offset[5];
            od[6]=(od[6]/samples)-offset[6];
            od[7]=(od[7]/samples)-offset[7];
        }
    }
    for (int i=0;i<8;i++)
        if(od[i]<=0)
            od[i]=1;
    qDebug()<<od[7]<<od[6]<<od[5]<<od[4]<<od[3]<<od[2]<<od[1]<<od[0];
    for (int i=0;i<8;i++)
    {
        od[i]=log10(blnk[i]/od[i]);
    }
    qDebug()<<QString::number(od[7], 'f', 3)
            <<QString::number(od[6], 'f', 3)
            <<QString::number(od[5], 'f', 3)
            <<QString::number(od[4], 'f', 3)
            <<QString::number(od[3], 'f', 3)
            <<QString::number(od[2], 'f', 3)
            <<QString::number(od[1], 'f', 3)
            <<QString::number(od[0], 'f', 3);
}

void MainWindow::on_pushButton_17_clicked()
{
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);
    ulong range=70000;
    for (ulong i=0;i<range;i++)
    {
        if(digitalRead(hm_sen)==0)
            break;
        else
        {
            digitalWrite(steps, HIGH);
            accle(range,i);
            digitalWrite(steps, LOW);
            accle(range,i);
        }
    }
    digitalWrite(en,HIGH);
}

void MainWindow::on_pushButton_15_clicked()
{
    ulong range=ui->pushButton_18->text().toULong();
    mot_forward(range);
}

void MainWindow::on_pushButton_16_clicked()
{
    ulong range=ui->pushButton_19->text().toULong();
    mot_backward(range);
}

void MainWindow::on_pushButton_18_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Forward Steps",ui->pushButton_18->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_18->setText(data);
}

void MainWindow::on_pushButton_19_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Backward Steps",ui->pushButton_19->text());
    key->exec();
    QString data = key->getData();
    ui->pushButton_19->setText(data);
}

void MainWindow::accle(ulong total, ulong current)
{
    ulong accle_range=100;
    ulong speed=10;
    if(current<(accle_range-speed))
        QThread::usleep(accle_range-current);
    else if (current>(total-accle_range+speed))
        QThread::usleep(current-(total-accle_range));
    else
        QThread::usleep(speed);
}

void MainWindow::on_toolButton_18_clicked()
{
    mot_forward(17000);
    on_pushButton_13_clicked();//read max
    mot_forward(9750);
    double len = std::ceil(double(total)/8);
    int length=int(len);
    ulong end_pos=5800+((12-ulong(length))*2400);
    for(int i=0;i<length;i++)
    {
        on_pushButton_12_clicked();//read OD
        for(int k=0;k<8;k++)
            pri_res[i][k]=od[k];

        if(i!=length)
            mot_forward(2400);
    }
    if(sec_wave == 0)
    {
        led_control(0);
        on_pushButton_17_clicked();//home plate
    }
    else
    {
        mot_forward(end_pos);
        led_control(sec_wave);
        on_pushButton_13_clicked();//read max
        mot_backward(end_pos);
        for(int i=length-1;i>=0;i--)
        {
            on_pushButton_12_clicked();// read OD
            for(int k=0;k<8;k++)
                sec_res[i][k]=od[k];
            if(i!=0)
                mot_backward(2400);
        }
    }
    led_control(0);
    on_pushButton_17_clicked();//home plate
    if(sec_wave==0)
    {
        for(int i=0;i<length;i++)
            for(int k=0;k<8;k++)
                fin_res[i][k]=pri_res[i][k];
    }
    else
    {
        for(int i=0;i<length;i++)
            for(int k=0;k<8;k++)
                fin_res[i][k]=pri_res[i][k]-sec_res[i][k];
    }
    result_page();
}

void MainWindow::led_control(int led)
{
    int pwm[10]={500,500,500,500,500,500,500,500,led,5000};
    QString My_String;
    for(int i=0; i<10; i++)
    {
        My_String.append(QString::number(pwm[i]));
        if(i==9)
            My_String.append('\0');
        else
            My_String.append(" ");
    }
    qDebug()<<My_String;
    int len=My_String.length();
    char* ch;
    QByteArray ba=My_String.toLatin1();
    ch=ba.data();
    arduino.i2cWrite(ch,len);
}



void MainWindow::mot_forward(ulong range)
{
    digitalWrite(en,LOW);
    digitalWrite(dir,LOW);
    for (ulong i=0;i<range;i++)
    {
        digitalWrite(steps, HIGH);
        accle(range,i);
        digitalWrite(steps, LOW);
        accle(range,i);
    }
    digitalWrite(en,HIGH);
}

void MainWindow::mot_backward(ulong range)
{
    digitalWrite(en,LOW);
    digitalWrite(dir,HIGH);
    for (ulong i=0;i<range;i++)
    {
        if(digitalRead(hm_sen)==0)
            break;
        else
        {
            digitalWrite(steps, HIGH);
            accle(range,i);
            digitalWrite(steps, LOW);
            accle(range,i);
        }
    }
    digitalWrite(en,HIGH);
}

void MainWindow::on_toolButton_21_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
    qApp->processEvents();
    if(save==1)
    {
        save_results();
        save=0;
    }
}

void MainWindow::on_toolButton_26_clicked()
{
    on_toolButton_21_clicked();
}

void MainWindow::on_toolButton_25_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_toolButton_20_clicked()
{
    result_table();
    ui->stackedWidget->setCurrentIndex(7);

}


void MainWindow::result_page()
{
    ui->stackedWidget->setCurrentIndex(6);

    QDateTime dt(QDateTime::currentDateTime());
    ui->label_17->setText(dt.toString("dd-MM-yyyy"));
    ui->label_40->setText(dt.toString("dd-MM-yyyy"));
    ui->label_38->setText(dt.toString("hh.mm.ss"));
    ui->label_39->setText(dt.toString("hh.mm.ss"));

    ui->comboBox_13->clear();
    QStringList matrixlist;

    if(test_mode==1)
    {
        matrixlist<<"Pri. ABS"<<"Sec. ABS"<<"∆ ABS"<<"AVG. ABS";
        ui->comboBox_13->addItems(matrixlist);
        ui->comboBox_13->setCurrentIndex(3);
    }
    else
    {
        matrixlist<<"PRI. ABS"<<"SEC. ABS"<<"∆ ABS"<<"AVG. ABS"<<"RESULTS"<<"REMARKS";
        ui->comboBox_13->addItems(matrixlist);
        ui->comboBox_13->setCurrentIndex(3);
        ui->comboBox_13->setCurrentIndex(4);
    }

    //ui->label_38->setText(ct.toString(Qt::LocaleDate));
    //ui->label_39->setText(ct.toString(Qt::LocaleDate));


    double len = std::ceil(double(total)/8);
    int length=int(len);
    for(int i=0;i<length;i++)
        for(int k=0;k<8;k++)
        {
            abs_res[k+(i*8)]=fin_res[i][k];
            pri[k+(i*8)]=pri_res[i][k];
            sec[k+(i*8)]=sec_res[i][k];
        }

    QPushButton* samp_buttons[96] = { ui->A1_2,ui->B1_2,ui->C1_2,ui->D1_2,ui->E1_2,ui->F1_2,ui->G1_2,ui->H1_2,
                                      ui->A2_2,ui->B2_2,ui->C2_2,ui->D2_2,ui->E2_2,ui->F2_2,ui->G2_2,ui->H2_2,
                                      ui->A3_2,ui->B3_2,ui->C3_2,ui->D3_2,ui->E3_2,ui->F3_2,ui->G3_2,ui->H3_2,
                                      ui->A4_2,ui->B4_2,ui->C4_2,ui->D4_2,ui->E4_2,ui->F4_2,ui->G4_2,ui->H4_2,
                                      ui->A5_2,ui->B5_2,ui->C5_2,ui->D5_2,ui->E5_2,ui->F5_2,ui->G5_2,ui->H5_2,
                                      ui->A6_2,ui->B6_2,ui->C6_2,ui->D6_2,ui->E6_2,ui->F6_2,ui->G6_2,ui->H6_2,
                                      ui->A7_2,ui->B7_2,ui->C7_2,ui->D7_2,ui->E7_2,ui->F7_2,ui->G7_2,ui->H7_2,
                                      ui->A8_2,ui->B8_2,ui->C8_2,ui->D8_2,ui->E8_2,ui->F8_2,ui->G8_2,ui->H8_2,
                                      ui->A9_2,ui->B9_2,ui->C9_2,ui->D9_2,ui->E9_2,ui->F9_2,ui->G9_2,ui->H9_2,
                                      ui->A10_2,ui->B10_2,ui->C10_2,ui->D10_2,ui->E10_2,ui->F10_2,ui->G10_2,ui->H10_2,
                                      ui->A11_2,ui->B11_2,ui->C11_2,ui->D11_2,ui->E11_2,ui->F11_2,ui->G11_2,ui->H11_2,
                                      ui->A12_2,ui->B12_2,ui->C12_2,ui->D12_2,ui->E12_2,ui->F12_2,ui->G12_2,ui->H12_2};

    for(int i =0;i<96; i++)
    {
        disconnect(samp_buttons[i], &QPushButton::clicked, this, &MainWindow::button_clicked);
        connect(samp_buttons[i], &QPushButton::clicked, this, &MainWindow::button_clicked);
    }

    for(int i=0;i<96;i++)
    {
        samp_buttons[i]->setText("");
        samp_buttons[i]->setStyleSheet("background-color: rgb(255, 255, 255)");
        abs_avg[i]=0;
        pid[i]=res[i]=rem[i]="";
    }
    process_average();
    if(test_mode==2)
    {
        process_result_multistandard();
        ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Result"));
    }
    if(test_mode==3)
    {
        process_result_cutoff();
        ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("S/Co"));
    }


    result_matrix();
    result_table();
}

void MainWindow::result_table()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(total);
    for(int i=0;i<8;i++)
    {
        ui->tableWidget->setColumnWidth(i,65);
        if(i==7)
            ui->tableWidget->setColumnWidth(i,100);
    }
    //    ui->tableWidget->setColumnCount(8);
    //    ui->tableWidget->setColumnWidth(0,50);
    //    ui->tableWidget->setColumnWidth(3,100);

    double len = std::ceil(double(total)/8);
    int length=int(len), start=blank+nc+pc+lc+cc+total_cal;
    QString wel[8]={"A","B","C","D","E","F","G","H"};
    QTableWidgetItem *well, *samp, *absr, *absa, *resl, *remk, *unt;
    QPushButton *sid;
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<8;j++)
        {
            well=new QTableWidgetItem;
            samp=new QTableWidgetItem;
            absr=new QTableWidgetItem;
            absa=new QTableWidgetItem;
            resl=new QTableWidgetItem;
            unt=new QTableWidgetItem;
            remk=new QTableWidgetItem;
            sid= new QPushButton;

            well->setTextAlignment(Qt::AlignCenter);
            samp->setTextAlignment(Qt::AlignCenter);
            absr->setTextAlignment(Qt::AlignCenter);
            absa->setTextAlignment(Qt::AlignCenter);
            resl->setTextAlignment(Qt::AlignCenter);
            unt->setTextAlignment(Qt::AlignCenter);
            remk->setTextAlignment(Qt::AlignCenter);
            sid->setStyleSheet("Text-align:left");
            sid->setObjectName(QString::number(j+(i*8)));

            well->setText(wel[j]+QString::number(i+1));
            samp->setText(dis[j+(i*8)]);
            absr->setText(QString::number(abs_res[j+(i*8)],'f',3));
            absa->setText(QString::number(abs_avg[j+(i*8)],'f',3));
            resl->setText(res[j+(i*8)]);
            remk->setText(rem[j+(i*8)]);
            unt->setText(unit);
            sid->setText(pid[j+(i*8)]);
            disconnect(sid, &QPushButton::clicked, this, &MainWindow::sid_button);
            connect(sid, &QPushButton::clicked, this, &MainWindow::sid_button);

            ui->tableWidget->setItem(j+(i*8), 0, well);//well
            ui->tableWidget->setItem(j+(i*8), 1, samp);//sample
            ui->tableWidget->setItem(j+(i*8), 2, absr);//absorbance
            ui->tableWidget->setItem(j+(i*8), 3, absa);//average
            ui->tableWidget->setItem(j+(i*8), 4, resl);//result
            if(j+(i*8)>=start)
            {
                ui->tableWidget->setItem(j+(i*8), 5, unt);//unit
                ui->tableWidget->setItem(j+(i*8), 6, remk);//remark
                ui->tableWidget->setCellWidget(j+(i*8), 7, sid);//PID
            }

        }

    }
}

void MainWindow::result_matrix()
{
    QPushButton* samp_buttons[96] = { ui->A1_2,ui->B1_2,ui->C1_2,ui->D1_2,ui->E1_2,ui->F1_2,ui->G1_2,ui->H1_2,
                                      ui->A2_2,ui->B2_2,ui->C2_2,ui->D2_2,ui->E2_2,ui->F2_2,ui->G2_2,ui->H2_2,
                                      ui->A3_2,ui->B3_2,ui->C3_2,ui->D3_2,ui->E3_2,ui->F3_2,ui->G3_2,ui->H3_2,
                                      ui->A4_2,ui->B4_2,ui->C4_2,ui->D4_2,ui->E4_2,ui->F4_2,ui->G4_2,ui->H4_2,
                                      ui->A5_2,ui->B5_2,ui->C5_2,ui->D5_2,ui->E5_2,ui->F5_2,ui->G5_2,ui->H5_2,
                                      ui->A6_2,ui->B6_2,ui->C6_2,ui->D6_2,ui->E6_2,ui->F6_2,ui->G6_2,ui->H6_2,
                                      ui->A7_2,ui->B7_2,ui->C7_2,ui->D7_2,ui->E7_2,ui->F7_2,ui->G7_2,ui->H7_2,
                                      ui->A8_2,ui->B8_2,ui->C8_2,ui->D8_2,ui->E8_2,ui->F8_2,ui->G8_2,ui->H8_2,
                                      ui->A9_2,ui->B9_2,ui->C9_2,ui->D9_2,ui->E9_2,ui->F9_2,ui->G9_2,ui->H9_2,
                                      ui->A10_2,ui->B10_2,ui->C10_2,ui->D10_2,ui->E10_2,ui->F10_2,ui->G10_2,ui->H10_2,
                                      ui->A11_2,ui->B11_2,ui->C11_2,ui->D11_2,ui->E11_2,ui->F11_2,ui->G11_2,ui->H11_2,
                                      ui->A12_2,ui->B12_2,ui->C12_2,ui->D12_2,ui->E12_2,ui->F12_2,ui->G12_2,ui->H12_2};

    for(int i=0;i<total;i++)
    {

        if(test_mode==1)
        {
            if(ui->comboBox_13->currentIndex()==0)
                samp_buttons[i]->setText(dis[i]+'\n'+QString::number(pri[i],'f',3));
            else if(ui->comboBox_13->currentIndex()==1)
                samp_buttons[i]->setText(dis[i]+'\n'+QString::number(sec[i],'f',3));
            else if(ui->comboBox_13->currentIndex()==2)
                samp_buttons[i]->setText(dis[i]+'\n'+QString::number(abs_res[i],'f',3));
            else if(ui->comboBox_13->currentIndex()==3)
                samp_buttons[i]->setText(dis[i]+'\n'+QString::number(abs_avg[i],'f',3));
        }
        else
        {
            if(invalid==0)
            {
                if(ui->comboBox_13->currentIndex()==0)
                    samp_buttons[i]->setText(dis[i]+'\n'+QString::number(pri[i],'f',3));
                else if(ui->comboBox_13->currentIndex()==1)
                    samp_buttons[i]->setText(dis[i]+'\n'+QString::number(sec[i],'f',3));
                else if(ui->comboBox_13->currentIndex()==2)
                    samp_buttons[i]->setText(dis[i]+'\n'+QString::number(abs_res[i],'f',3));
                else if(ui->comboBox_13->currentIndex()==3)
                    samp_buttons[i]->setText(dis[i]+'\n'+QString::number(abs_avg[i],'f',3));
                else if(ui->comboBox_13->currentIndex()==4)
                    samp_buttons[i]->setText(dis[i]+'\n'+res[i]);
                else if(ui->comboBox_13->currentIndex()==5)
                    samp_buttons[i]->setText(dis[i]+'\n'+rem[i]);
            }
            else
                samp_buttons[i]->setText(dis[i]+'\n'+"INV");
        }

        if(i<blank)
            samp_buttons[i]->setStyleSheet("background-color: rgb(235, 235, 235)");
        else if(i<blank+nc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(40, 230, 100)");
        else if(i<blank+nc+pc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(245, 30, 20)");
        else if(i<blank+nc+pc+lc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(170, 20, 240)");
        else if(i<blank+nc+pc+lc+cc)
            samp_buttons[i]->setStyleSheet("background-color: rgb(240, 20, 120)");
        else if(i<blank+nc+pc+lc+cc+total_cal)
            samp_buttons[i]->setStyleSheet("background-color: rgb(240, 240, 20)");
        else if(i<total)
        {
            if(rem[i]=="OOR")
                samp_buttons[i]->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgb(20, 120, 240), stop:0.2 rgb(20, 120, 240), stop:0.205 rgb(20, 175, 10), stop:1 rgb(20, 175, 10))");
            else if(rem[i]=="POS" or rem[i]=="HIGH")
                samp_buttons[i]->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgb(245, 30, 20), stop:0.2 rgb(245, 30, 20), stop:0.205 rgb(20, 175, 10), stop:1 rgb(20, 175, 10))");
            else if(rem[i]=="EQV" or rem[i]=="LOW")
                samp_buttons[i]->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgb(240, 240, 20), stop:0.2 rgb(240, 240, 20), stop:0.205 rgb(20, 175, 10), stop:1 rgb(20, 175, 10))");
            else
                samp_buttons[i]->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 rgb(20, 175, 10), stop:0.2 rgb(20, 175, 10), stop:0.205 rgb(20, 175, 10), stop:1 rgb(20, 175, 10))");
        }
    }
}

void MainWindow::process_average()
{

    if(blank>0)
    {
        int start=0;
        int end=blank;
        double avg=0;

        for(int i=start;i<end;i++)
            avg+=abs_res[i];
        abs_avg[start]=avg/blank;

        for(int i=start;i<end;i++)
            abs_avg[i]=abs_avg[start];

        for(int i=end;i<total;i++)
            abs_avg[i]=abs_res[i]-abs_avg[start];
    }
    else
        for(int i=blank;i<total;i++)
            abs_avg[i]=abs_res[i];
    if(nc>0)
    {
        int start=blank;
        int end=blank+nc;
        double avg=0;
        for(int i=start;i<end;i++)
            avg+=abs_avg[i];
        abs_avg[start]=avg/nc;

        for(int i=start;i<end;i++)
            abs_avg[i]=abs_avg[start];
    }
    if(pc>0)
    {
        int start=blank+nc;
        int end=blank+nc+pc;
        double avg=0;
        for(int i=start;i<end;i++)
            avg+=abs_avg[i];
        abs_avg[start]=avg/pc;

        for(int i=start;i<end;i++)
            abs_avg[i]=abs_avg[start];
    }
    if(lc>0)
    {
        int start=blank+nc+pc;
        int end=blank+nc+pc+lc;
        double avg=0;
        for(int i=start;i<end;i++)
            avg+=abs_avg[i];
        abs_avg[start]=avg/lc;

        for(int i=start;i<end;i++)
            abs_avg[i]=abs_avg[start];
    }
    if(cc>0)
    {
        int start=blank+nc+pc+lc;
        int end=blank+nc+pc+lc+cc;
        double avg=0;
        for(int i=start;i<end;i++)
            avg+=abs_avg[i];
        abs_avg[start]=avg/cc;

        for(int i=start;i<end;i++)
            abs_avg[i]=abs_avg[start];
    }
    if(dup_cal==2)
    {
        int start=blank+nc+pc+lc+cc;
        int end=blank+nc+pc+lc+cc+total_cal;
        for(int i=start;i<end;i+=2)
            abs_avg[i]=abs_avg[i+1]=(abs_avg[i]+abs_avg[i+1])/2;
    }

    if(dup_samp==2)
    {
        int start=blank+nc+pc+lc+cc+total_cal;
        int end=blank+nc+pc+lc+cc+total_cal+total_samp;
        for(int i=start;i<end;i+=2)
            abs_avg[i]=abs_avg[i+1]=(abs_avg[i]+abs_avg[i+1])/2;
    }

}

void MainWindow::process_result_multistandard()
{
    int start=blank+nc+pc+lc+cc;
    int end=blank+nc+pc+lc+cc+total_cal;
    int graph=0;
    double norl=0, norh=0;

    double test[8]={2, 1.5, 1, 0.5, 0.15 , 0.1, 1.3, 2.5};//test conc
    for(int i=0;i<8;i++)//test conc
    {
        abs_avg[i]=test[i];
    }

    QString absstr[10]={"abs1","abs2","abs3","abs4","abs5","abs6","abs7","abs8","abs9","abs10"};
    QString constr[10]={"conc1","conc2","conc3","conc4","conc5","conc6","conc7","conc8","conc9","conc10"};
    QSqlQuery Query;
    Query.prepare("select * FROM tests WHERE name = :bname");
    Query.bindValue(":bname", btn_name);
    Query.exec();
    while(Query.next())
    {
        graph=Query.value("graph").toInt();
        norl=Query.value("norl").toDouble();
        norh=Query.value("norh").toDouble();
        unit=Query.value("unit").toString();
        for(int i=0;i<nostd;i++)
        {
            y_abs[i]=Query.value(absstr[i]).toDouble();
            x_conc[i]=Query.value(constr[i]).toDouble();
        }
    }

    if(cal!=0)//take cal abs from new reading
    {
        for(int i=start,j=0;i<end;i++,j++)
        {
            y_abs[j]=abs_avg[i];
            if(dup_cal==2)
                i++;
        }
    }

    int inc=0,dec=0;
    for(int i=0;i<nostd-1;i++)
    {
        if(!(y_abs[i]<y_abs[i+1]))
            inc=1;
        if(!(y_abs[i]>y_abs[i+1]))
            dec=1;
    }
    if(inc==1 && dec==1)
    {
        invalid=1;
        ui->textBrowser_5->setText("Invalid");
        ui->textBrowser_6->setText("Invalid");
        ui->toolButton_32->setDisabled(true);
        ui->toolButton_33->setDisabled(true);
        ui->toolButton_23->setDisabled(true);
        ui->toolButton_29->setDisabled(true);
        return;
    }
    else
    {
        invalid=0;
        int start=blank+nc+pc+lc+cc;
        int end=blank+nc+pc+lc+cc+total_cal;
        for(int i=start,j=0;i<end;i++,j++)// copying standard conc to the result array
        {
            res[i]=QString::number(x_conc[j],'f',3);
            if(dup_cal==2)
            {
                i++;
                res[i]=QString::number(x_conc[j],'f',3);
            }
        }
        start=blank+nc+pc+lc+cc+total_cal;
        end=blank+nc+pc+lc+cc+total_cal+total_samp;
        for(int i=start;i<end;i++)// copying result conc to the result array
        {
            res[i]=calculate_regression(abs_avg[i], graph);
            if(res[i][0]==">" or res[i][0]=="<")
                rem[i]="OOR";
            else if(res[i].toDouble()>=norl and res[i].toDouble()<=norh)
                rem[i]="NOR";
            else if(res[i].toDouble()<norl)
                rem[i]="LOW";
            else if(res[i].toDouble()>norh)
                rem[i]="HIGH";
            if(dup_samp==2)
            {
                i++;
                res[i]=res[i-1];
                rem[i]=rem[i-1];
            }
        }
    }
}

void MainWindow::process_result_cutoff()
{
    int controls=nc+pc+lc+cc;
    double abs=0,ncabs=0,pcabs=0,lcabs=0,ccabs=0;
    QString msg="";
    QJSEngine parsexpression;

    double test[8]={0.1, 1, 1, 0.15, 0.25 ,0.22, 0.27, 0.26};//test conc
    for(int i=0;i<8;i++)//test conc
    {
        abs_avg[i]=test[i];
    }
    //Taking cut off equation
    QSqlQuery Query;
    Query.prepare("select cutoff FROM tests WHERE name = :bname");
    Query.bindValue(":bname", btn_name);
    Query.exec();
    while(Query.next())
    {
        cuteqn=Query.value("cutoff").toString();
    }

    if(controls!=0)//take cutabs from new reading
    {
        QString ncqc,pcqc,lcqc,ccqc,qcval,qcstring;
        QRegExp rx("(\\>|\\<|\\=)");
        QStringList qc;
        QSqlQuery Query;
        Query.prepare("select * FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        while(Query.next())
        {
            ncqc=Query.value("ncqc").toString();
            pcqc=Query.value("pcqc").toString();
            lcqc=Query.value("lcqc").toString();
            ccqc=Query.value("ccqc").toString();
        }

        if(nc>0)
        {
            qcval=ncqc;
            int start=blank;
            int end=blank+nc;
            for(int i=start;i<end;i++)
                ncabs=abs=abs_avg[i];
            qc=qcval.split(rx);
            if(qc.last().toDouble()>0.0)
            {
                qcstring="abscon1con2qc";
                qcstring.replace("con1",QString(qcval[0]));
                if(qc.length()==3)
                    qcstring.replace("con2",QString(qcval[1]));
                else
                    qcstring.replace("con2","");
                parsexpression.globalObject().setProperty("abs",abs);
                parsexpression.globalObject().setProperty("qc",qc.last().toDouble());
                if(!parsexpression.evaluate(qcstring).toBool())
                    msg+="NC Failed";
            }
        }
        if(pc>0)
        {
            qcval=pcqc;
            int start=blank+nc;
            int end=blank+nc+pc;
            for(int i=start;i<end;i++)
                pcabs=abs=abs_avg[i];
            qc=qcval.split(rx);
            if(qc.last().toDouble()>0.0)
            {
                qcstring="abscon1con2qc";
                qcstring.replace("con1",QString(qcval[0]));
                if(qc.length()==3)
                    qcstring.replace("con2",QString(qcval[1]));
                else
                    qcstring.replace("con2","");
                parsexpression.globalObject().setProperty("abs",abs);
                parsexpression.globalObject().setProperty("qc",qc.last().toDouble());
                if(!parsexpression.evaluate(qcstring).toBool())
                    msg+="PC Failed";
            }
        }
        if(lc>0)
        {
            qcval=lcqc;
            int start=blank+nc+pc;
            int end=blank+nc+pc+lc;
            for(int i=start;i<end;i++)
                lcabs=abs=abs_avg[i];
            qc=qcval.split(rx);
            if(qc.last().toDouble()>0.0)
            {
                qcstring="abscon1con2qc";
                qcstring.replace("con1",QString(qcval[0]));
                if(qc.length()==3)
                    qcstring.replace("con2",QString(qcval[1]));
                else
                    qcstring.replace("con2","");
                parsexpression.globalObject().setProperty("abs",abs);
                parsexpression.globalObject().setProperty("qc",qc.last().toDouble());
                if(!parsexpression.evaluate(qcstring).toBool())
                    msg+="LC Failed";
            }
        }
        if(cc>0)
        {
            qcval=ccqc;
            int start=blank+nc+pc+lc;
            int end=blank+nc+pc+lc+cc;
            for(int i=start;i<end;i++)
                ccabs=abs=abs_avg[i];
            qc=qcval.split(rx);
            if(qc.last().toDouble()>0.0)
            {
                qcstring="abscon1con2qc";
                qcstring.replace("con1",QString(qcval[0]));
                if(qc.length()==3)
                    qcstring.replace("con2",QString(qcval[1]));
                else
                    qcstring.replace("con2","");
                parsexpression.globalObject().setProperty("abs",abs);
                parsexpression.globalObject().setProperty("qc",qc.last().toDouble());
                if(!parsexpression.evaluate(qcstring).toBool())
                    msg+="CC Failed";
            }
        }
        if(msg.length()>1)
        {
            invalid=1;
            ui->textBrowser_5->setText("Invalid\n"+msg);
            ui->textBrowser_6->setText("Invalid\n"+msg);
            ui->toolButton_32->setDisabled(true);
            ui->toolButton_33->setDisabled(true);
            ui->toolButton_23->setDisabled(true);
            ui->toolButton_29->setDisabled(true);
            return;
        }
        else
        {
            invalid=0;
            QString str=cuteqn;
            str.replace("ABS","Math.abs");
            str.replace("LOG","0.434294482*Math.log");
            str.replace("ALG(","Math.pow(10,");
            str.replace("ALN","Math.exp");
            str.replace("LN","Math.log");
            str.replace("SQRT","Math.sqrt");
            parsexpression.globalObject().setProperty("NC",ncabs);
            parsexpression.globalObject().setProperty("PC",pcabs);
            parsexpression.globalObject().setProperty("LC",lcabs);
            parsexpression.globalObject().setProperty("CC",ccabs);
            cutabs=parsexpression.evaluate(str).toNumber();
            qDebug()<<cutabs;
        }
    }
    else
    {
        QSqlQuery Query;
        Query.prepare("select cutabs, threshold FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        while(Query.next())
        {
            cutabs=Query.value("cutabs").toDouble();
        }
        qDebug()<<cutabs;
    }

    int thresh=0;
    double posidx=0,negidx=0,greyz=0;
    Query.prepare("select threshold, pos, neg, grey FROM tests WHERE name = :bname");
    Query.bindValue(":bname", btn_name);
    Query.exec();
    while(Query.next())
    {
        thresh=Query.value("threshold").toInt();
        posidx=Query.value("pos").toDouble();
        negidx=Query.value("neg").toDouble();
        greyz=Query.value("grey").toDouble();
    }
    if(greyz>0.0)
    {
        posidx=cutabs+((cutabs/100)*greyz);
        negidx=cutabs-((cutabs/100)*greyz);
        int start=blank+nc+pc+lc+cc+total_cal;
        int end=blank+nc+pc+lc+cc+total_cal+total_samp;
        for(int i=start;i<end;i++)
        {
            res[i]=QString::number(abs_avg[i]/cutabs,'f',3);
            if(thresh==0)
            {
                if(abs_avg[i]>=negidx and abs_avg[i]<=posidx)
                    rem[i]="EQV";
                else if(abs_avg[i]>posidx)
                    rem[i]="POS";
                else if(abs_avg[i]<negidx)
                    rem[i]="NEG";
            }
            else
            {
                if(abs_avg[i]>=negidx and abs_avg[i]<=posidx)
                    rem[i]="EQV";
                else if(abs_avg[i]>posidx)
                    rem[i]="NEG";
                else if(abs_avg[i]<negidx)
                    rem[i]="POS";
            }
            if(dup_samp==2)
            {
                i++;
                res[i]=res[i-1];
                rem[i]=rem[i-1];
            }
        }
    }
    else
    {
        int start=blank+nc+pc+lc+cc+total_cal;
        int end=blank+nc+pc+lc+cc+total_cal+total_samp;
        for(int i=start;i<end;i++)
        {
            res[i]=QString::number(abs_avg[i]/cutabs,'f',3);
            if(thresh==0)
            {
                if(abs_avg[i]/cutabs>=negidx and abs_avg[i]/cutabs<=posidx)
                    rem[i]="EQV";
                else if(abs_avg[i]/cutabs>posidx)
                    rem[i]="POS";
                else if(abs_avg[i]/cutabs<negidx)
                    rem[i]="NEG";
            }
            else
            {
                if(abs_avg[i]/cutabs>=negidx and abs_avg[i]/cutabs<=posidx)
                    rem[i]="EQV";
                else if(abs_avg[i]/cutabs>posidx)
                    rem[i]="NEG";
                else if(abs_avg[i]/cutabs<negidx)
                    rem[i]="POS";
            }
            if(dup_samp==2)
            {
                i++;
                res[i]=res[i-1];
                rem[i]=rem[i-1];
            }
        }
    }
}



QString MainWindow::calculate_regression(double val, int graph)
{
    RegressionLine::Points pnts;
    QString result;
    if(graph==0)//point to point
    {
        double min=y_abs[0],max=y_abs[0];
        int min_index=0,max_index=0;
        for(int i=0;i<nostd;i++)
        {
            if(min>y_abs[i])
            {
                min=y_abs[i];
                min_index=i;
            }
            if(max<y_abs[i])
            {
                max=y_abs[i];
                max_index=i;
            }
        }
        if(val<min || val>max)
        {
            if(val<min)
            {
                if(min_index==0)
                {
                    return (x_conc[0]<x_conc[nostd-1]) ? "<"+QString::number(x_conc[0],'f',3) : ">"+QString::number(x_conc[0],'f',3);
                }
                else
                {
                    return (x_conc[0]<x_conc[nostd-1]) ? ">"+QString::number(x_conc[nostd-1],'f',3) : "<"+QString::number(x_conc[nostd-1],'f',3);
                }
            }
            else
            {
                if(max_index==0)
                {
                    return (x_conc[0]<x_conc[nostd-1]) ? "<"+QString::number(x_conc[0],'f',3) : ">"+QString::number(x_conc[0],'f',3);
                }
                else
                {
                    return (x_conc[0]<x_conc[nostd-1]) ? ">"+QString::number(x_conc[nostd-1],'f',3) : "<"+QString::number(x_conc[nostd-1],'f',3);
                }
            }
        }
        else
        {
            for(int i=0;i<nostd-1;i++)
            {
                if((val>=y_abs[i] && val<=y_abs[i+1]) || (val<=y_abs[i] && val>=y_abs[i+1]))
                {
                    pnts[x_conc[i]]=y_abs[i];
                    pnts[x_conc[i+1]]=y_abs[i+1];
                    break;
                }
            }
            RegressionLine myLine(pnts);
            return QString::number((val-myLine.yIntercept())/myLine.slope(),'f',3);
        }
    }
    else// linear
    {
        for (int i = 0; i < nostd; i++)
        {
            pnts[x_conc[i]] = y_abs[i];
        }
        RegressionLine myLine(pnts);
        return QString::number((val-myLine.yIntercept())/myLine.slope(),'f',3);
    }
}

void MainWindow::on_toolButton_30_clicked()
{
    const int count = 10;
    double x_conc[count] = {0,2,5,10,15,20,25,30,35,40};
    double y_abs[count] = {0.012,0.2,0.6,1.2,1.5,1.9,2.4,2.6,2.9,3.1};
    RegressionLine::Points pnts;
    for (int i = 0; i < count; i++)
    {
        pnts[x_conc[i]] = y_abs[i];
    }
    RegressionLine myLine(pnts);
    qDebug() << "Slope = " << myLine.slope() << endl;
    qDebug() << "yIntercept = " << myLine.yIntercept() << endl;
    qDebug() << "Regression Coefficient = " << myLine.regressionCoefficient() << endl;
    double y=2.5;
    double x=(y-myLine.yIntercept())/myLine.slope();
    qDebug()<<x;
}

void MainWindow::on_toolButton_31_clicked()
{
    QString str;
    QSqlQuery Query;
    Query.prepare("select cutoff FROM tests WHERE name = :bname");
    Query.bindValue(":bname", btn_name);
    Query.exec();
    while(Query.next())
    {
        str=Query.value("cutoff").toString();
    }
    str.replace("ABS","Math.abs");
    str.replace("LOG","0.434294482*Math.log");
    str.replace("ALG(","Math.pow(10,");
    str.replace("ALN","Math.exp");
    str.replace("LN","Math.log");
    str.replace("SQRT","Math.sqrt");
    QJSEngine parsexpression;
    double result=parsexpression.evaluate(str).toNumber();
    qDebug()<<result;
}

void MainWindow::on_toolButton_32_clicked()
{
    if(test_mode==2)
    {
        QString qry;
        qry="update tests set abs1=:a1,abs2=:a2,abs3=:a3,abs4=:a4,abs5=:a5, abs6=:a6, abs7=:a7, abs8=:a8, abs9=:a9, abs10=:a10 where name=:name";
        QSqlQuery Query;
        Query.prepare(qry);
        Query.bindValue(":name",btn_name);
        Query.bindValue(":a1",y_abs[0]);
        Query.bindValue(":a2",y_abs[1]);
        Query.bindValue(":a3",y_abs[2]);
        Query.bindValue(":a4",y_abs[3]);
        Query.bindValue(":a5",y_abs[4]);
        Query.bindValue(":a6",y_abs[5]);
        Query.bindValue(":a7",y_abs[6]);
        Query.bindValue(":a8",y_abs[7]);
        Query.bindValue(":a9",y_abs[8]);
        Query.bindValue(":a10",y_abs[9]);
        Query.exec();
        ui->toolButton_32->setDisabled(true);
        ui->toolButton_33->setDisabled(true);
    }
    if(test_mode==3)
    {
        QString qry;
        qry="update tests set cutabs=:cutabs where name=:name";
        QSqlQuery Query;
        Query.prepare(qry);
        Query.bindValue(":name",btn_name);
        Query.bindValue(":cutabs",cutabs);
        Query.exec();
        ui->toolButton_32->setDisabled(true);
        ui->toolButton_33->setDisabled(true);
    }


}

void MainWindow::on_toolButton_33_clicked()
{
    on_toolButton_32_clicked();
}


void MainWindow::on_pushButton_20_clicked()
{
    Dialog *dia=new Dialog(this);
    dia->setModal(true);
    if(test_mode==1)
    {
        dia->setPage(2);
        dia->update_data(btn_name,pri_wave,sec_wave,cuteqn,cutabs,invalid,x_conc,y_abs, nostd,ui->label_17->text(),ui->label_38->text());
        dia->exec();
    }
    else if(test_mode==2)
    {
        dia->setPage(1);
        dia->update_data(btn_name,pri_wave,sec_wave,cuteqn,cutabs,invalid,x_conc,y_abs, nostd,ui->label_17->text(),ui->label_38->text());
        dia->exec();
    }
    else if(test_mode==3)
    {
        dia->setPage(0);
        dia->update_data(btn_name,pri_wave,sec_wave,cuteqn,cutabs,invalid,x_conc,y_abs, nostd,ui->label_17->text(),ui->label_38->text());
        dia->exec();
    }

}

void MainWindow::button_clicked()
{
    QObject *senderObj = sender();
    QString senderObjName = senderObj->objectName();
    QMap<QString, int> map;
    QString objname[96]={"A1_2","B1_2","C1_2","D1_2","E1_2","F1_2","G1_2","H1_2",
                         "A2_2","B2_2","C2_2","D2_2","E2_2","F2_2","G2_2","H2_2",
                         "A3_2","B3_2","C3_2","D3_2","E3_2","F3_2","G3_2","H3_2",
                         "A4_2","B4_2","C4_2","D4_2","E4_2","F4_2","G4_2","H4_2",
                         "A5_2","B5_2","C5_2","D5_2","E5_2","F5_2","G5_2","H5_2",
                         "A6_2","B6_2","C6_2","D6_2","E6_2","F6_2","G6_2","H6_2",
                         "A7_2","B7_2","C7_2","D7_2","E7_2","F7_2","G7_2","H7_2",
                         "A8_2","B8_2","C8_2","D8_2","E8_2","F8_2","G8_2","H8_2",
                         "A9_2","B9_2","C9_2","D9_2","E9_2","F9_2","G9_2","H9_2",
                         "A10_2","B10_2","C10_2","D10_2","E10_2","F10_2","G10_2","H10_2",
                         "A11_2","B11_2","C11_2","D11_2","E11_2","F11_2","G11_2","H11_2",
                         "A12_2","B12_2","C12_2","D12_2","E12_2","F12_2","G12_2","H12_2"};
    for (int i=0;i<96;i++)
    {
        map.insert(objname[i],i);
    }
    qDebug()<<map[senderObjName]<<senderObjName;

    if(map[senderObjName]<ui->tableWidget->rowCount())
    {
        QString well=ui->tableWidget->item(map[senderObjName],0)->text();
        Dialog *dia=new Dialog(this);
        dia->setModal(true);
        if(test_mode==1)
        {
            dia->setPage(2);
            dia->update_data(btn_name,pri_wave,sec_wave,cuteqn,cutabs,invalid,x_conc,y_abs, nostd,ui->label_17->text(),ui->label_38->text());
            dia->update_results(well,dis[map[senderObjName]],abs_res[map[senderObjName]],abs_avg[map[senderObjName]],res[map[senderObjName]],unit,rem[map[senderObjName]],pid[map[senderObjName]]);
            dia->exec();
            pid[map[senderObjName]] = dia->getdiaData();

        }
        else if(test_mode==2)
        {
            dia->setPage(1);
            dia->update_data(btn_name,pri_wave,sec_wave,cuteqn,cutabs,invalid,x_conc,y_abs, nostd,ui->label_17->text(),ui->label_38->text());
            dia->update_results(well,dis[map[senderObjName]],abs_res[map[senderObjName]],abs_avg[map[senderObjName]],res[map[senderObjName]],unit,rem[map[senderObjName]],pid[map[senderObjName]]);
            dia->exec();
            pid[map[senderObjName]] = dia->getdiaData();
        }
        else if(test_mode==3)
        {
            dia->setPage(0);
            dia->update_data(btn_name,pri_wave,sec_wave,cuteqn,cutabs,invalid,x_conc,y_abs, nostd,ui->label_17->text(),ui->label_38->text());
            dia->update_results(well,dis[map[senderObjName]],abs_res[map[senderObjName]],abs_avg[map[senderObjName]],res[map[senderObjName]],unit,rem[map[senderObjName]],pid[map[senderObjName]]);
            dia->exec();
            pid[map[senderObjName]] = dia->getdiaData();
        }
    }
}

void MainWindow::on_pushButton_21_clicked()
{
    on_pushButton_20_clicked();
}

void MainWindow::sid_button()
{
    QObject *senderObj = sender();
    QString senderObjName = senderObj->objectName();
    qDebug()<< "Button: " << senderObjName;
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Enter PID/SID for "+dis[senderObjName.toInt()],pid[senderObjName.toInt()]);
    key->exec();
    pid[senderObjName.toInt()]=key->getData();
    result_table();
}


void MainWindow::on_toolButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
    dttimer->stop();
}

void MainWindow::on_toolButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);

    int index=0,index1=0;
    ui->comboBox_14->clear();
    QStringList pnames = QPrinterInfo::availablePrinterNames();
    pnames<<"Print to PDF";
    ui->comboBox_14->addItems(pnames);
    QSqlQuery Query;
    Query.prepare("select * FROM settings WHERE sno = 1");
    Query.exec();
    while(Query.next())
    {
        index=ui->comboBox_14->findText(Query.value("printer").toString());
        index1=ui->comboBox_16->findText(Query.value("printformat").toString());
        ui->pushButton_34->setText(Query.value("lab").toString());
        ui->pushButton_35->setText(Query.value("add1").toString());
        ui->pushButton_36->setText(Query.value("add2").toString());
        ui->pushButton_37->setText(Query.value("user").toString());
    }
    ui->comboBox_14->setCurrentIndex(index);
    ui->comboBox_16->setCurrentIndex(index1);


    //date and time
    QDateTime dt(QDateTime::currentDateTime());
    ui->dateTimeEdit_2->setDateTime(dt.currentDateTime());
    ui->dateTimeEdit_3->setDateTime(dt.currentDateTime());
    dttimer->start(1000);
}

void MainWindow::update_time()
{
    QDateTime dt(QDateTime::currentDateTime());
    ui->dateTimeEdit_2->setDateTime(dt.currentDateTime());
}

void MainWindow::on_toolButton_22_clicked()
{
    //QDateTime dt(QDateTime::currentDateTime());
    QString path="/home/pi/reader/CSV/"+btn_name+" "+ui->label_17->text()+" "+ui->label_38->text()+".csv";
    QFile file(path);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {            QTextStream data( &file );
        QStringList strList;
        for( int c = 0; c < ui->tableWidget->columnCount(); ++c )
        {
            strList<<ui->tableWidget->horizontalHeaderItem(c)->data(Qt::DisplayRole).toString();
        }
        data << strList.join(";") << "\n";
        for( int r = 0; r < ui->tableWidget->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->tableWidget->columnCount(); ++c )
            {
                if(c==ui->tableWidget->columnCount()-1)
                {
                    strList <<pid[r];
                }
                else
                {
                    QTableWidgetItem* item = ui->tableWidget->item(r,c);
                    if (!item || item->text().isEmpty())
                    {
                        ui->tableWidget->setItem(r,c,new QTableWidgetItem(""));
                    }
                    strList <<ui->tableWidget->item( r, c )->text();
                }
            }
            data << strList.join( ";" )+"\n";
        }
        file.close();
    }
    //ui->toolButton_22->setDisabled(true);
    //ui->toolButton_27->setDisabled(true);
    ui->label_41->setText("Saved..");
    ui->label_42->setText("Saved..");
    timer->start(2000);
}

void MainWindow::on_toolButton_27_clicked()
{
    on_toolButton_22_clicked();
}

void MainWindow::on_toolButton_36_clicked()
{
    on_toolButton_35_clicked();
}

void MainWindow::on_toolButton_35_clicked()
{
    on_toolButton_8_clicked();
    qApp->processEvents();
    if(save==1)
    {
        save_results();
        save=0;
    }
}


void MainWindow::delete_label()
{
    ui->label_41->clear();
    ui->label_42->clear();
    timer->stop();

}

/*void MainWindow::save_results1()
{
    QDateTime dt(QDateTime::currentDateTime());
    qDebug()<<dt.currentDateTime();

    QSqlQuery query;
    query.prepare("insert into results (name, sid, abs, result, unit, rem, date, time) values(:name,:sid,:abs,:result,:unit,:rem,:date,:time)");
    QVariantList vname,vsid,vabs,vresult,vunit,vrem,vdate,vtime;
    for (int i=blank+nc+pc+lc+cc+total_cal; i < total;i++)
    {
        vname << btn_name;
        vsid << pid[i];
        vabs << QString::number(abs_avg[i],'f',3);
        vresult << res[i];
        vunit << unit;
        vrem << rem[i];
        vdate<<ui->label_17->text();
        vtime << ui->label_38->text();
    }

    query.addBindValue(vname);
    query.addBindValue(vsid);
    query.addBindValue(vabs);
    query.addBindValue(vresult);
    query.addBindValue(vunit);
    query.addBindValue(vrem);
    query.addBindValue(vdate);
    query.addBindValue(vtime);
    query.execBatch();
    qDebug()<<dt.currentDateTime();
}
*/

void MainWindow::save_results()
{
    QDateTime dt(QDateTime::currentDateTime());
    qDebug()<<dt.currentDateTime();
    QSqlQuery query;
    query.prepare("insert into results (name, sid, abs, result, unit, rem, date, time) values(:name,:sid,:abs,:result,:unit,:rem,:date,:time)");
    for (int i=blank+nc+pc+lc+cc+total_cal; i < total; i++)
    {
        query.bindValue(":name", btn_name);
        query.bindValue(":sid", pid[i]);
        query.bindValue(":abs", QString::number(abs_avg[i],'f',3));
        query.bindValue(":result", res[i]);
        query.bindValue(":unit", unit);
        query.bindValue(":rem", rem[i]);
        query.bindValue(":date", ui->label_17->text());
        query.bindValue(":time", ui->label_38->text());
        query.exec();
        if(dup_samp==2)
            i++;
    }
    qDebug()<<dt.currentDateTime();

}


void MainWindow::on_toolButton_23_clicked()
{
    save=1;
    ui->label_41->setText("Results Saved..");
    ui->label_42->setText("Results Saved..");
    timer->start(2000);
}

void MainWindow::on_toolButton_29_clicked()
{
    on_toolButton_23_clicked();
}


void MainWindow::on_pushButton_24_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
    ui->tableWidget_2->clearContents();
    QSqlQuery query;
    query.prepare("select count(*) from results");
    query.exec();
    int length=0;
    while(query.next())
    {
        length=query.value(0).toInt();
    }
    qDebug()<<length;
    ui->tableWidget_2->setRowCount(length);

    QStringList test, abs, result, unit, rem, date, time, psid;
    query.prepare("select * from results");
    query.exec();
    while(query.next())
    {
        test<<query.value("name").toString();
        abs<<query.value("abs").toString();
        result<<query.value("result").toString();
        unit<<query.value("unit").toString();
        rem<<query.value("rem").toString();
        date<<query.value("date").toString();
        time<<query.value("time").toString();
        psid<<query.value("sid").toString();
    }

    QTableWidgetItem *test_item, *abs_item, *result_item, *unit_item, *rem_item, *date_item, *time_item, *psid_item;
    for(int i=0;i<length;i++)
    {
        test_item=new QTableWidgetItem;
        abs_item=new QTableWidgetItem;
        result_item=new QTableWidgetItem;
        unit_item=new QTableWidgetItem;
        rem_item=new QTableWidgetItem;
        date_item=new QTableWidgetItem;
        time_item=new QTableWidgetItem;
        psid_item= new QTableWidgetItem;

        test_item->setTextAlignment(Qt::AlignCenter);
        abs_item->setTextAlignment(Qt::AlignCenter);
        result_item->setTextAlignment(Qt::AlignCenter);
        unit_item->setTextAlignment(Qt::AlignCenter);
        rem_item->setTextAlignment(Qt::AlignCenter);
        date_item->setTextAlignment(Qt::AlignCenter);
        time_item->setTextAlignment(Qt::AlignCenter);
        psid_item->setTextAlignment(Qt::AlignVCenter);
        psid_item->setTextAlignment(Qt::AlignLeft);

        test_item->setText(test[i]);
        abs_item->setText(abs[i]);
        result_item->setText(result[i]);
        unit_item->setText(unit[i]);
        rem_item->setText(rem[i]);
        date_item->setText(date[i]);
        time_item->setText(time[i]);
        psid_item->setText(psid[i]);

        ui->tableWidget_2->setItem(length-i-1,0,test_item);
        ui->tableWidget_2->setItem(length-i-1,1,abs_item);
        ui->tableWidget_2->setItem(length-i-1,2,result_item);
        ui->tableWidget_2->setItem(length-i-1,3,unit_item);
        ui->tableWidget_2->setItem(length-i-1,4,rem_item);
        ui->tableWidget_2->setItem(length-i-1,5,date_item);
        ui->tableWidget_2->setItem(length-i-1,6,time_item);
        ui->tableWidget_2->setItem(length-i-1,7,psid_item);
    }

}

void MainWindow::on_pushButton_22_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
    ui->listWidget->clear();
    QDir dire("/home/pi/reader/CSV");
    QStringList filters;
    filters << "*.csv";
    for (const QFileInfo &file : dire.entryInfoList(filters,QDir::Files))
    {
        QListWidgetItem *item = new QListWidgetItem(file.fileName().left(file.fileName().indexOf(".csv")));
        qDebug()<<file.fileName()<<file.fileName().length()<<file.fileName().left(file.fileName().indexOf(".csv"));
        item->setData(Qt::UserRole, file.absolutePath()); // if you need absolute path of the file
        ui->listWidget->addItem(item);
    }
    int n=ui->tableWidget->currentRow();
    int col=ui->tableWidget->columnCount();
    qDebug()<<n<<col;
    for(int i=0;i<ui->tableWidget->columnCount();i++)
    {
        if(ui->tableWidget->item(n,i))
        {
            QString val=ui->tableWidget->item(n,i)->text();
            qDebug()<<val;
        }
        //qDebug()<<n<<i;
    }
    if(ui->listWidget->count()==0)
    {
        ui->pushButton_25->setDisabled(true);
        ui->pushButton_26->setDisabled(true);
    }
    else
    {
        ui->pushButton_25->setDisabled(false);
        ui->pushButton_26->setDisabled(false);
        ui->listWidget->setCurrentRow(0);
    }
}

void MainWindow::on_pushButton_25_clicked()
{

    ui->stackedWidget->setCurrentIndex(13);
    ui->tableWidget_3->clearContents();
    //QString fileName = QFileDialog::getOpenFileName(this, ("Open File"), NULL, ("csv File(*.csv)"));
    QString fileName="/home/pi/reader/CSV/"+ui->listWidget->currentItem()->text()+".csv";
    qDebug()<<fileName;
    QString data;
    QFile importedCSV(fileName);
    QStringList rowOfData;
    QStringList rowData;
    data.clear();
    rowOfData.clear();
    rowData.clear();

    if (importedCSV.open(QFile::ReadOnly))
    {
        data = importedCSV.readAll();
        rowOfData = data.split("\n");           //Value on each row
        importedCSV.close();
    }
    ui->tableWidget_3->setRowCount(rowOfData.size()-2);
    for(int i=0;i<8;i++)
    {
        ui->tableWidget_3->setColumnWidth(i,65);
        if(i==7)
            ui->tableWidget_3->setColumnWidth(i,100);
    }

    for (int x = 0; x < rowOfData.size(); x++)  //rowOfData.size() gives the number of row
    {
        if(x==0)
        {
            rowData = rowOfData.at(x).split(";");   //Number of collumn
            //int r=rowData.size();
            for (int y = 0; y < rowData.size(); y++)
            {
                //ui->tableWidget_3->setItem(x-1,y,new QTableWidgetItem(rowData[y]));
                ui->tableWidget_3->setHorizontalHeaderItem(y, new QTableWidgetItem(rowData[y]));
            }
        }
        else
        {
            rowData = rowOfData.at(x).split(";");   //Number of collumn
            //int r=rowData.size();
            for (int y = 0; y < rowData.size(); y++)
            {
                ui->tableWidget_3->setItem(x-1,y,new QTableWidgetItem(rowData[y]));
            }
        }
    }
    //statusBar()->showMessage(tr("File successfully loaded."), 3000);

}

void MainWindow::on_pushButton_27_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void MainWindow::on_comboBox_13_currentIndexChanged(int index)
{
    result_matrix();
}


void MainWindow::on_comboBox_14_activated(int index)
{
    QSqlQuery query;
    query.prepare("update settings set printer=:printer where sno=1");
    query.bindValue(":printer",ui->comboBox_14->currentText());
    query.exec();
}

void MainWindow::on_toolButton_34_clicked()
{
    doc.clear();
    QString doctext;
    QString text("<head><style>table, th, td {border: 1px solid black;border-collapse: collapse;text-align:left;font-size:10px;}table.center { margin-left: auto; margin-right: auto;}</style></head>");
    text.append("<body>");
    text.append("<h1 style='font-size:10px;text-align:center'>");
    text.append(btn_name).append("   ").append(ui->label_40->text()).append("   ").append(ui->label_39->text());
    text.append("</h1>");
    text.append("<table class='center' style='width:30%'>");
    text.append("<tr>");

    doctext.append("<h1 style='font-size:8px;text-align:center'>");
    doctext.append(btn_name).append("   ").append(ui->label_40->text()).append("   ").append(ui->label_39->text());
    doctext.append("</h1>");
    doctext.append("<table>");
    doctext.append("<tr>");

    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        text.append("<th>").append(ui->tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()).append(" ").append("</th>");
        doctext.append("<th>").append(ui->tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()).append(" ").append("</th>");
    }
    text.append("</tr>");
    doctext.append("</tr>");
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        text.append("<tr>");
        doctext.append("<tr>");
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            if(j==ui->tableWidget->columnCount()-1)
            {
                text.append("<td>").append(pid[i]).append("</td>");
                doctext.append("<td>").append(pid[i]).append("</td>");
            }
            else
            {
                QTableWidgetItem *item = ui->tableWidget->item(i, j);
                if (!item || item->text().isEmpty())
                {
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(" "));
                }
                text.append("<td>").append(ui->tableWidget->item(i, j)->text()+" ").append("</td>");
                QString data=ui->tableWidget->item(i, j)->text();
                if(data[0]=="<")
                    data.replace("<","&lt;");//since html not considering < as character, it affect the printer
                doctext.append("<td>").append(data+" ").append("</td>");
            }
            if(i!=ui->tableWidget->rowCount()-1)
            {
                //text.append("<hr>"); //if required line between each row
            }
        }
        text.append("</tr>");
        doctext.append("</tr>");
    }
    text.append("</table></body>");
    doctext.append("</table>");
    web.setHtml(text);
    doc.setHtml(doctext);
    print_process(84,0);
}


void MainWindow::on_toolButton_24_clicked()
{
    doc.clear();
    QString doctext;
    QString vind[8]={"A","B","C","D","E","F","G","H"};
    QString hind[13]={" 00 "," 01 "," 02 "," 03 "," 04 "," 05 "," 06 "," 07 "," 08 "," 09 "," 10 "," 11 "," 12 "};
    QString text("<head><style>table, th, td {border: 1px solid black;border-collapse: collapse;text-align:left;font-size:10px;background-color: white);}table.center { margin-left: auto; margin-right: auto;}</style></head>");
    text.append("<body>");
    text.append("<h1 style='font-size:10px;text-align:center'>");
    text.append(btn_name).append("   ").append(ui->comboBox_13->currentText()).append("   ").append(ui->label_40->text()).append("   ").append(ui->label_39->text());
    text.append("</h1>");
    text.append("<table class='center' style='width:30%'>");
    text.append("<tr>");

    doctext.append("<h1 style='font-size:8px;text-align:center'>");
    doctext.append(btn_name).append("   ").append(ui->comboBox_13->currentText()).append("   ").append(ui->label_40->text()).append("   ").append(ui->label_39->text());
    doctext.append("</h1>");
    doctext.append("<table>");
    doctext.append("<tr>");
    for(int j=0;j<13;j++)
    {
        text.append("<td>").append(hind[j]).append("</td>");
        doctext.append("<td>").append(hind[j]).append("</td>");
    }
    text.append("</tr>");
    doctext.append("</tr>");

    for (int i = 0; i < 8; i++)
    {
        text.append("<tr>");
        doctext.append("<tr>");
        for(int j=0;j<12;j++)
        {
            if(j==0)
            {
                text.append("<td>").append(vind[i]).append("</td>");
                doctext.append("<td>").append(vind[i]).append("</td>");
            }
            text.append("<td>");
            doctext.append("<td>");

            if(ui->comboBox_13->currentIndex()==0)
            {
                text.append(QString::number(pri[i+j*8],'f',3));
                doctext.append(QString::number(pri[i+j*8],'f',3));
            }
            else if(ui->comboBox_13->currentIndex()==1)
            {
                text.append(QString::number(sec[i+j*8],'f',3));
                doctext.append(QString::number(sec[i+j*8],'f',3));
            }
            else if(ui->comboBox_13->currentIndex()==2)
            {
                text.append(QString::number(abs_res[i+j*8],'f',3));
                doctext.append(QString::number(abs_res[i+j*8],'f',3));
            }
            else if(ui->comboBox_13->currentIndex()==3)
            {
                text.append(QString::number(abs_avg[i+j*8],'f',3));
                doctext.append(QString::number(abs_avg[i+j*8],'f',3));
            }
            else if(ui->comboBox_13->currentIndex()==4)
            {
                text.append(res[i+j*8]);
                if(res[i+j*8][0]=="<")
                {
                    QString data=res[i+j*8];
                    data.replace("<","&lt;");
                    doctext.append(data);
                }
                else
                {
                   doctext.append(res[i+j*8]);
                }

            }
            else if(ui->comboBox_13->currentIndex()==5)
            {
                text.append(rem[i+j*8]);
                doctext.append(rem[i+j*8]);
            }
            text.append("</td>");
            doctext.append("</td>");
            //text.append("<hr>");//if required line between each row
        }
        text.append("</tr>");
        doctext.append("</tr>");
    }
    text.append("</table></body>");
    doctext.append("</table>");
    web.setHtml(text);
    doc.setHtml(doctext);
    print_process(105,0);
}


void MainWindow::on_toolButton_28_clicked()
{
    QStringList head,val;
    int n=ui->tableWidget->currentRow();
    for( int i = 0; i < ui->tableWidget->columnCount(); i++ )
    {
        head<<ui->tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString();
        if(ui->tableWidget->item(n,i))
        {
            val<<ui->tableWidget->item(n,i)->text();
        }
        else if(i==ui->tableWidget->columnCount()-1)
        {
            val<<pid[n];
        }
        else
        {
            val<<" ";
        }
    }
    qDebug()<<val;
    qDebug()<<head;
    doc.clear();
    QString doctext;
    QString text("<head><style>table, th, td {border: 1px solid black;border-collapse: collapse;text-align:left;font-size:10px;}table.center { margin-left: auto; margin-right: auto;}</style></head>");
    text.append("<body>");
    text.append("<h1 style='font-size:10px;text-align:center'>");
    text.append(btn_name).append("<br>").append(ui->label_40->text()).append("<br>").append(ui->label_39->text());
    text.append("</h1>");
    text.append("<table class='center' style='width:30%'>");

    doctext.append("<h1 style='font-size:8px;text-align:center'>");
    doctext.append(btn_name).append("<br>").append(ui->label_40->text()).append("<br>").append(ui->label_39->text());
    doctext.append("</h1>");
    doctext.append("<table>");
    for (int i = 0; i < head.length(); i++)
    {
        text.append("<tr>");
        text.append("<th>").append(head[i]).append(" ").append("</th>");
        text.append("<td>").append(val[i]).append(" ").append("</td>");
        text.append("</tr>");

        doctext.append("<tr>");
        doctext.append("<th>").append(head[i]).append(" ").append("</th>");
        if(val[i][0]=="<")
        {
            QString data=val[i];
            data.replace("<","&lt;");
            doctext.append("<td>").append(data).append(" ").append("</td>");
        }
        else
        {
            doctext.append("<td>").append(val[i]).append(" ").append("</td>");
        }
        doctext.append("</tr>");
    }
    text.append("</table></body>");
    doctext.append("</table>");
    web.setHtml(text);
    doc.setHtml(doctext);
    print_process(65,1);

}


void MainWindow::print_process(int paper_length, int individual)
{
    QPrinter printer;
    QString printername,format;
    QSqlQuery Query;
    Query.prepare("select * FROM settings WHERE sno = 1");
    Query.exec();
    while(Query.next())
    {
        printername=Query.value("printer").toString();
        format=Query.value("printformat").toString();
    }

    printer.setPrinterName(printername);
    if(printername=="Internal")
    {
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageOrder(QPrinter::LastPageFirst);//not working, need to check
        printer.setPaperSize(QSize(58, paper_length),QPrinter::Millimeter);//paper_length 84 for table and 125 for matrix
        //printer.setPageMargins(10,10,10,10, QPrinter::Millimeter);
        QFont font;
        font.setPointSize(6);
        font.setBold(QFont::DemiBold);
        font.setFamily("Calibri");
        font.setLetterSpacing(QFont::PercentageSpacing,100);
        doc.setDefaultFont(font);
        if(individual==1)
        {
            printer.setOrientation(QPrinter::Portrait);
            printer.setPaperSize(QSize(58, paper_length),QPrinter::Millimeter);//paper_length 84 for table and 125 for matrix
        }

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
            QString path="/home/pi/reader/PDF/"+btn_name+" "+ui->label_17->text()+" "+ui->label_38->text()+".pdf";
            printer.setOutputFileName(path);
        }
    }
    if(format=="With Border")
        web.print(&printer);
    else
    {
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);
    }
}


void MainWindow::on_comboBox_16_activated(int index)
{
    QSqlQuery query;
    query.prepare("update settings set printformat=:printer where sno=1");
    query.bindValue(":printer",ui->comboBox_16->currentText());
    query.exec();
}

void MainWindow::on_pushButton_26_clicked()
{

    QString fileName="/home/pi/reader/CSV/"+ui->listWidget->currentItem()->text()+".csv";
    QFile file (fileName);
    file.remove();
    on_pushButton_22_clicked();

}

void MainWindow::on_pushButton_38_clicked()
{
    QStringList head,val;
    int n=ui->tableWidget_3->currentRow();
    for( int i = 0; i < ui->tableWidget_3->columnCount(); i++ )
    {
        head<<ui->tableWidget_3->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString();
        if(ui->tableWidget_3->item(n,i))
        {
            val<<ui->tableWidget_3->item(n,i)->text();
        }
        else if(i==ui->tableWidget_3->columnCount()-1)
        {
            val<<pid[n];
        }
        else
        {
            val<<" ";
        }
    }
    qDebug()<<val;
    qDebug()<<head;
    QString fileName=ui->listWidget->currentItem()->text();
    QStringList list1 = fileName.split(QRegExp("\\s+"));
    btn_name=list1[0];
    ui->label_17->setText(list1[1]);
    ui->label_38->setText(list1[2]);
    doc.clear();
    QString doctext;
    QString text("<head><style>table, th, td {border: 1px solid black;border-collapse: collapse;text-align:left;font-size:10px;}table.center { margin-left: auto; margin-right: auto;}</style></head>");
    text.append("<body>");
    text.append("<h1 style='font-size:10px;text-align:center'>");
    text.append(list1[0]).append("<br>").append(list1[1]).append("<br>").append(list1[2]);
    text.append("</h1>");
    text.append("<table class='center' style='width:30%'>");

    doctext.append("<h1 style='font-size:8px;text-align:center'>");
    doctext.append(list1[0]).append("<br>").append(list1[1]).append("<br>").append(list1[2]);
    doctext.append("</h1>");
    doctext.append("<table>");
    for (int i = 0; i < head.length(); i++)
    {
        text.append("<tr>");
        text.append("<th>").append(head[i]).append(" ").append("</th>");
        text.append("<td>").append(val[i]).append(" ").append("</td>");
        text.append("</tr>");

        doctext.append("<tr>");
        doctext.append("<th>").append(head[i]).append(" ").append("</th>");
        if(val[i][0]=="<")
        {
            QString data=val[i];
            data.replace("<","&lt;");
            doctext.append("<td>").append(data).append(" ").append("</td>");
        }
        else
        {
            doctext.append("<td>").append(val[i]).append(" ").append("</td>");
        }
        doctext.append("</tr>");
    }
    text.append("</table></body>");
    doctext.append("</table>");
    web.setHtml(text);
    doc.setHtml(doctext);
    print_process(65,1);
}

void MainWindow::on_pushButton_51_clicked()
{
    doc.clear();
    QString doctext;
    QString fileName=ui->listWidget->currentItem()->text();
    QStringList list1 = fileName.split(QRegExp("\\s+"));
    btn_name=list1[0];
    ui->label_17->setText(list1[1]);
    ui->label_38->setText(list1[2]);
    QString text("<head><style>table, th, td {border: 1px solid black;border-collapse: collapse;text-align:left;font-size:10px;}table.center { margin-left: auto; margin-right: auto;}</style></head>");
    text.append("<body>");
    text.append("<h1 style='font-size:10px;text-align:center'>");
    text.append(list1[0]).append("   ").append(list1[1]).append("   ").append(list1[2]);
    text.append("</h1>");
    text.append("<table class='center' style='width:30%'>");
    text.append("<tr>");

    doctext.append("<h1 style='font-size:8px;text-align:center'>");
    doctext.append(list1[0]).append("   ").append(list1[1]).append("   ").append(list1[2]);
    doctext.append("</h1>");
    doctext.append("<table>");
    doctext.append("<tr>");

    for (int i = 0; i < ui->tableWidget_3->columnCount(); i++)
    {
        text.append("<th>").append(ui->tableWidget_3->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()).append(" ").append("</th>");
        doctext.append("<th>").append(ui->tableWidget_3->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()).append(" ").append("</th>");
    }
    text.append("</tr>");
    doctext.append("</tr>");
    for (int i = 0; i < ui->tableWidget_3->rowCount(); i++)
    {
        text.append("<tr>");
        doctext.append("<tr>");
        for (int j = 0; j < ui->tableWidget_3->columnCount(); j++)
        {
            if(j==ui->tableWidget_3->columnCount()-1)
            {
                text.append("<td>").append(pid[i]).append("</td>");
                doctext.append("<td>").append(pid[i]).append("</td>");
            }
            else
            {
                QTableWidgetItem *item = ui->tableWidget_3->item(i, j);
                if (!item || item->text().isEmpty())
                {
                    ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(" "));
                }
                text.append("<td>").append(ui->tableWidget_3->item(i, j)->text()+" ").append("</td>");
                QString data=ui->tableWidget_3->item(i, j)->text();
                if(data[0]=="<")
                    data.replace("<","&lt;");//since html not considering < as character, it affect the printer
                doctext.append("<td>").append(data+" ").append("</td>");
            }
            if(i!=ui->tableWidget_3->rowCount()-1)
            {
                //text.append("<hr>"); //if required line between each row
            }
        }
        text.append("</tr>");
        doctext.append("</tr>");
    }
    text.append("</table></body>");
    doctext.append("</table>");
    web.setHtml(text);
    doc.setHtml(doctext);
    print_process(84,0);
}

void MainWindow::on_pushButton_34_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Lab Name",ui->pushButton_34->text());
    key->exec();
    ui->pushButton_34->setText(key->getData());
    QSqlQuery query;
    query.prepare("update settings set lab=:lab where sno=1");
    query.bindValue(":lab",key->getData());
    query.exec();
}

void MainWindow::on_pushButton_35_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Lab Address 1",ui->pushButton_35->text());
    key->exec();
    ui->pushButton_35->setText(key->getData());
    QSqlQuery query;
    query.prepare("update settings set add1=:lab where sno=1");
    query.bindValue(":lab",key->getData());
    query.exec();
}

void MainWindow::on_pushButton_36_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Lab Address 2",ui->pushButton_36->text());
    key->exec();
    ui->pushButton_36->setText(key->getData());
    QSqlQuery query;
    query.prepare("update settings set add2=:lab where sno=1");
    query.bindValue(":lab",key->getData());
    query.exec();
}

void MainWindow::on_pushButton_37_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("User Info",ui->pushButton_37->text());
    key->exec();
    ui->pushButton_37->setText(key->getData());
    QSqlQuery query;
    query.prepare("update settings set user=:lab where sno=1");
    query.bindValue(":lab",key->getData());
    query.exec();
}



void MainWindow::on_pushButton_52_clicked()
{
    QDateTime newdatetime=ui->dateTimeEdit_3->dateTime();
    QString changeDT=newdatetime.toString("yyyy-MM-dd HH:mm:ss");
    QString command = "sudo date -s '"+changeDT+"'";
    system(command.toUtf8().constData());
}
