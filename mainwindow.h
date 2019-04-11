#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_edd_clicked();

    void on_search_clicked();

    void on_podrobno_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_eddStudent_clicked();

    void on_searchStudent_clicked();

    void on_dat_clicked();

    void on_zabrat_clicked();

    void on_cleanStudent_clicked();

    void on_podrobno_2_clicked();

    void on_tableStudent_cellChanged(int row, int column);

    void on_der_currentIndexChanged(const QString &arg1);

    void on_options_2_clicked();

    void on_clean_2_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    bool change, changeStudent;
    QString derect;
signals:
    void ch();
};

#endif // MAINWINDOW_H
