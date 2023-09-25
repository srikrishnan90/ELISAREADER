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
static int blank=0,cal=0,dup_cal=1,nc=0,pc=0,cc=0,lc=0,samp=0,dup_samp=1,total=0,total_cal=0,total_samp=0;
static int led_freq=10000;
static double offset[8], blnk[8], od[8];
static int pri_wave=0,sec_wave=0;
static double pri_res[12][8],sec_res[12][8],fin_res[12][8],abs_res[96],abs_avg[96];
static QString dis[96];
static Pi2c arduino(7);

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
    sqdb.setDatabaseName("/home/pi/Desktop/er.db");
    if(!sqdb.open())
        qDebug() << "Can't Connect to DB !";
    else
        qDebug() << "Connected Successfully to DB !";
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
    test_menu();
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
    key->setData("Experiment Name");
    key->exec();
    QString data = key->getData();
    ui->pushButton_46->setText(data);
}

void MainWindow::on_pushButton_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Experiment Name");
    key->exec();
    QString data = key->getData();
    ui->pushButton->setText(data);
}

void MainWindow::on_pushButton_3_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of Calibrators");
    key->exec();
    QString data = key->getData();
    ui->pushButton_3->setText(data);
}

void MainWindow::on_pushButton_4_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Low Range");
    key->exec();
    QString data = key->getData();
    ui->pushButton_4->setText(data);
}

void MainWindow::on_pushButton_5_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("High Range");
    key->exec();
    QString data = key->getData();
    ui->pushButton_5->setText(data);
}

void MainWindow::on_pushButton_6_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Unit");
    key->exec();
    QString data = key->getData();
    ui->pushButton_6->setText(data);
}

void MainWindow::on_c1_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-1 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c1->setText(data);
}

void MainWindow::on_c2_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-2 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c2->setText(data);
}

void MainWindow::on_c3_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-3 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c3->setText(data);
}

void MainWindow::on_c4_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-4 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c4->setText(data);
}

void MainWindow::on_c5_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-5 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c5->setText(data);
}

void MainWindow::on_c6_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-6 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c6->setText(data);
}

void MainWindow::on_c7_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-7 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c7->setText(data);
}

void MainWindow::on_c8_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-8 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c8->setText(data);
}

void MainWindow::on_c9_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-9 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c9->setText(data);
}

void MainWindow::on_c10_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-10 Conc.");
    key->exec();
    QString data = key->getData();
    ui->c10->setText(data);
}

void MainWindow::on_a1_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-1 ABS");
    key->exec();
    QString data = key->getData();
    ui->a1->setText(data);
}

void MainWindow::on_a2_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-2 ABS");
    key->exec();
    QString data = key->getData();
    ui->a2->setText(data);
}

void MainWindow::on_a3_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-3 ABS");
    key->exec();
    QString data = key->getData();
    ui->a3->setText(data);
}

void MainWindow::on_a4_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-4 ABS");
    key->exec();
    QString data = key->getData();
    ui->a4->setText(data);
}

void MainWindow::on_a5_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-5 ABS");
    key->exec();
    QString data = key->getData();
    ui->a5->setText(data);
}

void MainWindow::on_a6_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-6 ABS");
    key->exec();
    QString data = key->getData();
    ui->a6->setText(data);
}

void MainWindow::on_a7_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-7 ABS");
    key->exec();
    QString data = key->getData();
    ui->a7->setText(data);
}

void MainWindow::on_a8_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-8 ABS");
    key->exec();
    QString data = key->getData();
    ui->a8->setText(data);
}

void MainWindow::on_a9_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-9 ABS");
    key->exec();
    QString data = key->getData();
    ui->a9->setText(data);
}

void MainWindow::on_a10_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("STD-10 ABS");
    key->exec();
    QString data = key->getData();
    ui->a10->setText(data);
}

void MainWindow::on_pushButton_39_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(0);
    key->setData("Experiment Name");
    key->exec();
    QString data = key->getData();
    ui->pushButton_39->setText(data);
}

void MainWindow::on_pushButton_40_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of NC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_40->setText(data);
}

void MainWindow::on_pushButton_41_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of PC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_41->setText(data);
}

void MainWindow::on_pushButton_42_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of LC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_42->setText(data);
}

void MainWindow::on_pushButton_67_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("No. of CC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_67->setText(data);
}

void MainWindow::on_pushButton_64_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of NC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_64->setText(data);
}

void MainWindow::on_pushButton_65_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of PC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_65->setText(data);
}

