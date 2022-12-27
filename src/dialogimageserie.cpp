#include "dialogimageserie.h"
#include "ui_dialogimageserie.h"

#include <QDir>
#include <QFileDialog>
#include <QPushButton>
#include <QString>

DialogImageSerie::DialogImageSerie(QWidget *parent, ImgSerie s, double startZoom, unsigned startAddIT) :
    QDialog(parent),
    ui(new Ui::DialogImageSerie),
    startZoom(startZoom),
    startITC(startAddIT)
{
    ui->setupUi(this);
    this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(true);

    this->dir = s.dirPath;
    this->ui->lineEditPath->setText(dir);

    this->ui->lineEditNamePrefix->setText( s.prefix );
    this->ui->lineEditSuffix->setText( s.suffix );
    this->ui->spinBox_NameItStart->setValue( s.nameStartConst );
    this->ui->spinBoxBilderZahl->setValue( s.imgCountConst );
    this->ui->doubleSpinBoxZoomStep->setValue( s.zoomStep );
    this->ui->label_startZoom->setText(QString::number(startZoom));
    updateEndZoom();
    updateEndItCount();
}


DialogImageSerie::~DialogImageSerie()
{
    delete ui;
}

DialogImageSerie::ImgSerie DialogImageSerie::getImgSerieSettings()
{
    return ImgSerie(this->ui->doubleSpinBoxZoomStep->value(), ui->spinBoxBilderZahl->value(), dir, ui->spinBox_NameItStart->value(),
                    ui->lineEditNamePrefix->text(), ui->lineEditSuffix->text(), ui->spinBoxAddIt->value());
}

#include <cmath>

void DialogImageSerie::updateEndZoom()
{
    this->ui->label_startZoom->setText(QString::number(startZoom));
    this->ui->label_endZoom->setText(QString::number(startZoom * std::pow(ui->doubleSpinBoxZoomStep->value(), ui->spinBoxBilderZahl->value()) ) );
}

void DialogImageSerie::updateEndItCount()
{
    this->ui->label_StartIt->setText(QString::number(startITC));
    this->ui->label_EndIt->setText(QString::number(startITC + (ui->spinBoxBilderZahl->value() - 1) * ui->spinBoxAddIt->value() ) );
}

void DialogImageSerie::on_pushButtonOpenSaveDir_clicked()
{
    QString dir_ = QFileDialog::getExistingDirectory( this, tr("Ordner auswählen"), this->dir, QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks );
    if(!dir_.isEmpty() && QDir(dir_).exists()) {
        this->ui->lineEditPath->setText( this->dir = dir_ );
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(false);
    } else {
        this->ui->lineEditPath->setText( "" );
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(true);
    }
}


DialogImageSerie::ImgSerie::ImgSerie()
{
    this->zoomStep = 1.01;
    this->dirPath = "";

    //wichtig 0 == deaktiviert
    this->imgCount = 0;
    this->imgCountConst = 200;

    this->nameItStart = 0;
    this->nameStartConst = nameItStart;

    this->prefix = "";
    this->suffix = ".jpg";

}

DialogImageSerie::ImgSerie::ImgSerie(double zoom, unsigned imgCount, QString path, unsigned nameItStart, QString prefix, QString suffix , unsigned addIT)
{
    this->zoomStep = zoom;
    this->imgCount = imgCount;
    this->imgCountConst = imgCount;

    this->dirPath = path;
    this->nameItStart = nameItStart;
    this->nameStartConst = nameItStart;

    this->prefix = prefix;
    this->suffix = suffix;
    this->addIT = addIT;

}

void DialogImageSerie::on_lineEditPath_textChanged(const QString &arg1)
{
    if(QDir(arg1).exists()) {
        this->dir = arg1;
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(false);
    } else {
        this->ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setDisabled(true);
    }
}


void DialogImageSerie::on_spinBoxBilderZahl_valueChanged(int)
{
    updateEndZoom();
    updateEndItCount();

}


void DialogImageSerie::on_doubleSpinBoxZoomStep_valueChanged(double)
{
    updateEndZoom();

}


void DialogImageSerie::on_spinBoxAddIt_valueChanged(int)
{
    updateEndItCount();

}

