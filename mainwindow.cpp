#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLocale>
#include <QDesktopServices>

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
    file_input = QFileDialog::getOpenFileName(this, "Open an ASCII file", "C://", filter);

    // Write file name on line_path
    ui->line_path->setText(file_input);

    // Enable compress button
    ui->compress->setEnabled(true);
}

QString MainWindow::get_file_size(const QString& file_name)
{
    int size = 0;
    QFile file(file_name);
    if (file.open(QIODevice::ReadOnly))
    {
        size = file.size();  //when file does open.
        file.close();
    }
    QLocale locale = this->locale();
    QString value_text = locale.formattedDataSize(size);

    return value_text;
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
    std::string file_path = (ui->line_path->text()).toStdString();

    check_directory_exists();

    huffman = paimon::huffman::compress(file_path);

    ui->lookup_table_display->setPlainText(QString::fromStdString((huffman.get_lookup_table()).str()));

    // Get original file size
    ui->label_original_size->setText("Original Size: " + get_file_size(file_input));

    write_compressed_binary();
    clear_decompressed_data();

    ui->label_compressed_size->setText("Compressed Size: " + get_file_size(QDir::currentPath() + "/data/data.bin"));
    ui->pushButton_showShowInFolder->setEnabled(true);
    ui->decompress->setEnabled(true);
}

void MainWindow::check_directory_exists()
{
    QDir dir("data/");
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
}

void MainWindow::on_pushButton_showShowInFolder_clicked()
{
    if (QFile::exists(QDir::currentPath() + "/data/data.bin"))
    {
        QDesktopServices::openUrl( QUrl::fromLocalFile(QDir::currentPath() + "/data") );
    }
    else
    {
        ui->pushButton_showShowInFolder->setEnabled(false);
    }
}

void MainWindow::on_decompress_clicked()
{
    if (QFile::exists(QDir::currentPath() + "/data/data.bin"))
    {
        QDir dir("data/");
        if (!dir.exists())
        {
            dir.mkpath(".");
        }
        huffman.decompress_file((QDir::currentPath() + "/data/data.bin").toStdString());
        write_uncompressed_data();
        ui->label_decompressed_size->setText("Deompressed Size: " + get_file_size(QDir::currentPath() + "/data/uncompressed.txt"));
        ui->pushButton_showShowInFolder_2->setEnabled(true);
    }
    else
    {
        ui->decompress->setEnabled(false);
    }
}

void MainWindow::on_pushButton_showShowInFolder_2_clicked()
{
    if (QFile::exists(QDir::currentPath() + "/data/uncompressed.txt"))
    {
        QDesktopServices::openUrl( QUrl::fromLocalFile(QDir::currentPath() + "/data") );
    }
    else
    {
        ui->pushButton_showShowInFolder_2->setEnabled(false);
    }
}

void MainWindow::write_compressed_binary()
{
    std::stringstream output;
    std::ifstream input((QDir::currentPath() + "/data/data.bin").toStdString());

    // Record binary stream
    std::vector<unsigned char> stream(std::istreambuf_iterator<char>(input), {});

    // Parse stream into bytes and store in bitset
    // Get bitset string representation and print
    for (std::size_t i = 0; i < stream.size(); i++)
    {
        std::bitset<8> bits(stream[i]);
        std::string byte = bits.to_string();
        output << byte << " ";
    }

    // Append newline
    output << std::endl;

    ui->compressed_stream->setPlainText(QString::fromStdString((output.str())));
}

void MainWindow::write_uncompressed_data()
{
    std::stringstream output;
    char key;
    std::ifstream input((QDir::currentPath() + "/data/uncompressed.txt").toStdString());
    while (input >> std::noskipws >> key) {
        output << key;
    }
    ui->decompressed_stream->setPlainText(QString::fromStdString((output.str())));
}

void MainWindow::clear_decompressed_data()
{
    ui->decompressed_stream->clear();
    if (QFile::exists(QDir::currentPath() + "/data/uncompressed.txt"))
    {
        QFile::remove(QDir::currentPath() + "/data/uncompressed.txt");
    }
    ui->pushButton_showShowInFolder_2->setEnabled(false);
}