void MainWindow::on_pushButton_66_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of LC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_66->setText(data);
}

void MainWindow::on_pushButton_68_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("QC of CC");
    key->exec();
    QString data = key->getData();
    ui->pushButton_68->setText(data);
}

void MainWindow::on_pushButton_43_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Cutoff Equation");
    key->exec();
    QString data = key->getData();
    ui->pushButton_43->setText(data);
}

void MainWindow::on_pushButton_44_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Positive Index");
    key->exec();
    QString data = key->getData();
    ui->pushButton_44->setText(data);
}

void MainWindow::on_pushButton_45_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Negative Index");
    key->exec();
    QString data = key->getData();
    ui->pushButton_45->setText(data);
}

void MainWindow::on_pushButton_47_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("% Greyzone");
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
    QString unit=ui->pushButton_6->text();

    QPushButton* concb[] = { ui->c1,ui->c2,ui->c3,ui->c4,ui->c5,ui->c6,ui->c7,ui->c8,ui->c9,ui->c10};
    QPushButton* absb[] = { ui->a1,ui->a2,ui->a3,ui->a4,ui->a5,ui->a6,ui->a7,ui->a8,ui->a9,ui->a10};

    QString calbindstring[10]={":c1",":c2",":c3",":c4",":c5",":c6",":c7",":c8",":c9",":c10"};
    QString absbindstring[10]={":a1",":a2",":a3",":a4",":a5",":a6",":a7",":a8",":a9",":a10"};
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


    /*for(int i=0;i<10;i++)
    {
        insertQuery.bindValue(calbindstring[i],cal[i]);
        qDebug()<<calbindstring[i]<<cal[i];
        insertQuery.bindValue(absbindstring[i],abs[i]);
    }*/


    //insertQuery.exec();
    if (!insertQuery.exec())
    {
        qDebug() << "Failed to insert data into the database!";
        qDebug() << "Error details:" << insertQuery.lastError().text();
    }

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
    ui->stackedWidget->setCurrentIndex(5);
    ui->stackedWidget->raise();
    ui->label_4->setText(btn_name);
    ui->label_15->setDisabled(false);
    ui->label_16->setDisabled(false);
    ui->comboBox_11->setDisabled(false);
    ui->comboBox_12->setDisabled(false);
    ui->comboBox_11->setCurrentIndex(0);
    ui->comboBox_10->setCurrentIndex(0);
    ui->pushButton_48->setText("");
    ui->pushButton_49->setText("");

    QSqlQuery Query;
    Query.prepare("select mode,std,nc,pc,lc,cc,pri,sec FROM tests WHERE name = :bname");
    Query.bindValue(":bname", btn_name);
    Query.exec();
    while(Query.next())
    {
        test_mode=Query.value("mode").toInt();
        cal=Query.value("std").toInt();
        nc=Query.value("nc").toInt();
        pc=Query.value("pc").toInt();
        lc=Query.value("lc").toInt();
        cc=Query.value("cc").toInt();
        pri_wave=Query.value("pri").toInt();
        sec_wave=Query.value("sec").toInt();
    }
    led_control(pri_wave);

    if(test_mode==1)
    {
        ui->label_15->setDisabled(true);
        ui->label_16->setDisabled(true);
        ui->comboBox_11->setDisabled(true);
        ui->comboBox_12->setDisabled(true);
    }
    if(test_mode==2)
    {
        Query.prepare("select std, abs1, abs2 FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        double abs1=0,abs2=0;
        while(Query.next())
        {
            cal=Query.value("std").toInt();
            abs1=Query.value("abs1").toDouble();
            abs2=Query.value("abs2").toDouble();
        }
        if(abs1==0.0 && abs2==0.0)
        {
            ui->label_15->setDisabled(true);
            ui->comboBox_11->setDisabled(true);
        }
        ui->label_15->setText("Cal.");

    }
    if(test_mode==3)
    {
        Query.prepare("select nc, pc, lc, cc, cutabs FROM tests WHERE name = :bname");
        Query.bindValue(":bname", btn_name);
        Query.exec();
        double cutabs=0;
        while(Query.next())
        {
            nc=Query.value("nc").toInt();
            pc=Query.value("pc").toInt();
            lc=Query.value("lc").toInt();
            cc=Query.value("cc").toInt();
            cutabs=Query.value("cutabs").toDouble();

        }
        if(!(cutabs>0.0))
        {
            ui->label_15->setDisabled(true);
            ui->comboBox_11->setDisabled(true);
        }
        ui->label_16->setDisabled(true);
        ui->comboBox_12->setDisabled(true);
        ui->label_15->setText("Cont.");
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
    double ncq=ui->pushButton_64->text().toDouble();
    double pcq=ui->pushButton_65->text().toDouble();
    double lcq=ui->pushButton_66->text().toDouble();
    double ccq=ui->pushButton_68->text().toDouble();
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
        QString unit;
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
        double ncq=0,pcq=0,lcq=0,ccq=0,pos=0,neg=0,grey=0,cuta=0;;
        QString cut;

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
            ncq=Query.value("ncqc").toDouble();
            pcq=Query.value("pcqc").toDouble();
            lcq=Query.value("lcqc").toDouble();
            ccq=Query.value("ccqc").toDouble();
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
        ui->pushButton_64->setText(QString::number(ncq));
        ui->pushButton_65->setText(QString::number(pcq));
        ui->pushButton_66->setText(QString::number(lcq));
        ui->pushButton_68->setText(QString::number(ccq));
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
    key->setData("Number of Blank");
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
    key->setData("Number of Blank");
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
    qDebug()<<total_cal<<total_samp<<total;
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
    key->setData("Cutoff Equation");
    key->exec();
    QString data = key->getData();
    ui->pushButton_50->setText(data);
}

void MainWindow::on_comboBox_11_currentIndexChanged(int index)
{
    if(index==1)
    {
        nc=pc=lc=cc=cal=0;
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
}

void MainWindow::on_pushButton_14_clicked()
{
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
    key->setData("Forward Steps");
    key->exec();
    QString data = key->getData();
    ui->pushButton_18->setText(data);
}

void MainWindow::on_pushButton_19_clicked()
{
    keyboard *key=new keyboard(this);
    key->setModal(true);
    key->setPage(2);
    key->setData("Backward Steps");
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
}

void MainWindow::on_toolButton_26_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
}

void MainWindow::on_toolButton_25_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_toolButton_20_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->tableWidget->setRowCount(total);
    for(int i=0;i<8;i++)
    {
        ui->tableWidget->setColumnWidth(i,65);
    }
    //    ui->tableWidget->setColumnCount(8);
    //    ui->tableWidget->setColumnWidth(0,50);
    //    ui->tableWidget->setColumnWidth(3,100);

    double len = std::ceil(double(total)/8);
    int length=int(len);
    QString wel[8]={"A","B","C","D","E","F","G","H"};
    QLabel *well,*samp, *absr, *absa;
    QPushButton *pb;
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<8;j++)
        {
            well = new QLabel();
            well->setAlignment(Qt::AlignCenter);
            well->setText(wel[j]+QString::number(i+1));

            samp = new QLabel();
            samp->setAlignment(Qt::AlignCenter);
            samp->setText(dis[j+(i*8)]);

            absr = new QLabel();
            absr->setAlignment(Qt::AlignCenter);
            absr->setText(QString::number(abs_res[j+(i*8)],'f',3));

            absa = new QLabel();
            absa->setAlignment(Qt::AlignCenter);
            absa->setText(QString::number(abs_avg[j+(i*8)],'f',3));

            pb = new QPushButton();


            ui->tableWidget->setCellWidget(j+(i*8), 0, well);//well
            ui->tableWidget->setCellWidget(j+(i*8), 1, samp);//sample
            ui->tableWidget->setCellWidget(j+(i*8), 2, absr);//absorbance
            ui->tableWidget->setCellWidget(j+(i*8), 3, absa);//average
            ui->tableWidget->setCellWidget(j+(i*8), 7, pb);//PID
        }

    }
}


void MainWindow::result_page()
{
    ui->stackedWidget->setCurrentIndex(6);
    double len = std::ceil(double(total)/8);
    int length=int(len);
    for(int i=0;i<length;i++)
        for(int k=0;k<8;k++)
            abs_res[k+(i*8)]=fin_res[i][k];

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
                                      ui->A12_2,ui->B12_2,ui->C12_2,ui->D12_2,ui->E12_2,ui->F12_2,ui->G12_2,ui->H12_2,};
    for(int i=0;i<96;i++)
    {
        samp_buttons[i]->setText("");
        samp_buttons[i]->setStyleSheet("background-color: rgb(255, 255, 255)");
        abs_avg[i]=0;
    }
    process_average();

    for(int i=0;i<total;i++)
    {
        samp_buttons[i]->setText(dis[i]+'\n'+QString::number(abs_avg[i],'f', 3));
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
