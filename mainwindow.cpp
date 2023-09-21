#include "mainwindow.h"
#include "ui_mainwindow.h"

static int test_mode=0;
static int test_entry=0;
static QString btn_name="";
static int blank=0,cal=0,dup_cal=1,nc=0,pc=0,cc=0,lc=0,samp=0,dup_samp=1,total=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainLayout=new QGridLayout();
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    ui->scrollAreaWidgetContents->setLayout(mainLayout);
    QSqlDatabase sqdb = QSqlDatabase::addDatabase("QSQLITE");
    sqdb.setDatabaseName("/home/pi/Desktop/er.db");
    if(!sqdb.open())
    {
        qDebug() << "Can't Connect to DB !";
    }
    else
    {
        qDebug() << "Connected Successfully to DB !";
    }
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
    ui->stackedWidget->setCurrentIndex(5);
    ui->stackedWidget->raise();
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

    QSqlQuery insertQuery;
    insertQuery.prepare("insert into tests(name,mode,pri,sec,threshold,nc,pc,lc,cc,ncqc,pcqc,lcqc,ccqc,cutoff,pos,neg,grey) values(:name,:mode,:pri,:sec,:thresh,:nc,:pc,:lc,:cc,:ncq,:pcq,:lcq,:ccq,:cut,:pos,:neg,:grey)");
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
    QPushButton* absb[] = { ui->a1,ui->a2,ui->a3,ui->a4,ui->a5,ui->a6,ui->a7,ui->a8,ui->a9,ui->a10};
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
        double ncq=0,pcq=0,lcq=0,ccq=0,pos=0,neg=0,grey=0;
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
    }

}

void MainWindow::on_toolButton_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->lower();
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
    total=blank+nc+pc+lc+cc+(samp*dup_samp)+(cal*dup_cal);
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
    QString dis[96];
    for(int i=0;i<96;i++)
    {
        samp_buttons[i]->setText("");
    }
    for(int i=0,j=1;i<blank;i++,j++)
    {
        dis[i]='B'+QString::number(j);

    }
    for(int i=blank,j=1;i<total;i++,j++)
    {
        if(dup_samp==1)
        {
            dis[i]='S'+QString::number(j);
        }
        else
        {
            dis[i]=dis[i+1]='s'+QString::number(j);
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

    }
}




