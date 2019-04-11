#include "podrobno.h"
#include "ui_podrobno.h"
#include "book.h"
#include "student.h"
#include <QMessageBox>

Podrobno::Podrobno(QWidget *parent, QString path) :
    QDialog(parent),
    ui(new Ui::Podrobno)
{
    ui->setupUi(this);
    Path = path;
    book Book(Path);
    Author = false; Number = false; Name = false; Data = false; Izd = false; Otdel = false; Price = false;
    Nal = false; Der =  false; Content = false;
    ui->lineAuthor->setText(Book.getAuthor());
    ui->lineName->setText(Book.getName());
    ui->lineNumber->setText(Book.getNumber());
    ui->lineData->setText(Book.getData());
    ui->lineIzd->setText(Book.getIzd());
    ui->lineOtdel->setText(Book.getOtdel());
    ui->linePrice->setText(Book.getPrice());
    QString der = Book.getDer();
    for(int i = 0; i != ui->der->count(); i++)
    {
        if(ui->der->itemText(i) == der)
        {
            ui->der->setCurrentIndex(i);
            break;
        }
    }
    if(der == "Учебный фонд")
    {
        auto b = Book.getRelate();
        ui->label_Number->setText("Класс:");
        QString str = "Количество экземпляров в бибилотеке: " + Book.getNal();
        ui->nal->insertItem(0, str);
        if(b.size() > 0)
            for(int i = 1; i <= b.size(); i++)
            {
                Student student(b[i-1]);
                ui->nal->insertItem(i,student.SNC());
                ui->nal->item(i)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
            }
    }
    else
    {
        if(Book.getNal() == "Да") ui->nal->insertItem(0,"Находится в библиотеке");
        else ui->nal->insertItem(0, Book.getNal());
    }
    auto content = Book.getContent();
    for(int i = 0; i < content.size(); i++)
    {
        ui->content->insertItem(i,content.at(i));
        ui->content->item(i)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }

    ui->content->addItem("");
    ui->content->item(ui->content->count()-1)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

    Author = true; Number = true; Name = true; Data = true; Izd = true; Otdel = true; Price = true;
    Nal = true; Der = true; Content = true;
}

Podrobno::~Podrobno()
{
    delete ui;
}

void Podrobno::on_lineAuthor_textChanged(const QString &arg1)   //Изменение Автора
{
    if(Author)
    {
        book Book(Path);
        Book.setAuthor(arg1);
    }
}

void Podrobno::on_lineName_textChanged(const QString &arg1)     //Изменение Названия
{
    if(Name)
    {
        book Book(Path);
        Book.setName(arg1);
    }
}

void Podrobno::on_lineNumber_textChanged(const QString &arg1)   //Изменение Номера
{
    if(Number)
    {
        book Book(Path);
        Book.setNumber(arg1);
    }
}

void Podrobno::on_lineIzd_textChanged(const QString &arg1)      //Изменение Издательства
{
    if(Izd)
    {
        book Book(Path);
        Book.setIzd(arg1);
    }
}

void Podrobno::on_lineOtdel_textChanged(const QString &arg1)    //Изменение Отдела
{
    if(Otdel)
    {
        book Book(Path);
        Book.setOtdel(arg1);
    }
}

void Podrobno::on_lineData_textChanged(const QString &arg1)     //Изменение Даты
{
    if(Data)
    {
          book Book(Path);
          Book.setData(arg1);
    }
}

void Podrobno::on_linePrice_textChanged(const QString &arg1)    //Изменение Цены
{
    if(Price)
    {
        book Book(Path);
        Book.setPrice(arg1);
    }
}

void Podrobno::on_content_itemChanged(QListWidgetItem *item)
{
    if(Content)
    {
        Content = false;
        QList<QString> content;
        book Book(Path);

        for(int i = 0; i < ui->content->count(); i++)
            if(!ui->content->item(i)->text().isEmpty())
                content.push_back(ui->content->item(i)->text());

        Book.setContent(content);

        if(!ui->content->item(ui->content->count()-1)->text().isEmpty())
        {
            ui->content->addItem("");
            ui->content->item(ui->content->count()-1)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
        }
        Content = true;
    }
}
