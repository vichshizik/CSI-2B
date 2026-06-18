#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::on_Potential_Equation_Box_textChanged(const QString &arg1)
{
    Potential_Equation_Parse();
    DrawMesh();
}

void MainWindow::on_Calc_settings_R_min_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::on_Calc_settings_R_max_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::on_Calc_settings_Step_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::on_Calc_settings_SplineStep_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::on_Calc_settings_l_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::on_Calc_settings_Mass1_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::on_Calc_settings_Mass2_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::Calc_Settings_textChanged()
{
    Check_container(Calc_settings_success[0], ui->Calc_settings_R_min_Box, Calc_settings_R_min_d);
    Check_container(Calc_settings_success[1], ui->Calc_settings_R_max_Box, Calc_settings_R_max_d);
    Check_container(Calc_settings_success[2], ui->Calc_settings_Step_Box, Calc_settings_h_step_d);
    Check_container(Calc_settings_success[3], ui->Calc_settings_SplineStep_Box, Calc_settings_h_spline_step_d);
    Check_container(Calc_settings_success[4], ui->Calc_settings_l_Box, Calc_settings_l_d);
    Check_container(Calc_settings_success[5], ui->Calc_settings_Mass1_Box, Calc_settings_Mass1_d);
    Check_container(Calc_settings_success[6], ui->Calc_settings_Mass2_Box, Calc_settings_Mass2_d);
    Check_container(Calc_settings_success[7], ui->Calc_settings_Radius1_Box, Calc_settings_Radius1_d);
    Check_container(Calc_settings_success[8], ui->Calc_settings_Radius2_Box, Calc_settings_Radius2_d);

    if(Calc_settings_success[0] && (Calc_settings_R_min_d < 0))
    {
        ui->Calc_settings_R_min_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_R_min_Box->setToolTip("The entered value cannot be lower than 0");
        Calc_settings_success[0] = false;
    }
    else if(Calc_settings_success[0])
    {
        ui->Calc_settings_R_min_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: white; color: black;}");
        ui->Calc_settings_R_min_Box->setToolTip("");
    }


    if(Calc_settings_success[1] && (Calc_settings_R_max_d < 1e-15))
    {
        ui->Calc_settings_R_max_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_R_max_Box->setToolTip("The entered value cannot be lower than 0");
        Calc_settings_success[1] = false;
    }
    else if(Calc_settings_success[1])
    {
        ui->Calc_settings_R_max_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: white; color: black;}");
        ui->Calc_settings_R_max_Box->setToolTip("");
    }
    if(Calc_settings_success[0] && Calc_settings_success[1] && (Calc_settings_R_max_d < Calc_settings_R_min_d))
    {
        ui->Calc_settings_R_max_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_R_max_Box->setToolTip("The entered value cannot be lower than Rmin");
        Calc_settings_success[1] = false;
        ui->Calc_settings_R_min_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_R_min_Box->setToolTip("The entered value cannot be higher than Rmax");
        Calc_settings_success[0] = false;
    }


    if(Calc_settings_success[2] && (Calc_settings_h_step_d < 1e-15))
    {
        ui->Calc_settings_Step_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_Step_Box->setToolTip("The entered value cannot be lower or equal than 0");
        Calc_settings_success[2] = false;
    }
    else if(Calc_settings_success[2])
    {
        ui->Calc_settings_Step_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: white; color: black;}");
        ui->Calc_settings_Step_Box->setToolTip("");
    }


    if(Calc_settings_success[3] && (Calc_settings_h_spline_step_d < 1e-15))
    {
        ui->Calc_settings_SplineStep_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_SplineStep_Box->setToolTip("The entered value cannot be lower or equal than 0");
        Calc_settings_success[3] = false;
    }
    else if(Calc_settings_success[3])
    {
        ui->Calc_settings_SplineStep_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: white; color: black;}");
        ui->Calc_settings_SplineStep_Box->setToolTip("");
    }
    if(Calc_settings_success[2] && Calc_settings_success[3] && (Calc_settings_h_step_d < Calc_settings_h_spline_step_d))
    {
        ui->Calc_settings_Step_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_Step_Box->setToolTip("The entered value cannot be lower than spline step");
        Calc_settings_success[2] = false;
        ui->Calc_settings_SplineStep_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_SplineStep_Box->setToolTip("The entered value cannot be lower than mesh step");
        Calc_settings_success[3] = false;
    }


    if(Calc_settings_success[0] && Calc_settings_success[1] && Calc_settings_success[2])
    {
        CountMeshPoints = (Calc_settings_R_max_d - Calc_settings_R_min_d)/Calc_settings_h_step_d + 1;
        if(CountMeshPoints < 3)
        {
            ui->Calc_settings_Step_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
            ui->Calc_settings_Step_Box->setToolTip("The entered values provides too few mesh points");
            ui->Calc_settings_R_max_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
            ui->Calc_settings_R_max_Box->setToolTip("The entered values provides too few mesh points");
            ui->Calc_settings_R_min_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
            ui->Calc_settings_R_min_Box->setToolTip("The entered values provides too few mesh points");
            Calc_settings_success[0] = false;
            Calc_settings_success[1] = false;
            Calc_settings_success[2] = false;
        }
    }


    if(Calc_settings_success[4] && Calc_settings_l_d < 0)
    {
        ui->Calc_settings_l_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_l_Box->setToolTip("The entered value cannot be lower than 0");
        Calc_settings_success[4] = false;
    }
    if(Calc_settings_success[5] && Calc_settings_Mass1_d < 0)
    {
        ui->Calc_settings_Mass1_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_Mass1_Box->setToolTip("The entered value cannot be lower than 0");
        Calc_settings_success[5] = false;
    }
    if(Calc_settings_success[6] && Calc_settings_Mass2_d < 0)
    {
        ui->Calc_settings_Mass2_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_Mass2_Box->setToolTip("The entered value cannot be lower than 0");
        Calc_settings_success[6] = false;
    }

    if(Calc_settings_success[7] && Calc_settings_Radius1_d < 0)
    {
        ui->Calc_settings_Radius1_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_Radius1_Box->setToolTip("The entered value cannot be lower than 0");
        Calc_settings_success[7] = false;
    }
    if(Calc_settings_success[8] && Calc_settings_Radius2_d < 0)
    {
        ui->Calc_settings_Radius2_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        ui->Calc_settings_Radius2_Box->setToolTip("The entered value cannot be lower than 0");
        Calc_settings_success[8] = false;
    }


    Set_button_activity(ui->Calculate_Button, Calc_settings_success);
    CalculateRAMNeeded();
    DrawMesh();
}

