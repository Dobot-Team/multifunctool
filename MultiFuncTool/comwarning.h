#ifndef COMWARNING_H
#define COMWARNING_H

#include <QWidget>

namespace Ui
{
    class ComWarning;
}

class ComWarning : public QWidget
{
    Q_OBJECT

public:
    explicit ComWarning(QWidget *parent = 0);
    ~ComWarning();

private:
    Ui::ComWarning *ui;
};

#endif // COMWARNING_H
