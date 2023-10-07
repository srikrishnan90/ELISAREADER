#include "keyboard.h"
#include "ui_keyboard.h"

keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{
    ui->setupUi(this);
}

keyboard::~keyboard()
{
    delete ui;
}


QString keyboard::getData()
{
    return(ui->lineEdit->text());
}

void keyboard::setData(const QString &labelText, const QString &lineeditText)
{
    ui->label->setText(labelText);
    ui->lineEdit->setText(lineeditText);
}

void keyboard::setPage(int index)
{
    ui->stackedWidget_2->setCurrentIndex(index);
}


void keyboard::on_pushButton_9_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_9->text());
}

void keyboard::on_pushButton_10_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_10->text());
}

void keyboard::on_pushButton_36_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_36->text());
}

void keyboard::on_pushButton_35_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_35->text());
}

void keyboard::on_pushButton_40_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_40->text());
}

void keyboard::on_pushButton_38_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_38->text());
}

void keyboard::on_pushButton_39_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_39->text());
}

void keyboard::on_pushButton_37_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_37->text());
}

void keyboard::on_pushButton_43_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_43->text());
}

void keyboard::on_pushButton_42_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_42->text());
}

void keyboard::on_pushButton_85_clicked()
{
    ui->lineEdit->backspace();
}

void keyboard::on_pushButton_41_clicked()
{
    ui->lineEdit->backspace();
}

void keyboard::on_pushButton_51_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_51->text());
}

void keyboard::on_pushButton_45_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_45->text());
}

void keyboard::on_pushButton_46_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_46->text());
}

void keyboard::on_pushButton_44_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_44->text());
}

void keyboard::on_pushButton_52_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_52->text());
}

void keyboard::on_pushButton_49_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_49->text());
}

void keyboard::on_pushButton_47_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_47->text());
}

void keyboard::on_pushButton_48_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_48->text());
}

void keyboard::on_pushButton_53_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_53->text());
}

void keyboard::on_pushButton_63_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_63->text());
}

void keyboard::on_pushButton_60_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_60->text());
}

void keyboard::on_pushButton_55_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_55->text());
}

void keyboard::on_pushButton_62_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_62->text());
}

void keyboard::on_pushButton_64_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_64->text());
}

void keyboard::on_pushButton_58_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_58->text());
}

void keyboard::on_pushButton_59_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_59->text());
}

void keyboard::on_pushButton_54_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_54->text());
}

void keyboard::on_pushButton_56_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_56->text());
}

void keyboard::on_pushButton_75_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_75->text());
}

void keyboard::on_pushButton_73_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_73->text());
}

void keyboard::on_pushButton_71_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_71->text());
}

void keyboard::on_pushButton_76_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_76->text());
}

void keyboard::on_pushButton_100_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_100->text());
}

void keyboard::on_pushButton_77_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_77->text());
}

void keyboard::on_pushButton_78_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_78->text());
}

void keyboard::on_pushButton_80_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_80->text());
}

void keyboard::on_pushButton_79_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_79->text());
}

void keyboard::on_pushButton_84_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_84->text());
}

void keyboard::on_pushButton_82_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_82->text());
}

void keyboard::on_pushButton_83_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_83->text());
}

void keyboard::on_pushButton_81_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_81->text());
}

void keyboard::on_pushButton_87_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_87->text());
}

void keyboard::on_pushButton_86_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_86->text());
}

void keyboard::on_pushButton_95_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_95->text());
}

void keyboard::on_pushButton_89_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_89->text());
}

void keyboard::on_pushButton_90_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_90->text());
}

void keyboard::on_pushButton_88_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_88->text());
}

void keyboard::on_pushButton_96_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_96->text());
}

void keyboard::on_pushButton_93_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_93->text());
}

void keyboard::on_pushButton_91_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_91->text());
}

void keyboard::on_pushButton_92_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_92->text());
}

void keyboard::on_pushButton_97_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_97->text());
}

void keyboard::on_pushButton_107_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_107->text());
}

void keyboard::on_pushButton_104_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_104->text());
}

void keyboard::on_pushButton_99_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_99->text());
}

void keyboard::on_pushButton_106_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_106->text());
}

void keyboard::on_pushButton_108_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_108->text());
}

void keyboard::on_pushButton_102_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_102->text());
}

void keyboard::on_pushButton_103_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_103->text());
}

void keyboard::on_pushButton_98_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_98->text());
}

