#include "addstudent.h"
#include "ui_eddstudent.h"
#include "student.h"
#include "QMessageBox"

eddStudent::eddStudent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eddStudent)
{
    ui->setupUi(this);
    ui->edd->setEnabled(false);
    QRegExp s("[А-Я]{1}[а-яА-я]{1,50}");
    QRegExp c("[а-яА-я0-9]{1,50}");
    ui->Surname_2->setValidator(new QRegExpValidator(s,this));
    ui->Name_2->setValidator(new QRegExpValidator(s,this));
    ui->Class_2->setValidator(new QRegExpValidator(c,this));
    connect(ui->Surname_2,SIGNAL(textChanged(QString)),this,SLOT(OK()));
    connect(ui->Name_2,SIGNAL(textChanged(QString)),this,SLOT(OK()));
    connect(ui->Class_2,SIGNAL(textChanged(QString)),this,SLOT(OK()));
}

eddStudent::~eddStudent()
{
    delete ui;
}

void eddStudent::OK()
{
    ui->edd->setEnabled(ui->Surname_2->hasAcceptableInput() &&
                        ui->Name_2->hasAcceptableInput() &&
                        ui->Class_2->hasAcceptableInput());
}

void eddStudent::on_edd_clicked()
{
    QString Surname = ui->Surname_2->text();
    QString Name = ui->Name_2->text();
    QString Class = ui->Class_2->text();
    QString Data = ui->Data_2->text();
    QString Phone = ui->Phone_2->text();
    QString Address = ui->Address_2->text();

    try
    {
        Student student(Surname, Name, Class, Data, Phone, Address);
        student.addStudent();
    }
    catch(QString error)
    {
        QMessageBox::critical(this, "Ошибка!!!",error);
    }

    QApplication::beep();
}
