#include "splinetwobody_eigen.h"

double SplineTwoBody_Eigen::Potential(double r, double l)
{
    //return Potential_Equation_Expression.value();
    Potential_Equation_R = r;
    double Value = Potential_Equation_Expression.value();
    return Value;
    //return -7.5 * r * r * exp(-r) + l*(l+1.0)/(r*r);
}

void SplineTwoBody_Eigen::Initialize_Matrix_B_AHA(int l)
{
    //Calculate A matrix
    try {
        Matrix_m.resize(matrix_size, matrix_size);
    } catch (const std::exception& e) {
        MemAllocFailed = true;
        return;
    }
    Matrix_m.setZero();
    for (int i = 0; i < matrix_size - 1; i++)
    {
        Matrix_m(i,i) = -((Mesh[i+1] - Mesh[i]) + (Mesh[i+2] - Mesh[i+1])) / 3.0;
        Matrix_m(i + 1, i) = -(Mesh[i+2] - Mesh[i+1]) / 6.0;
        Matrix_m(i, i + 1) = -(Mesh[i+2] - Mesh[i+1]) / 6.0;
    }
    Matrix_m(matrix_size - 1,matrix_size - 1) = -((Mesh[matrix_size] - Mesh[matrix_size - 1]) + (Mesh[matrix_size+1] - Mesh[matrix_size])) / 3.0;
    //-----------------------------------------
    Eigen::MatrixXd Matrix_A_1H_Inverse(matrix_size, matrix_size);
    //Calculate inverse matrix of A
    try {
        Matrix_A_1H_Inverse = Matrix_m.inverse();
    } catch (const std::exception& e) {
        MemAllocFailed = true;
        return;
    }
    //-----------------------------------------
    //Multiple A by H
    for (int i = 0; i < matrix_size; i++)
    {
        Matrix_m(i, 0) = Matrix_A_1H_Inverse(i, 0) * (-(1.0 / (Mesh[1] - Mesh[0]) + 1.0 / (Mesh[2] - Mesh[1]))) + Matrix_A_1H_Inverse(i, 1) * 1.0 / (Mesh[2] - Mesh[1]);
        for (int j = 1; j < matrix_size-1; j++)
        {
            Matrix_m(i, j) = Matrix_A_1H_Inverse(i, j - 1) * 1.0 / (Mesh[j+1] - Mesh[j]) + Matrix_A_1H_Inverse(i, j) * (-(1.0 / (Mesh[j+1] - Mesh[j]) + 1.0/(Mesh[j+2] - Mesh[j+1]))) + Matrix_A_1H_Inverse(i, j + 1) * 1.0 / (Mesh[j+2] - Mesh[j+1]);
        }
        Matrix_m(i, matrix_size - 1) = Matrix_A_1H_Inverse(i, matrix_size - 2) * 1.0 / (Mesh[matrix_size] - Mesh[matrix_size-1]) + Matrix_A_1H_Inverse(i, matrix_size - 1) * (-(1.0 / (Mesh[matrix_size] - Mesh[matrix_size-1]) + 1.0/(Mesh[matrix_size+1] - Mesh[matrix_size])));
    }
    //-----------------------------------------
    //Copy A_1H to Xi
    try {
        Matrix_Xi.resize(matrix_size, matrix_size);
    } catch (const std::exception& e) {
        MemAllocFailed = true;
        return;
    }
    Matrix_Xi = Matrix_m;
    //-----------------------------------------
    //Add matrix B
    for (int i = 0; i < matrix_size; i++)
    {
        Matrix_Xi(i, i) = Matrix_Xi(i, i) + k_units * mass * Potential(Mesh[i+1], l) + l * (l + 1) / (Mesh[i+1]) / (Mesh[i+1]);
    }
    //-----------------------------------------
}