void keyboard::on_pushButton_113_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_113->text());
}

void keyboard::on_pushButton_111_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_111->text());
}

void keyboard::on_pushButton_109_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_109->text());
}

void keyboard::on_pushButton_114_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_114->text());
}

void keyboard::on_pushButton_74_clicked()
{
    ui->lineEdit->setText(" ");
}

void keyboard::on_pushButton_112_clicked()
{
    ui->lineEdit->setText(" ");

}

void keyboard::on_pushButton_117_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_117->text());
}

void keyboard::on_pushButton_115_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_115->text());
}

void keyboard::on_pushButton_116_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_116->text());
}

void keyboard::on_pushButton_128_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_128->text());
}

void keyboard::on_pushButton_125_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_125->text());
}

void keyboard::on_pushButton_123_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_123->text());
}

void keyboard::on_pushButton_138_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_138->text());
}

void keyboard::on_pushButton_140_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_140->text());
}

void keyboard::on_pushButton_134_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_134->text());
}

void keyboard::on_pushButton_141_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_141->text());
}

void keyboard::on_pushButton_118_clicked()
{
    ui->lineEdit->backspace();
}

void keyboard::on_pushButton_136_clicked()
{
    getData();
    this->accept();
}


void keyboard::on_pushButton_94_clicked()
{
    getData();
    this->accept();
}

void keyboard::on_pushButton_50_clicked()
{
    getData();
    this->accept();
}

void keyboard::on_pushButton_57_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}

void keyboard::on_pushButton_61_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);

}

void keyboard::on_pushButton_101_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);

}

void keyboard::on_pushButton_105_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void keyboard::on_pushButton_110_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);

}

void keyboard::on_pushButton_72_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void keyboard::on_pushButton_139_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void keyboard::on_pushButton_12_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_12->text());
}

void keyboard::on_pushButton_11_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_11->text());

}

void keyboard::on_pushButton_70_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_70->text());

}

void keyboard::on_pushButton_66_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_66->text());

}

void keyboard::on_pushButton_69_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_69->text());

}

void keyboard::on_pushButton_68_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_68->text());

}

void keyboard::on_pushButton_124_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_124->text());

}

void keyboard::on_pushButton_122_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"&");

}

void keyboard::on_pushButton_121_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_121->text());

}

void keyboard::on_pushButton_67_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_67->text());

}

void keyboard::on_pushButton_126_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_126->text());

}

void keyboard::on_pushButton_120_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_120->text());

}

void keyboard::on_pushButton_18_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_18->text());

}

void keyboard::on_pushButton_17_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_17->text());

}

void keyboard::on_pushButton_238_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_238->text());

}

void keyboard::on_pushButton_235_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_235->text());

}

void keyboard::on_pushButton_237_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_237->text());

}

void keyboard::on_pushButton_236_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_236->text());

}

void keyboard::on_pushButton_241_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_241->text());

}

void keyboard::on_pushButton_233_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_233->text());

}

void keyboard::on_pushButton_234_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_234->text());

}

void keyboard::on_pushButton_239_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_239->text());

}

void keyboard::on_pushButton_240_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_240->text());

}

void keyboard::on_pushButton_119_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_119->text());

}

void keyboard::on_pushButton_242_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_242->text());
}

void keyboard::on_pushButton_244_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_244->text());
}

void keyboard::on_pushButton_243_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_243->text());
}

void keyboard::on_pushButton_245_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_245->text());
}

void keyboard::on_pushButton_247_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_247->text());
}

void keyboard::on_pushButton_246_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_246->text());
}

void keyboard::on_pushButton_249_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_249->text());
}

void keyboard::on_pushButton_248_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_248->text());
}

void keyboard::on_pushButton_251_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_251->text());
}

void keyboard::on_pushButton_250_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_250->text());
}

void keyboard::on_pushButton_253_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_253->text());
}

void keyboard::on_pushButton_252_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_252->text());
}

void keyboard::on_pushButton_255_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_255->text());
}

void keyboard::on_pushButton_254_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_254->text());
}

void keyboard::on_pushButton_257_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_257->text());
}

void keyboard::on_pushButton_256_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_256->text());
}

void keyboard::on_pushButton_137_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_137->text());
}

void keyboard::on_pushButton_135_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_135->text());
}

void keyboard::on_pushButton_142_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+ui->pushButton_142->text());
}
