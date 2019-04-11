#ifndef PODROBNO_H
#define PODROBNO_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class Podrobno;
}

class Podrobno : public QDialog
{
    Q_OBJECT

public:
    explicit Podrobno(QWidget *parent = 0, QString path = "");
    ~Podrobno();

private slots:
    void on_lineAuthor_textChanged(const QString &arg1);

    void on_lineName_textChanged(const QString &arg1);

    void on_lineNumber_textChanged(const QString &arg1);

    void on_lineIzd_textChanged(const QString &arg1);

    void on_lineOtdel_textChanged(const QString &arg1);

    void on_lineData_textChanged(const QString &arg1);

    void on_linePrice_textChanged(const QString &arg1);

    void on_content_itemChanged(QListWidgetItem *item);

private:
    Ui::Podrobno *ui;

    bool Author, Name, Number, Data, Izd, Otdel, Price, Nal, Der, Content;
    QString Path;
};

#endif // PODROBNO_H