void SplineTwoBody_Eigen::CalculateEigenValuesAndVectors()
{
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> EigenResult;
    QVector<std::pair<int, double>> bound_states_index;
    try {
        EigenResult.compute(Matrix_Xi);
    } catch (const std::exception& e) {
        MemAllocFailed = true;
        return;
    }
    for(int i = 0; i < matrix_size; i++)
    {
        bound_states_index.append(std::pair(i, EigenResult.eigenvalues()[i]/(k_units*mass)));
    }
    std::sort(bound_states_index.begin(), bound_states_index.end(), [](std::pair<int, double> a, std::pair<int, double> b){return a.second < b.second;});
    TheLowestEnergy = bound_states_index[0].second;
    if(TheLowestEnergy < EnergyLimit)
    {
        BoundStatesExist = true;
    }
    else
    {
        BoundStatesExist = false;
        return;
    }
    for(int i = 0; i < matrix_size; i++)
    {
        if(bound_states_index[i].second > EnergyLimit)
        {
            bound_states_size = i;
            break;
        }
        else if(i == matrix_size-1)
        {
            bound_states_size = matrix_size;
        }
    }
    try {
        Matrix_Energies.resize(bound_states_size);
        Matrix_Xi.resize(matrix_size, bound_states_size);
    } catch (const std::exception& e) {
        MemAllocFailed = true;
        return;
    }
    Matrix_Xi.setZero();
    for(int i = 0; i < bound_states_size; i++)
    {
        Matrix_Energies(i) = bound_states_index[i].second;
        for(int j = 0; j < matrix_size; j++)
        {
            Matrix_Xi(j, i) = EigenResult.eigenvectors().real()(j, bound_states_index[i].first);
        }
    }
    Matrix_m = -Matrix_m*Matrix_Xi;
    Radius = new (std::nothrow) double[bound_states_size];
    if(!Radius)
    {
        MemAllocFailed = true;
        return;
    }
}

