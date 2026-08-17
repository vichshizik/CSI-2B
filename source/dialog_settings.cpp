#include "dialog_settings.h"
#include "ui_dialog_settings.h"

Dialog_settings::Dialog_settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_settings)
{
    ui->setupUi(this);
    ui->DecimalPoint_Label->setToolTip("How many numbers will be displayed\n"
                                       "after the decimal point");
    ui->Potential_Rmin_Label->setToolTip("The lower limit of the displaying potential");
    ui->Potential_Rmax_Label->setToolTip("The upper limit of the displaying potential");
    ui->Potential_Step_Label->setToolTip("The step between points of the displaying potential");
    ui->Switch_EnergyLimit->setToolTip("The energy which determines the upper limit of bound states energy");
    ui->EnergyLimit_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: gray; color: black;}");
}

QLineEdit* Dialog_settings::get_Visual_R_min_Box()
{
    return ui->Visual_R_min_Box;
}

QLineEdit* Dialog_settings::get_Visual_R_max_Box()
{
    return ui->Visual_R_max_Box;
}

QLineEdit* Dialog_settings::get_Visual_Step_Box()
{
    return ui->Visual_Step_Box;
}

QLineEdit* Dialog_settings::get_Potential_EnergyLimit_Box()
{
    return ui->EnergyLimit_Box;
}

int Dialog_settings::get_Decimal_point_Box()
{
    return ui->Decimal_point_Box->value();
}

bool Dialog_settings::get_Switch_EnergyLimit()
{
    return ui->Switch_EnergyLimit->isChecked();
}

void Dialog_settings::set_Visual_R_min_Box(double Visual_R_min_d)
{
    ui->Visual_R_min_Box->setText(QString::number(Visual_R_min_d));
}

void Dialog_settings::set_Visual_R_max_Box(double Visual_R_max_d)
{
    ui->Visual_R_max_Box->setText(QString::number(Visual_R_max_d));
}

void Dialog_settings::set_Visual_Step_Box(double Visual_Step_d)
{
    ui->Visual_Step_Box->setText(QString::number(Visual_Step_d));
}

void Dialog_settings::set_Decimal_point_Box(int Decimal_points_numbers)
{
    ui->Decimal_point_Box->setValue(Decimal_points_numbers);
}

void Dialog_settings::set_Potential_EnergyLimit_Box(double EnergyLimit_d)
{
    ui->EnergyLimit_Box->setText(QString::number(EnergyLimit_d));
}

void Dialog_settings::set_Switch_EnergyLimit(bool EnergyLimit_Enabled)
{
    return ui->Switch_EnergyLimit->setChecked(EnergyLimit_Enabled);
}


Dialog_settings::~Dialog_settings()
{
    delete ui;
}

void Dialog_settings::on_Visual_R_min_Box_textEdited(const QString &arg1)
{
    emit transfer_Visual_R_min_Box_textEdited(arg1, ui->Visual_R_min_Box);
}

void Dialog_settings::on_Visual_R_max_Box_textEdited(const QString &arg1)
{
    emit transfer_Visual_R_max_Box_textEdited(arg1, ui->Visual_R_max_Box);
}

void Dialog_settings::on_Visual_Step_Box_textEdited(const QString &arg1)
{
    emit transfer_Visual_Step_Box_textEdited(arg1, ui->Visual_Step_Box);
}

void Dialog_settings::on_EnergyLimit_Box_textEdited(const QString &arg1)
{
    emit transfer_EnergyLimit_Box_textEdited(arg1, ui->EnergyLimit_Box);
}

void Dialog_settings::on_Decimal_point_Box_valueChanged(int arg1)
{
    emit transfer_Decimal_point_Box_valueChanged(arg1);
}

void Dialog_settings::on_Switch_EnergyLimit_stateChanged(int arg1)
{
    if(arg1 > 0)
    {
        ui->EnergyLimit_Box->setEnabled(true);
        ui->EnergyLimit_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: white; color: black;}");
    }
    else
    {
        ui->EnergyLimit_Box->setEnabled(false);
        ui->EnergyLimit_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: gray; color: black;}");
    }
    emit transfer_Switch_EnergyLimit_stateChanged(arg1);
}

