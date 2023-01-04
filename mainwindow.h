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

private:
    Ui::MainWindow *ui;
    paimon::huffman* huffman;
};
#endif // MAINWINDOW_H
