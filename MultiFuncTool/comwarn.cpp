#include "comwarn.h"
#include "ui_comwarn.h"

ComWarn::ComWarn(QWidget *parent) : QDialog(parent),
                                    ui(new Ui::ComWarn)
{
    ui->setupUi(this);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

ComWarn::~ComWarn()
{
    delete ui;
}

/*************************************************************
** Function name:       ComWarn::ErrShow
** Descriptions:        错误显示
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void ComWarn::ErrShow(QString err)
{
    this->setWindowTitle("Warning");
    ui->InfoButton->setText("关闭");
    ui->Info->setText(err);
    //    this->show();
}
