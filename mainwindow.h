#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "keyboard.h"
#include <QGridLayout>
#include <QScroller>
#include <pi2c.h>
#include <ADS1263.h>
#include <DEV_Config.h>
#include "regressionline.h"
#include <QJSEngine>
#include <math.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_toolButton_5_clicked();

    void on_toolButton_clicked();

    void on_toolButton_9_clicked();

    void on_toolButton_10_clicked();

    void on_toolButton_11_clicked();

    void on_toolButton_17_clicked();

    void on_toolButton_13_clicked();

    void on_toolButton_15_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_c1_clicked();

    void on_c2_clicked();

    void on_c3_clicked();

    void on_c4_clicked();

    void on_c5_clicked();

    void on_c6_clicked();

    void on_c7_clicked();

    void on_c8_clicked();

    void on_c9_clicked();

    void on_c10_clicked();

    void on_a1_clicked();

    void on_a2_clicked();

    void on_a3_clicked();

    void on_a4_clicked();

    void on_a5_clicked();

    void on_a6_clicked();

    void on_a7_clicked();

    void on_a8_clicked();

    void on_a9_clicked();

    void on_a10_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_64_clicked();

    void on_pushButton_65_clicked();

    void on_pushButton_66_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_47_clicked();

    void on_toolButton_16_clicked();

    void on_toolButton_12_clicked();


    void on_toolButton_8_clicked();

    void on_toolButton_14_clicked();

    int abs_check();
    int multi_check();
    int cut_check();
    void test_menu();
    void sig_button();
    void accle(ulong,ulong);


    void on_toolButton_7_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_19_clicked();

    void on_pushButton_48_clicked();

    void on_comboBox_10_currentIndexChanged(int index);
    void update_sample_page();

    void on_comboBox_12_currentIndexChanged(int index);

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    void on_comboBox_11_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_toolButton_4_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_toolButton_18_clicked();

    void led_control(int);

    void mot_forward(ulong);
    void mot_backward(ulong);

    void on_toolButton_21_clicked();

    void on_toolButton_26_clicked();

    void on_toolButton_25_clicked();

    void on_toolButton_20_clicked();
    void result_page();
    void process_average();
    void process_result_multistandard();
    void process_result_cutoff();
    QString calculate_regression(double, int);

    void on_toolButton_30_clicked();

    void on_toolButton_31_clicked();

    void on_toolButton_32_clicked();

    void on_toolButton_33_clicked();

private:
    Ui::MainWindow *ui;
    QGridLayout* mainLayout;
};

#endif // MAINWINDOW_H
