#include "mainwindow.h"
#include "./ui_mainwindow.h"


void MainWindow::Potential_Equation_Parse()
{
    if(Visual_settings_success[0] && Visual_settings_success[1] && Visual_settings_success[2])
    {
        Potential_Expression = ui->Potential_Equation_Box->text().toStdString();
        if(Potential_Equation_Parser.compile(Potential_Expression, Potential_Equation_Expression))
        {
            int N = (Visual_R_max_d - Visual_R_min_d)/Visual_Step_d + 1;
            Potential_success = true;
            //Potential_Equation_Table.add_constants();
            ui->PlotPotential->graph(0)->data()->clear();
            DataPotentialPoints_x.clear();
            DataPotentialPoints_y.clear();
            ui->Potential_Equation_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: white; color: black;}");
            for(int i = 0; i < N; i++)
            {
                Potential_Equation_Parse_R = Visual_R_min_d + i*Visual_Step_d;
                DataPotentialPoints_x.append(Potential_Equation_Parse_R);
                DataPotentialPoints_y.append(Potential_Equation_Expression.value());
            }
            ui->PlotPotential->graph(0)->setData(DataPotentialPoints_x, DataPotentialPoints_y);
            ui->PlotPotential->replot();
        }
        else
        {
            Potential_success = false;
            ui->Potential_Equation_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: red; color: black;}");
            ui->Calc_settings_R_min_Box->setToolTip("The entered expression is incorrect");
        }
        Set_button_activity(ui->Calculate_Button, Potential_success);
    }
}
