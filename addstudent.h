#ifndef EDDSTUDENT_H
#define EDDSTUDENT_H

#include <QDialog>

namespace Ui {
class eddStudent;
}

class eddStudent : public QDialog
{
    Q_OBJECT

public:
    explicit eddStudent(QWidget *parent = 0);
    ~eddStudent();

private slots:
    void on_edd_clicked();
    void OK();

private:
    Ui::eddStudent *ui;
};

#endif // EDDSTUDENT_H
