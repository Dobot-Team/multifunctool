#include "comwarning.h"
#include "ui_comwarning.h"

ComWarning::ComWarning(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::ComWarning)
{
    ui->setupUi(this);
}

ComWarning::~ComWarning()
{
    delete ui;
}
