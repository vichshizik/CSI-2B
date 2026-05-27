#ifndef SPLINETWOBODY_EIGEN_H
#define SPLINETWOBODY_EIGEN_H
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <QVector>
#include <QDebug>
#include <QProgressBar>
#include "exprtk_complex_adaptor.hpp"
#include <Eigen/Dense>
using Eigen::MatrixXd;

class SplineTwoBody_Eigen
{
private:
    double* Radial_Functions = nullptr;
    double* Wave_Functions = nullptr;
    double* Mesh = nullptr;
    double* Radius = nullptr;
    Eigen::MatrixXd Matrix_Xi;
    Eigen::MatrixXd Matrix_m;
    Eigen::VectorXd Matrix_Energies;
    double rmax, rmin, h_spl;
    double radius1, radius2;
    int matrix_size;
    int bound_states_size;
    int Wave_and_Radial_size;

    double k_units = 2.0;
    double mass = 1.0;
    double mass1 = 1.0;
    double mass2 = 1.0;
    double TheLowestEnergy;
    double EnergyLimit;
    bool BoundStatesExist;
    bool MemAllocFailed;

    exprtk::symbol_table<double> Potential_Equation_Table;
    exprtk::expression<double> Potential_Equation_Expression;
    exprtk::parser<double> Potential_Equation_Parser;
    double Potential_Equation_R;

public:
    SplineTwoBody_Eigen()
    {

    }

    void Initialize_Param(QVector<double> Mesh_input, double h_spline_step, int Units_selected, double m1, double m2, double r1, double r2, std::string expression)
    {
        this->mass1 = m1;
        this->mass2 = m2;
        this->radius1 = r1;
        this->radius2 = r2;
        this->mass = (m1*m2)/(m1+m2);
        this->EnergyLimit = 0;
        this->BoundStatesExist = false;
        this->MemAllocFailed = false;
        this->rmax = Mesh_input[Mesh_input.size()-1];
        this->rmin = Mesh_input[0];
        this->h_spl = h_spline_step;
        this->matrix_size = Mesh_input.size() - 2;
        this->Wave_and_Radial_size = int((rmax - rmin) / h_spl + 1);
        this->Mesh = new (std::nothrow) double [matrix_size + 2];
        if(!Mesh)
        {
            MemAllocFailed = true;
            return;
        }
        for(int i = 0; i < Mesh_input.size(); i++)
        {
            Mesh[i] = Mesh_input[i];
        }
        switch(Units_selected)
        {
        case(0):
            k_units = 0.04784506682;
            break;
        case(1):
            k_units = 2;
            break;
        default:
            k_units = 2;
            break;
        }
        Potential_Equation_Parser.settings().disable_all_control_structures();
        Potential_Equation_Parser.settings().disable_all_logic_ops();
        Potential_Equation_Table.add_variable("r", Potential_Equation_R);
        Potential_Equation_Expression.register_symbol_table(Potential_Equation_Table);
        Potential_Equation_Parser.compile(expression, Potential_Equation_Expression);
    }

    bool Calculate_on_CPU(int l, QProgressBar *progress)
    {
        progress->setValue(1);
        progress->setFormat("Calc Matrices");
        Initialize_Matrix_B_AHA(l);
        if(MemAllocFailed)
        {
            return false;
        }
        progress->setValue(2);
        progress->setFormat("Calc Eigen Values");
        CalculateEigenValuesAndVectors();
        if(MemAllocFailed)
        {
            return false;
        }
        if(BoundStatesExist){
            progress->setValue(3);
            progress->setFormat("Calc Wave Functions");
            Calculate_Wave_Functions();
            if(MemAllocFailed)
            {
                return false;
            }
        }
        else
        {
            progress->setFormat("Error");
            progress->setValue(4);
            return false;
        }
        progress->setFormat("Done");
        progress->setValue(4);
        return true;
    }

    bool Calculate_on_CPU(int l, QProgressBar *progress, double EnergyLimit)
    {
        this->EnergyLimit = EnergyLimit;
        progress->setValue(1);
        progress->setFormat("Calc Matrices");
        Initialize_Matrix_B_AHA(l);
        if(MemAllocFailed)
        {
            return false;
        }
        progress->setValue(2);
        progress->setFormat("Calc Eigen Values");
        CalculateEigenValuesAndVectors();
        if(MemAllocFailed)
        {
            return false;
        }
        if(BoundStatesExist){
            progress->setValue(3);
            progress->setFormat("Calc Wave Functions");
            Calculate_Wave_Functions();
            if(MemAllocFailed)
            {
                return false;
            }
        }
        else
        {
            progress->setFormat("Error");
            progress->setValue(4);
            return false;
        }
        progress->setFormat("Done");
        progress->setValue(4);
        return true;
    }

