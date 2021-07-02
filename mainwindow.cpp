#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "llong.h"
std::string poliz_str(std::string s);
bool is_correct(std::string s);
std::string computing(string poliz);
void reset();
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui->Answer->setFixedWidth(30);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_Expresson_textChanged()
{
    //ui->Answer->setFixedHeight(30);
    //this->ui->tableWidget->setRowCount(this->ui->tableWidget->rowCount() + 1);
}

void MainWindow::on_CalculateButton_clicked()
{
    ui->Answer->setFixedHeight(25);
    ui->Poliz->setFixedHeight(25);
    std:: string expr = this->ui->Expression->toPlainText().trimmed().toStdString();
    reset();
    this->ui->Poliz->setText("");
    try {
          is_correct(expr);
          std:: string ans = poliz_str(expr);
          QString qstr_poliz = QString::fromStdString(ans);
          this->ui->Poliz->setText(qstr_poliz);
          QString qstr = QString::fromStdString(computing(ans));
          this->ui->Answer->setText(qstr);
          //this->ui->Poliz->setText(qstr_poliz);

    }
    catch (const char *str) {
        this->ui->Answer->setText(QString::fromUtf8(str));
    }
    catch(char c){
        QString ans = "введён неопределенный символ " + QString(QChar::fromLatin1(c));
        this->ui->Answer->setText(ans);
    }
}