void MainWindow::Potential_Settings_textChanged()
{
    Check_container(Potential_settings_success[0], Settings_Window->get_Potential_R_max_Box(), Potential_R_max_d);
    Check_container(Potential_settings_success[1], Settings_Window->get_Potential_R_min_Box(), Potential_R_min_d);
    Check_container(Potential_settings_success[2], Settings_Window->get_Potential_Step_Box(), Potential_Step_d);

    if(Potential_settings_success[0] && Potential_R_max_d < 0)
    {
        Settings_Window->get_Potential_R_max_Box()->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        Settings_Window->get_Potential_R_max_Box()->setToolTip("The entered value cannot be lower than 0");
        Potential_settings_success[0] = false;
    }

    if(Potential_settings_success[1] && Potential_R_min_d < 0)
    {
        Settings_Window->get_Potential_R_min_Box()->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        Settings_Window->get_Potential_R_min_Box()->setToolTip("The entered value cannot be lower than 0");
        Potential_settings_success[1] = false;
    }

    if(Potential_settings_success[0] && Potential_settings_success[1] && (Potential_R_max_d < Potential_R_min_d))
    {
        Settings_Window->get_Potential_R_min_Box()->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        Settings_Window->get_Potential_R_min_Box()->setToolTip("The entered value cannot be higher than Rmax");
        Settings_Window->get_Potential_R_max_Box()->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        Settings_Window->get_Potential_R_max_Box()->setToolTip("The entered value cannot be lower than Rmin");
        Potential_settings_success[0] = false;
        Potential_settings_success[1] = false;
    }


    if(Potential_settings_success[2] && Potential_Step_d < 1e-15)
    {
        Settings_Window->get_Potential_Step_Box()->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: orange; color: black;}");
        Settings_Window->get_Potential_Step_Box()->setToolTip("The entered value cannot be equal or lower than 0");
        Potential_settings_success[2] = false;
    }

    Potential_Equation_Parse();
}

void MainWindow::receive_Potential_R_max_Box_textChanged(QString arg1)
{
    Potential_Settings_textChanged();
}

void MainWindow::receive_Potential_R_min_Box_textChanged(QString arg1)
{
    Potential_Settings_textChanged();
}

void MainWindow::receive_Potential_Step_Box_textChanged(QString arg1)
{
    Potential_Settings_textChanged();
}

void MainWindow::receive_EnergyLimit_Box_textChanged(QString arg1)
{
    Check_container(Potential_settings_success[3], Settings_Window->get_Potential_EnergyLimit_Box(), EnergyLimit_d);
}

void MainWindow::receive_Decimal_point_Box_valueChanged(int arg1)
{
    Decimal_points_numbers = arg1;
}

void MainWindow::receive_Switch_EnergyLimit_stateChanged(int arg1)
{
    if(arg1 > 0)
    {
        EnergyLimit_Enabled = true;
    }
    else
    {
        EnergyLimit_Enabled = false;
    }
}

void MainWindow::on_Calc_settings_Radius1_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}

void MainWindow::on_Calc_settings_Radius2_Box_textChanged(const QString &arg1)
{
    Calc_Settings_textChanged();
}