    void Calculate_Wave_Functions()
    {
        this->Radial_Functions = new (std::nothrow) double [bound_states_size*Wave_and_Radial_size];
        if(!Radial_Functions)
        {
            MemAllocFailed = true;
            return;
        }
        this->Wave_Functions = new (std::nothrow) double [bound_states_size*Wave_and_Radial_size];
        if(!Wave_Functions)
        {
            MemAllocFailed = true;
            return;
        }
        for (int i = 0; i < bound_states_size; i++)
        {
            MethodSplainNatural(i);
            if(MemAllocFailed)
            {
                return;
            }
        }
    }

    void Export_Wave_Functions(QVector<double> &Exported_x_axis , QVector<QVector<double>> &Exported_Wave_functions, QVector<QVector<double>> &Exported_Radial_functions, QVector<double> &Exported_Energies, int &Exported_size_splined, int &Exported_size_states)
    {
        Exported_x_axis.clear();
        Exported_Radial_functions.clear();
        Exported_Wave_functions.clear();
        Exported_Energies.clear();
        Exported_size_states = bound_states_size;
        Exported_size_splined = Wave_and_Radial_size;

        for(int i = 0; i < Wave_and_Radial_size; i++)
        {
            Exported_x_axis.append(rmin + i*h_spl);
        }
        QVector<double> Wave;
        QVector<double> Radial;
        for (int i = 0; i < bound_states_size; i++)
        {
            Wave.clear();
            Radial.clear();
            for (int j = 0; j < Wave_and_Radial_size; j++)
            {
                Wave.append(Wave_Functions[j + i*Wave_and_Radial_size]);
                Radial.append(Radial_Functions[j + i*Wave_and_Radial_size]);
            }
            Exported_Wave_functions.append(Wave);
            Exported_Radial_functions.append(Radial);
        }
        for (int i = 0; i < bound_states_size; i++)
        {
            Exported_Energies.append(Matrix_Energies(i));
        }
    }

    void Export_Mesh(QVector<double> &Exported_x_axis, QVector<QVector<double>> &Exported_Mesh_Radial, QVector<QVector<double>> &Exported_Mesh_Wave, int &Exported_size_mesh)
    {
        Exported_x_axis.clear();
        Exported_Mesh_Radial.clear();
        Exported_Mesh_Wave.clear();
        Exported_size_mesh = matrix_size+2;
        for(int i = 0; i < Exported_size_mesh; i++)
        {
            Exported_x_axis.append(Mesh[i]);
        }
        QVector<double> Mesh_Radial;
        QVector<double> Mesh_Wave;
        for (int i = 0; i < bound_states_size; i++)
        {
            Mesh_Wave.clear();
            Mesh_Wave.append(0);
            Mesh_Radial.clear();
            Mesh_Radial.append(Radial_Functions[i*Wave_and_Radial_size]);
            for (int j = 1; j < matrix_size+1; j++)
            {
                Mesh_Wave.append(Matrix_Xi(j-1, i));
                Mesh_Radial.append(Matrix_Xi(j-1, i)/Exported_x_axis[j+1]);
            }
            Mesh_Wave.append(0);
            Mesh_Radial.append(0);
            Exported_Mesh_Wave.append(Mesh_Wave);
            Exported_Mesh_Radial.append(Mesh_Radial);
        }
    }

    void Export_Radius(QVector<double> &Exported_Radius, QVector<double> &Exported_Radius_Wave)
    {
        for(int i =0; i < bound_states_size; i++)
        {
            Exported_Radius_Wave.append(Radius[i]);
            Exported_Radius.append(sqrt(Radius[i]*Radius[i] + radius1*radius1 + radius2*radius2));
        }
    }

    void Show_WaveFunctions()
    {
        for (int i = 0; i < bound_states_size; i++)
        {
            QString stroka = "";
            for (int j = 0; j < Wave_and_Radial_size; j++)
            {
                stroka = stroka + " " + QString::number(Wave_Functions[j + i*Wave_and_Radial_size]);
            }
            qDebug() << stroka;
        }
        qDebug() << " --- ";
    }

    double GetGroundState()
    {
        return TheLowestEnergy;
    }

    bool IsMemAllocFailed()
    {
        return MemAllocFailed;
    }

    bool IsBoundStatesExist()
    {
        return BoundStatesExist;
    }

    ~SplineTwoBody_Eigen()
    {
        Matrix_Xi.resize(0,0);
        Matrix_m.resize(0,0);
        Matrix_Energies.resize(0);
        if(Radial_Functions)
        {
            delete[] Radial_Functions;
        }
        if(Wave_Functions)
        {
            delete[] Wave_Functions;
        }
        if(Mesh)
        {
            delete[] Mesh;
        }
        if(Radius)
        {
            delete[] Radius;
        }
    }

private:
    //Calculation of Matrices and Potential
    void Initialize_Matrix_B_AHA(int l);
    double Potential(double r, double l);
    void CalculateEigenValuesAndVectors();
    //Methods
    void MethodSplainNatural(int p);
    void ProgonkaRIGHT_SPLAINMOD(unsigned int n, double* a, double* b, double* x);
    double MethodSplainNatural_Radius(double* X, double* Y, int size);
    void RadiusDistribution(double r1);
};

#endif // SPLINETWOBODY_EIGEN_H
