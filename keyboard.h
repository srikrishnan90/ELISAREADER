#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>

namespace Ui {
class keyboard;
}

class keyboard : public QDialog
{
    Q_OBJECT

public:
    explicit keyboard(QWidget *parent = nullptr);
    ~keyboard();
    QString getData();
    void setData(const QString &labelText, const QString &lineeditText);
    void setPage(int index);


private slots:




    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_85_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_63_clicked();

    void on_pushButton_60_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_64_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_59_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_75_clicked();

    void on_pushButton_73_clicked();

    void on_pushButton_71_clicked();

    void on_pushButton_76_clicked();

    void on_pushButton_100_clicked();

    void on_pushButton_77_clicked();

    void on_pushButton_78_clicked();

    void on_pushButton_80_clicked();

    void on_pushButton_79_clicked();

    void on_pushButton_84_clicked();

    void on_pushButton_82_clicked();

    void on_pushButton_83_clicked();

    void on_pushButton_81_clicked();

    void on_pushButton_87_clicked();

    void on_pushButton_86_clicked();

    void on_pushButton_95_clicked();

    void on_pushButton_89_clicked();

    void on_pushButton_90_clicked();

    void on_pushButton_88_clicked();

    void on_pushButton_96_clicked();

    void on_pushButton_93_clicked();

    void on_pushButton_91_clicked();

    void on_pushButton_92_clicked();

    void on_pushButton_97_clicked();

    void on_pushButton_107_clicked();

    void on_pushButton_104_clicked();

    void on_pushButton_99_clicked();

    void on_pushButton_106_clicked();

    void on_pushButton_108_clicked();

    void on_pushButton_102_clicked();

    void on_pushButton_103_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_113_clicked();

    void on_pushButton_111_clicked();

    void on_pushButton_109_clicked();

    void on_pushButton_114_clicked();

    void on_pushButton_74_clicked();

    void on_pushButton_112_clicked();

    void on_pushButton_117_clicked();

    void on_pushButton_115_clicked();

    void on_pushButton_116_clicked();

    void on_pushButton_128_clicked();

    void on_pushButton_125_clicked();

    void on_pushButton_123_clicked();

    void on_pushButton_138_clicked();

    void on_pushButton_140_clicked();

    void on_pushButton_134_clicked();

    void on_pushButton_141_clicked();

    void on_pushButton_118_clicked();

    void on_pushButton_136_clicked();

    void on_pushButton_94_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_101_clicked();

    void on_pushButton_105_clicked();

    void on_pushButton_110_clicked();

    void on_pushButton_72_clicked();

    void on_pushButton_139_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_70_clicked();

    void on_pushButton_66_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_124_clicked();

    void on_pushButton_122_clicked();

    void on_pushButton_121_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_126_clicked();

    void on_pushButton_120_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_238_clicked();

    void on_pushButton_235_clicked();

    void on_pushButton_237_clicked();

    void on_pushButton_236_clicked();

    void on_pushButton_241_clicked();

    void on_pushButton_233_clicked();

    void on_pushButton_234_clicked();

    void on_pushButton_239_clicked();

    void on_pushButton_240_clicked();

    void on_pushButton_119_clicked();

    void on_pushButton_242_clicked();

    void on_pushButton_244_clicked();

    void on_pushButton_243_clicked();

    void on_pushButton_245_clicked();

    void on_pushButton_247_clicked();

    void on_pushButton_246_clicked();

    void on_pushButton_249_clicked();

    void on_pushButton_248_clicked();

    void on_pushButton_251_clicked();

    void on_pushButton_250_clicked();

    void on_pushButton_253_clicked();

    void on_pushButton_252_clicked();

    void on_pushButton_255_clicked();

    void on_pushButton_254_clicked();

    void on_pushButton_257_clicked();

    void on_pushButton_256_clicked();

    void on_pushButton_137_clicked();

    void on_pushButton_135_clicked();

    void on_pushButton_142_clicked();

private:
    Ui::keyboard *ui;
};

#endif // KEYBOARD_H
