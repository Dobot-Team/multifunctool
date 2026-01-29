#ifndef COMWARN_H
#define COMWARN_H

#include <QDialog>

namespace Ui
{
    class ComWarn;
}

class ComWarn : public QDialog
{
    Q_OBJECT

public:
    explicit ComWarn(QWidget *parent = 0);
    ~ComWarn();
public slots:
    void ErrShow(QString err);

private:
    Ui::ComWarn *ui;
};

#endif // COMWARN_H
