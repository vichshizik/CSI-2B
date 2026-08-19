#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::LoadSettings(QString path)
{
    QSettings settings(path, QSettings::IniFormat);
    QString settings_text;
    //Main Window
    settings_text = settings.value("Potential_Equation_Box", "-7.5*r*r*exp(-r)").toString();
    Potential_Expression = settings_text.toStdString();
    ui->Potential_Equation_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_Step_Box", "1").toString();
    Calc_settings_h_step_d = settings_text.toDouble();
    ui->Calc_settings_Step_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_R_min_Box", "0").toString();
    Calc_settings_R_min_d = settings_text.toDouble();
    ui->Calc_settings_R_min_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_R_max_Box", "10").toString();
    Calc_settings_R_max_d = settings_text.toDouble();
    ui->Calc_settings_R_max_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_SplineStep_Box", "0.5").toString();
    Calc_settings_h_spline_step_d = settings_text.toDouble();
    ui->Calc_settings_SplineStep_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_Mass1_Box", "1").toString();
    Calc_settings_Mass1_d = settings_text.toDouble();
    ui->Calc_settings_Mass1_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_Mass2_Box", "1").toString();
    Calc_settings_Mass2_d = settings_text.toDouble();
    ui->Calc_settings_Mass2_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_l_Box", "0").toString();
    Calc_settings_l_d = settings_text.toInt();
    ui->Calc_settings_l_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_Radius1_Box", "0").toString();
    Calc_settings_Radius1_d = settings_text.toDouble();
    ui->Calc_settings_Radius1_Box->setText(settings_text);

    settings_text = settings.value("Calc_settings_Radius2_Box", "0").toString();
    Calc_settings_Radius2_d = settings_text.toDouble();
    ui->Calc_settings_Radius2_Box->setText(settings_text);

    settings_text = settings.value("UnitsType_Select", "0").toString();
    ui->UnitsType_Select->setCurrentIndex(settings_text.toInt());

    settings_text = settings.value("Visual_R_min_Box", "0").toString();
    Visual_R_min_d = settings_text.toDouble();
    Settings_Window->set_Visual_R_min_Box(Visual_R_min_d);

    settings_text = settings.value("Visual_R_max_Box", "100").toString();
    Visual_R_max_d = settings_text.toDouble();
    Settings_Window->set_Visual_R_max_Box(Visual_R_max_d);

    settings_text = settings.value("Visual_Step_Box", "0.01").toString();
    Visual_Step_d = settings_text.toDouble();
    Settings_Window->set_Visual_Step_Box(Visual_Step_d);

    settings_text = settings.value("Decimal_point_Box", "3").toString();
    Decimal_points_numbers = settings_text.toInt();
    Settings_Window->set_Decimal_point_Box(Decimal_points_numbers);

    settings_text = settings.value("EnergyLimit_Box", "0").toString();
    EnergyLimit_d = settings_text.toDouble();
    Settings_Window->set_Potential_EnergyLimit_Box(EnergyLimit_d);

    settings_text = settings.value("EnergyLimit_Enabled", "0").toString();
    EnergyLimit_Enabled = settings_text.toInt();
    Settings_Window->set_Potential_EnergyLimit_Box(EnergyLimit_Enabled);

    Potential_Equation_Parse();
    DrawMesh();
}

void MainWindow::SaveSettings(QString path)
{
    QSettings settings(path, QSettings::IniFormat);
    QString settings_text;

    //Main Window
    settings_text = QString::number(Calc_settings_h_step_d);
    settings.setValue("Calc_settings_Step_Box", settings_text);

    settings_text = QString::number(Calc_settings_R_min_d);
    settings.setValue("Calc_settings_R_min_Box", settings_text);

    settings_text = QString::number(Calc_settings_R_max_d);
    settings.setValue("Calc_settings_R_max_Box", settings_text);

    settings_text = QString::number(Calc_settings_h_spline_step_d);
    settings.setValue("Calc_settings_SplineStep_Box", settings_text);

    settings_text = QString::number(Calc_settings_Mass1_d);
    settings.setValue("Calc_settings_Mass1_Box", settings_text);

    settings_text = QString::number(Calc_settings_Mass2_d);
    settings.setValue("Calc_settings_Mass2_Box", settings_text);

    settings_text = QString::number(Calc_settings_l_d);
    settings.setValue("Calc_settings_l_Box", settings_text);

    settings_text = QString::number(Calc_settings_Radius1_d);
    settings.setValue("Calc_settings_Radius1_Box", settings_text);

    settings_text = QString::number(Calc_settings_Radius2_d);
    settings.setValue("Calc_settings_Radius2_Box", settings_text);

    settings_text = QString::fromStdString(Potential_Expression);
    settings.setValue("Potential_Equation_Box", settings_text);

    settings_text = QString::number(ui->UnitsType_Select->currentIndex());
    settings.setValue("UnitsType_Select", settings_text);

    settings_text = QString::number(Visual_R_max_d);
    settings.setValue("Visual_R_max_Box", settings_text);

    settings_text = QString::number(Visual_R_min_d);
    settings.setValue("Visual_R_min_Box", settings_text);

    settings_text = QString::number(Visual_Step_d);
    settings.setValue("Visual_Step_Box", settings_text);

    settings_text = QString::number(Decimal_points_numbers);
    settings.setValue("Decimal_point_Box", settings_text);

    settings_text = QString::number(EnergyLimit_d);
    settings.setValue("EnergyLimit_Box", settings_text);

    settings_text = QString::number(EnergyLimit_Enabled);
    settings.setValue("EnergyLimit_Enabled", settings_text);
}

void MainWindow::on_actionSave_triggered()
{
    QString DirectoryPath = QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, tr("Choose Directory"), qApp->applicationDirPath(), "*.ini"));
    if(!DirectoryPath.isEmpty())
    {
        if(!std::filesystem::exists(DirectoryPath.toStdString()))
        {
            SaveSettings(DirectoryPath);
        }
        else
        {
            QMessageBox::StandardButton RewriteQustionFile = QMessageBox::question( this, "Confirm the action", tr("File already exist with such name \n Do you want to overwrite it?"),
                                                                                   QMessageBox::No | QMessageBox::Yes,
                                                                                   QMessageBox::Yes);
            switch (RewriteQustionFile){
            case(QMessageBox::Yes):
                SaveSettings(DirectoryPath);
                break;
            case(QMessageBox::No):
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QString DirectoryPath = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Choose Directory"), qApp->applicationDirPath(), "*.ini"));
    if(!DirectoryPath.isEmpty())
    {
        LoadSettings(DirectoryPath);
    }
}
