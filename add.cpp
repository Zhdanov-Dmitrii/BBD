#include "add.h"
#include "ui_edd.h"
#include "book.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <ctime>


edd::edd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edd)
{
    ui->setupUi(this);
    ui->Ok->setEnabled(false);
    CB = new QCheckBox("В наличии");
    der = ui->comboBox->currentText();
    ui->verticalLayout_5->addWidget(CB);
    ui->content->addItem("");
    ui->content->item(ui->content->count()-1)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

    QRegExp AuRegExp("[А-Я]{1}[а-яА-я]{1,50}[ ]{1,1}[А-Я]{1,1}[.]{1,1}[ ]{1,1}[А-Я]{1,1}[.]{1,1}");
    QRegExp NaRegExp("[0-9а-яА-яA-Za-z]{1,50}[0-9а-яА-яA-Za-z ]{1,50}");
    QRegExp NuRegExp("[0-9]{1,5}");

    ui->author->setValidator(new QRegExpValidator(AuRegExp,this));
    ui->name->setValidator(new QRegExpValidator(NaRegExp,this));
    ui->number->setValidator(new QRegExpValidator(NuRegExp,this));
    connect(ui->author, SIGNAL(textChanged(QString)),this,SLOT(OkEnanled()));
    connect(ui->name, SIGNAL(textChanged(QString)),this,SLOT(OkEnanled()));
    connect(ui->number, SIGNAL(textChanged(QString)),this,SLOT(OkEnanled()));
}

edd::~edd()
{
    delete ui;
}

void edd::OkEnanled()
{
    ui->Ok->setEnabled(ui->author->hasAcceptableInput() && ui->name->hasAcceptableInput() && ui->number->hasAcceptableInput());
}

void edd::on_Ok_clicked()
{
    QString author  = ui->author->text();                     //автор
    QString name    = ui->name->text();                       //название
    QString number  = ui->number->text();                     //номер
    QString otdel   = ui->otdel->text();                      //отдел
    QString izd     = ui->izdatelstvo->text();                //Издательство
    QString data    = ui->data->text();                       //дата издания
    QString price   = ui->price->text();                      //цена
    QList<QString> content;                                   //содержание

    for(int i = 0; i < ui->content->count(); i++)
        if(!ui->content->item(i)->text().isEmpty())
            content.push_back(ui->content->item(i)->text());


    QString nal;
    if(der != "Учебный фонд")
    {
        nal = "Да";
        if (!CB->isChecked())
            nal = "Нет";
    }
    else
    {
        nal = SB->text();
    }


    try
    {
        book Book(name, author,number,nal,otdel,izd,data,price,der,content);
        Book.addBook();
    }
    catch(QString error)
    {
        QMessageBox::critical(this, "Ошибка", error);
    }
    QApplication::beep();
}

void edd::on_Close_clicked()
{
    close();
}

void edd::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Учебный фонд")
    {
        ui->publishing->setText("Класс:");
        delete CB;
        SB = new QSpinBox(this);
        Lbl = new QLabel(this);
        Lbl->setText("В наличии: ");
        ui->verticalLayout_5->addWidget(Lbl);
        ui->verticalLayout_5->addWidget(SB);
        der = ui->comboBox->currentText();
    }
    else
    {
        if(der == "Учебный фонд")
        {
            ui->publishing->setText("Номер:");
            delete SB;
            delete Lbl;
            CB = new QCheckBox(this);
            CB->setText("В наличии: ");
            ui->verticalLayout_5->addWidget(CB);
            der = ui->comboBox->currentText();
        }
        else
        {
            der = ui->comboBox->currentText();
        }
    }
}

void edd::on_content_currentRowChanged(int currentRow)
{
    if(currentRow == ui->content->count()-1)
    {
        ui->content->addItem("");
        ui->content->item(ui->content->count()-1)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }
}
