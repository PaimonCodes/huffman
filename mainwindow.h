#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "huffman.h"
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browse_clicked();

    void on_compress_clicked();

    void on_line_path_textEdited(const QString &arg1);

    void on_pushButton_showShowInFolder_clicked();

    void on_decompress_clicked();

    void on_pushButton_showShowInFolder_2_clicked();

private:
    QString get_file_size(const QString& file_name);

    void check_directory_exists();

    void open_path(const QString& file_path);

    void write_compressed_binary();

    void write_uncompressed_data();

    void clear_decompressed_data();

    Ui::MainWindow *ui;

    paimon::huffman huffman;

    QString file_input;
};
#endif // MAINWINDOW_H