void SplineTwoBody_Eigen::MethodSplainNatural(int p)
{
    double S;
    double* Y_norm;
    double* X_norm;
    Y_norm = new (std::nothrow) double[matrix_size+2];
    if(!Y_norm)
    {
        MemAllocFailed = true;
        return;
    }
    X_norm = new (std::nothrow) double[matrix_size+2];
    if(!X_norm)
    {
        delete [] Y_norm;
        MemAllocFailed = true;
        return;
    }
    //Normalization
    double Norm;
    X_norm[0] = Mesh[0];
    Y_norm[0] = 0;
    for (int i = 0; i < matrix_size; i++)
    {
        X_norm[i+1] = Mesh[i+1];
        Y_norm[i+1] = Matrix_Xi(i, p) * Matrix_Xi(i, p);
    }
    X_norm[matrix_size+1] = Mesh[matrix_size+1];
    Y_norm[matrix_size+1] = 0;
    Norm = sqrt(MethodSplainNatural_Radius(X_norm, Y_norm, matrix_size+2));
    /////////////////
    //Radius
    X_norm[0] = Mesh[0];
    Y_norm[0] = 0;
    for (int i = 0; i < matrix_size; i++)
    {
        X_norm[i+1] = Mesh[i+1];
        Y_norm[i+1] = Matrix_Xi(i, p) * Matrix_Xi(i, p)*Mesh[i+1]*Mesh[i+1]*(mass2/(mass1+mass2))*(mass2/(mass1+mass2));
    }
    X_norm[matrix_size+1] = Mesh[matrix_size+1];
    Y_norm[matrix_size+1] = 0;
    Radius[p] = sqrt(MethodSplainNatural_Radius(X_norm, Y_norm, matrix_size+2)/Norm);
    /////////////////
    int t = 1;
    do{
        S = 0 + (1.0 / (Mesh[0 + 1] - Mesh[0]) * (Matrix_Xi(0, p) - 0) - (Mesh[0 + 1] - Mesh[0]) * 0.5 * 0 - (Mesh[0 + 1] - Mesh[0]) / 6.0 * (Matrix_m(0, p) - 0)) * (t * h_spl - Mesh[0]);
        S += 0 * 0.5 * (t * h_spl - Mesh[0]) * (t * h_spl - Mesh[0]);
        S += 1.0 / (6 * (Mesh[0 + 1] - Mesh[0])) * (Matrix_m(0, p) - 0) * (t * h_spl - Mesh[0]) * (t * h_spl - Mesh[0]) * (t * h_spl - Mesh[0]);
        Wave_Functions[p * Wave_and_Radial_size + t] = S /Norm;
        Radial_Functions[p * Wave_and_Radial_size + t] = S/(t * h_spl)/Norm;
        t++;
    }while(t*h_spl < Mesh[1]);
    for (int i = 1; i < matrix_size; i++)
    {
        do{
            S = Matrix_Xi(i-1, p) + (1.0 / (Mesh[i + 1] - Mesh[i]) * (Matrix_Xi(i, p) - Matrix_Xi(i-1, p)) - (Mesh[i + 1] - Mesh[i]) * 0.5 * Matrix_m(i-1, p) - (Mesh[i + 1] - Mesh[i]) / 6.0 * (Matrix_m(i, p) - Matrix_m(i-1, p))) * (t * h_spl - Mesh[i]);
            S += Matrix_m(i-1, p) * 0.5 * (t * h_spl - Mesh[i]) * (t * h_spl - Mesh[i]);
            S += 1.0 / (6 * (Mesh[i + 1] - Mesh[i])) * (Matrix_m(i, p) - Matrix_m(i-1,p)) * (t * h_spl - Mesh[i]) * (t * h_spl - Mesh[i]) * (t * h_spl - Mesh[i]);
            Wave_Functions[p * Wave_and_Radial_size + t] = S /Norm;
            Radial_Functions[p * Wave_and_Radial_size + t] = S/(t * h_spl)/Norm;
            t++;
        }while(t*h_spl < Mesh[i+1]);
    }
    do{
        S = Matrix_Xi(matrix_size-1, p) + (1.0 / (Mesh[matrix_size + 1] - Mesh[matrix_size]) * (0 - Matrix_Xi(matrix_size-1, p)) - (Mesh[matrix_size + 1] - Mesh[matrix_size]) * 0.5 * Matrix_m(matrix_size-1, p) - (Mesh[matrix_size + 1] - Mesh[matrix_size]) / 6.0 * (0 - Matrix_m(matrix_size-1, p))) * (t * h_spl - Mesh[matrix_size]);
        S += Matrix_m(matrix_size-1, p) * 0.5 * (t * h_spl - Mesh[matrix_size]) * (t * h_spl - Mesh[matrix_size]);
        S += 1.0 / (6 * (Mesh[matrix_size + 1] - Mesh[matrix_size])) * (0 - Matrix_m(matrix_size-1,p)) * (t * h_spl - Mesh[matrix_size]) * (t * h_spl - Mesh[matrix_size]) * (t * h_spl - Mesh[matrix_size]);
        Wave_Functions[p * Wave_and_Radial_size + t] = S /Norm;
        Radial_Functions[p * Wave_and_Radial_size + t] = S/(t * h_spl)/Norm;
        t++;
    }while(t*h_spl < Mesh[matrix_size+1]);
    Wave_Functions[p * Wave_and_Radial_size] = 0;
    Wave_Functions[p * Wave_and_Radial_size + Wave_and_Radial_size - 1] = 0;
    Radial_Functions[p * Wave_and_Radial_size] = Radial_Functions[p * Wave_and_Radial_size + 1];
    Radial_Functions[p * Wave_and_Radial_size + Wave_and_Radial_size - 1] = 0;
    for (int i = 0; i < matrix_size; i++)
    {
        Matrix_Xi(i, p) = Matrix_Xi(i, p) /Norm;
    }
    delete[] X_norm;
    delete[] Y_norm;
}

