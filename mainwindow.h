#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QFile>
#include <QFileDialog>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    int sum_Byts(QByteArray array, int begin, int length);
    int byte_to_int(char);
    void edit_data(QByteArray &array, int data, int begin, int length);
private slots:
    void read_slot();
    void convert_slot();
private:
    Ui::MainWindow *ui;
    QByteArray arr;
    QString path;
    int inf_end_header;
    double inf_wight;
    double inf_height;
    double inf_ratio;
    int inf_begin_arr;
};
#endif // MAINWINDOW_H
