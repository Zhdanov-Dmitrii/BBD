#ifndef EDD_H
#define EDD_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>

namespace Ui {
class edd;
}

class edd : public QDialog
{
    Q_OBJECT

public:
    explicit edd(QWidget *parent = 0);
    ~edd();

private slots:
    void on_Ok_clicked();
    void OkEnanled();
    void on_Close_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_content_currentRowChanged(int currentRow);

private:
    QCheckBox *CB;
    QSpinBox *SB;
    QLabel *Lbl;
    Ui::edd *ui;
    QString der;
};

#endif // EDD_H