double SplineTwoBody_Eigen::MethodSplainNatural_Radius(double* X, double* Y, int size)
{
    int matrixsize = size - 2;
    double* B = new double[matrixsize];
    double* A = new double [matrixsize*3];
    double* m = new double[matrixsize + 2];
    for (int i = 1; i < matrixsize - 1; i++)
    {
        A[i] = (X[i + 1] - X[i]) / 6.0;
        A[i + matrixsize] = (-X[i] + X[i + 2]) / 3.0;
        A[i+ 2*matrixsize] = (X[i + 2] - X[i + 1]) / 6.0;
    }
    A[0] = (-X[0] + X[2]) / 3.0;
    A[0+ matrixsize] = (X[2] - X[1]) / 6.0;
    A[0+2* matrixsize] = 0;
    A[matrixsize - 1] = (X[matrixsize] - X[matrixsize - 1]) / 6.0;
    A[matrixsize - 1+ matrixsize] = (-X[matrixsize - 1] + X[matrixsize + 1]) / 3.0;
    A[matrixsize - 1+2* matrixsize] = 0;
    for (int i = 0; i < matrixsize; i++)
    {
        B[i] = (Y[i + 2] - Y[i + 1]) / (X[i + 2] - X[i + 1]) - (Y[i + 1] - Y[i]) / (X[i + 1] - X[i]);
    }
    ProgonkaRIGHT_SPLAINMOD(matrixsize, A, B, m);
    for (int i = matrixsize + 1; i > 0; i--)
    {
        m[i] = m[i-1];
    }
    m[0] = 0;
    m[matrixsize + 1] = 0;
    double C;
    double IntegralOfSpline = 0;
    for (int j = 0; j < size - 1; j++)
    {
        C = (1.0 / (X[j + 1] - X[j]) * (Y[j + 1] - Y[j]) - (X[j + 1] - X[j]) * 0.5 * m[j] - (X[j + 1] - X[j]) / 6.0 * (m[j + 1] - m[j]));
        IntegralOfSpline +=
            Y[j] * (X[j + 1] - X[j]) +
            C / 2 * (X[j + 1] - X[j]) * (X[j + 1] - X[j]) +
            m[j] / 6 * (X[j + 1] - X[j]) * (X[j + 1] - X[j]) * (X[j + 1] - X[j]) +
            1.0 / (24 * (X[j + 1] - X[j])) * (m[j + 1] - m[j]) * (X[j + 1] - X[j]) * (X[j + 1] - X[j]) * (X[j + 1] - X[j]) * (X[j + 1] - X[j]);
    }
    delete[] A;
    delete[] B;
    delete[] m;
    return IntegralOfSpline;
}

void SplineTwoBody_Eigen::ProgonkaRIGHT_SPLAINMOD(unsigned int n, double* a, double* b, double* x)
{
    double* alfa = new double[n];
    double* beta = new double[n];
    alfa[0] = -a[0+n] / a[0];
    beta[0] = b[0] / a[0];
    for (int i = 1; i < n; i++)
    {
        alfa[i] = -a[i+2* n] / (a[i] * alfa[i - 1] + a[i+n]);
        beta[i] = (b[i] - a[i] * beta[i - 1]) / (a[i] * alfa[i - 1] + a[i+n]);
    }
    x[n - 1] = beta[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        x[i] = alfa[i] * x[i + 1] + beta[i];
    }
    delete[] alfa;
    delete[] beta;
}

void SplineTwoBody_Eigen::RadiusDistribution(double r1)
{

}

// for (int i = 0; i < matrix_size; i++)
// {
//     QString stroka = "";
//     for (int j = 0; j < matrix_size; j++)
//     {
//         stroka = stroka + " " + QString::number(Result(i, j));
//     }
//     qDebug() << stroka;
// }
// qDebug() << " ";
