#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(convert_slot()));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(read_slot()));
}

void MainWindow::edit_data(QByteArray &array, int data, int begin, int length){
    int ff;
    for(int i = 0, shift = 0; i < length; i++, shift += 8){
        ff = data;
        ff = ff >> shift;
        ff = ff & 0xff;
        array[begin + i] = (char)ff;
    }
}

int MainWindow::byte_to_int(char byte)
{
    return QByteArray::fromRawData(&byte, 1).toHex().toInt(nullptr, 16);
}

int MainWindow::sum_Byts(QByteArray array, int begin, int length)
{
    int rez = 0;
    for(int i = begin, shift = 0; i < begin + length; i++, shift += 8){
        rez += QByteArray::fromRawData(&array[i], 1).toHex().toInt(nullptr, 16) << shift;
    }
    return rez;
}

void MainWindow::read_slot()
{
    path = QFileDialog::getOpenFileName();
    ui->label->setPixmap(QPixmap(path).scaled(400,
                                              400,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    arr = file.readAll();
    file.close();
    inf_end_header = 14 + sum_Byts(arr, 14, 4);
    inf_wight = sum_Byts(arr, 18, 4);
    inf_height = sum_Byts(arr, 22, 4);
    inf_begin_arr = inf_end_header + 256 * 4;
}

void MainWindow::convert_slot()
{
    int s = 0;
    inf_ratio = ui->comboBox->currentText().toDouble();
    for(int i = 0; i < 1024; i+=4){
        m.setColor(i / 4, qRgb(QByteArray::fromRawData(&arr[inf_end_header + i], 1).toHex().toInt(nullptr, 16),
                   QByteArray::fromRawData(&arr[inf_end_header + i + 1], 1).toHex().toInt(nullptr, 16),
                QByteArray::fromRawData(&arr[inf_end_header + i + 2], 1).toHex().toInt(nullptr, 16)));
    }
    QFile file("C:\\Qt\\Project\\GRI_Lab5\\result.bmp");
    file.open(QIODevice::WriteOnly);
    QByteArray arr1;
    for(int i = 0; i < inf_begin_arr; i++){
        arr1.push_back(arr[i]);
    }
    if(inf_ratio < 1){
        int k = (qSqrt(inf_ratio) * 10);
        int w = inf_wight / 10 * k;
        int h = inf_height / 10 * k;
        while(w % 4){
            w++;
        }
        while(h % 4){
            h++;
        }
        edit_data(arr1, w, 18, 4);
        edit_data(arr1, h, 22, 4);
        qDebug() << inf_wight << k << "/ 10 =" << w;
        qDebug() << inf_height << k << "/ 10 =" << h;
        for(int i = 0; i < inf_begin_arr; i++){
            file.putChar(arr1[i]);
        }
        for(int i = 0, index = inf_begin_arr, c = 0, g = 0; i < inf_height; i++){
            if(g < h){
                if(c == 9){
                    c = 0;
                }
                if(c < k){
                    for(int j = 0, c1 = 0, g1 = 0; j < inf_wight; j++){
                        if(g1 < w){
                            if(c1 == 9){
                                c1 = 0;
                            }
                            if(c1 < k){
                                file.putChar(arr[index]);
                                g1++;
                            }
                            c1++;
                        }
                        index++;
                    }
                    g++;
                }else{
                    index += inf_wight;
                }
                c++;
            }
        }
        s = 400 * ((double)k / 10.0);
    }else{
        edit_data(arr1, inf_wight * inf_ratio, 18, 4);
        edit_data(arr1, inf_height * inf_ratio, 22, 4);
        for(int i = 0; i < inf_begin_arr; i++){
            file.putChar(arr1[i]);
        }
        for(int i = 0; i < inf_height; i++){
            for(int g = 0; g < inf_ratio; g++){
                for(int j = 0; j < inf_wight; j++){
                    for(int g1 = 0; g1 < inf_ratio; g1++){
                        file.putChar(arr[inf_begin_arr + inf_wight * i + j]);
                    }
                }
            }
        }
        s = 400 * qSqrt(inf_ratio);
    }
    file.close();
    ui->label_2->setPixmap(QPixmap(path).scaled(s,
                                                s,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
}
MainWindow::~MainWindow()
{
    delete ui;
}

