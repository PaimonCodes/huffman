#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->compress->setEnabled(false);
    ui->decompress->setEnabled(false);
    ui->pushButton_showShowInFolder->setEnabled(false);
    ui->pushButton_showShowInFolder_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browse_clicked()
{
    // Open home directory if on UNIX or C:// on Windows, filter txt files
    QString filter = "Text File(*.txt)";
    QString file_name = QFileDialog::getOpenFileName(this, "Open an ASCII file", "C://", filter);

    // Write file name on line_path
    ui->line_path->setText(file_name);

    // Get original file size
    int size = 0;
    QFile file(file_name);
    if (file.open(QIODevice::ReadOnly))
    {
        size = file.size();  //when file does open.
        file.close();
    }
    QLocale locale = this->locale();
    QString valueText = locale.formattedDataSize(size);

    ui->label_original_size->setText("Original Size: " + valueText);
    ui->compress->setEnabled(true);
}

void MainWindow::on_line_path_textEdited(const QString &arg1)
{
    QFile file(arg1);
    if (file.open(QIODevice::ReadOnly))
    {
        ui->compress->setEnabled(true);
    }
    else
    {
        ui->compress->setEnabled(false);
    }
}

void MainWindow::on_compress_clicked()
{
    huffman = new paimon::huffman();

    std::string file_path = (ui->line_path->text()).toStdString();

    QDir dir("data/");
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
    huffman->compress(file_path);
}
