#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->selectImageBtn, &QPushButton::clicked, this, &MainWindow::sliceImage);
}


void MainWindow::sliceImage()
{
    // Открываем файл с картинкой
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    QImage m_image;

    m_image.load(fileName);
    qDebug() << fileName;

    // Создаём вектор кусков картинки
    QVector<QPixmap> pieces;

    // Задаём размеры куска
    const int piece_width = m_image.width() / ui->rowsSpinBox->value();
    const int piece_height = m_image.height() / ui->columnsSpinBox->value();

    // В цикле генерируем куски
    for(int i = 0; i < ui->rowsSpinBox->value(); i++)
    {
        for (int j = 0; j < m_image.width(); j += piece_width)
        {
            pieces.push_back(QPixmap::fromImage(QImage(createSubImage
                                                       (&m_image, QRect(QPoint(j, i * piece_height), QPoint(j + piece_width, (i + 1) * piece_height))))));
        }
    }

    // Cохраняем куски в файлы
    for(int i = 0; i < pieces.size(); ++i)
    {
        QString filename = QString::number(i) + ".png";
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        pieces[i].save(&file, "PNG");
    }

    QMessageBox mb;
    mb.setText("Успешно!");
    mb.show();
}

QImage MainWindow::createSubImage(QImage *image, const QRect &rect)
{
    // https://stackoverflow.com/questions/12681554/dividing-qimage-to-smaller-pieces

    size_t offset = rect.x() * image->depth() / 8
            + rect.y() * image->bytesPerLine();
    return QImage(image->bits() + offset, rect.width(), rect.height(),
                  image->bytesPerLine(), image->format());
}


MainWindow::~MainWindow()
{
    delete ui;
}



