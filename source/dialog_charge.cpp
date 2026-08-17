#include "dialog_charge.h"
#include "ui_dialog_charge.h"

Dialog_charge::Dialog_charge(int Decimal_points_numbers, int Sci_times_notation, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Dialog_charge)
{
    ui->setupUi(this);
    this->Decimal_points_numbers = Decimal_points_numbers;
    this->Sci_times_notation = Sci_times_notation;

    connect(ui->PlotChargeDistribution, SIGNAL(mouseMove(QMouseEvent*)), SLOT(MouseMovePlotChargeDistribution(QMouseEvent*)));
    connect(ui->PlotChargeDistribution, SIGNAL(mouseWheel(QWheelEvent*)),  SLOT(MouseWheelPlotChargeDistribution(QWheelEvent*)));

    CoordText = new QCPItemText(ui->PlotChargeDistribution);
    CoordText->setText(QString("(%1, %2)").arg(0).arg(0));
    CoordText->position->setCoords(QPointF(ui->PlotChargeDistribution->xAxis->pixelToCoord(50), ui->PlotChargeDistribution->yAxis->pixelToCoord(-10)));
    CoordText->setFont(QFont(font().family(), 10));


    //Settings for PlotRootsFunction
    ui->PlotChargeDistribution->axisRect()->setupFullAxesBox();
    ui->PlotChargeDistribution->xAxis->setRange(-20, 20);
    ui->PlotChargeDistribution->yAxis->setRange(-1, 1);
    ui->PlotChargeDistribution->xAxis->setVisible(true);
    ui->PlotChargeDistribution->yAxis->setVisible(true);
    ui->PlotChargeDistribution->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->PlotChargeDistribution->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->PlotChargeDistribution->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->PlotChargeDistribution->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->PlotChargeDistribution->xAxis->grid()->setSubGridVisible(true);
    ui->PlotChargeDistribution->yAxis->grid()->setSubGridVisible(true);
    ui->PlotChargeDistribution->xAxis->grid()->setZeroLinePen(Qt::SolidLine);
    ui->PlotChargeDistribution->yAxis->grid()->setZeroLinePen(Qt::SolidLine);
    ui->PlotChargeDistribution->addGraph();
    ui->PlotChargeDistribution->addGraph();
    ui->PlotChargeDistribution->addGraph();
    ui->PlotChargeDistribution->addGraph();
    ui->PlotChargeDistribution->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->PlotChargeDistribution->graph(0)->setLineStyle(QCPGraph::LineStyle(QCPCurve::lsLine));
    ui->PlotChargeDistribution->graph(0)->setPen(QPen(Qt::blue));
    ui->PlotChargeDistribution->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->PlotChargeDistribution->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->PlotChargeDistribution->graph(1)->setPen(QPen(Qt::red, 1.0, Qt::SolidLine));
    ui->PlotChargeDistribution->graph(2)->setLineStyle(QCPGraph::LineStyle(QCPCurve::lsLine));
    ui->PlotChargeDistribution->graph(2)->setPen(QPen(Qt::blue, 0.5, Qt::DashLine));
    ui->PlotChargeDistribution->graph(3)->setLineStyle(QCPGraph::LineStyle(QCPCurve::lsLine));
    ui->PlotChargeDistribution->graph(3)->setPen(QPen(Qt::blue, 0.5, Qt::DashLine));
    ui->PlotChargeDistribution->xAxis->setLabelFont(QFont("Segoe UI", 12));
    ui->PlotChargeDistribution->yAxis->setLabelFont(QFont("Segoe UI", 12));
    ui->PlotChargeDistribution->xAxis->setLabel("r");
    ui->PlotChargeDistribution->xAxis->setLabelColor(Qt::black);
    ui->PlotChargeDistribution->yAxis->setLabel("Psi(r)");
    ui->PlotChargeDistribution->yAxis->setLabelColor(Qt::black);
    ui->PlotChargeDistribution->rescaleAxes();
    //ui->PlotChargeDistribution->graph(0)->setData(Functions_x, Wave_functions[0]);
    ui->PlotChargeDistribution->replot();
}

// void Dialog_charge::ChargeDistribution_Parse()
// {
//     if(Potential_settings_success[0] && Potential_settings_success[1] && Potential_settings_success[2])
//     {
//         Potential_Expression = ui->Potential_Equation_Box->text().toStdString();
//         if(Potential_Equation_Parser.compile(Potential_Expression, Potential_Equation_Expression))
//         {
//             int N = (Visual_R_max_d - Visual_R_min_d)/Visual_Step_d + 1;
//             Potential_success = true;
//             //Potential_Equation_Table.add_constants();
//             ui->PlotPotential->graph(0)->data()->clear();
//             DataPotentialPoints_x.clear();
//             DataPotentialPoints_y.clear();
//             ui->Potential_Equation_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: white; color: black;}");
//             for(int i = 0; i < N; i++)
//             {
//                 Potential_Equation_Parse_R = Visual_R_min_d + i*Visual_Step_d;
//                 DataPotentialPoints_x.append(Visual_Equation_Parse_R);
//                 DataPotentialPoints_y.append(Visual_Equation_Expression.value());
//             }
//             ui->PlotPotential->graph(0)->setData(DataPotentialPoints_x, DataPotentialPoints_y);
//             ui->PlotPotential->replot();
//         }
//         else
//         {
//             Potential_success = false;
//             ui->Potential_Equation_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: red; color: black;}");
//             ui->Calc_settings_R_min_Box->setToolTip("The entered expression is incorrect");
//         }
//         Set_button_activity(ui->Calculate_Button, Potential_success);
//     }
// }


Dialog_charge::~Dialog_charge()
{
    delete ui;
}

void Dialog_charge::on_ChargeDistribution_Equation_Box_textEdited(const QString &arg1)
{

}

void Dialog_charge::DrawCrossAndCoord(QPoint Cursor)
{
    ui->PlotChargeDistribution->graph(2)->data()->clear();
    ui->PlotChargeDistribution->graph(3)->data()->clear();
    QVector<double> xData;
    QVector<double> yData;
    xData.append(ui->PlotChargeDistribution->xAxis->pixelToCoord(Cursor.x()));
    yData.append(ui->PlotChargeDistribution->yAxis->pixelToCoord(0));
    xData.append(ui->PlotChargeDistribution->xAxis->pixelToCoord(Cursor.x()));
    yData.append(ui->PlotChargeDistribution->yAxis->pixelToCoord(ui->PlotChargeDistribution->height()));
    ui->PlotChargeDistribution->graph(2)->setData(xData, yData);
    xData.clear();
    yData.clear();
    xData.append(ui->PlotChargeDistribution->xAxis->pixelToCoord(0));
    yData.append(ui->PlotChargeDistribution->yAxis->pixelToCoord(Cursor.y()));
    xData.append(ui->PlotChargeDistribution->xAxis->pixelToCoord(ui->PlotChargeDistribution->width()));
    yData.append(ui->PlotChargeDistribution->yAxis->pixelToCoord(Cursor.y()));
    ui->PlotChargeDistribution->graph(3)->addData(xData, yData);
    double coord_x = ui->PlotChargeDistribution->xAxis->pixelToCoord(Cursor.x());
    double coord_y = ui->PlotChargeDistribution->yAxis->pixelToCoord(Cursor.y());
    CoordText->setText(QString("(%1, %2)").arg(GetFormattedNumber(coord_x)).arg(GetFormattedNumber(coord_y)));
    CoordText->position->setCoords(QPointF( ui->PlotChargeDistribution->xAxis->pixelToCoord(Cursor.x() + 50),  ui->PlotChargeDistribution->yAxis->pixelToCoord(Cursor.y()-10)));
    CoordText->setFont(QFont(font().family(), 10));
    ui->PlotChargeDistribution->replot();
}

void Dialog_charge::MouseMovePlotChargeDistribution(QMouseEvent *event)
{
    DrawCrossAndCoord(event->pos());
}

void Dialog_charge::MouseWheelPlotChargeDistribution(QWheelEvent *event)
{
    DrawCrossAndCoord(event->position().toPoint());
}

std::string Dialog_charge::GetFormattedNumber(double Value)
{
    if(abs(Value) > Sci_times_notation)
    {
        return std::string(std::format("{:.{}E}", Value, Decimal_points_numbers));
    }
    else
    {
        return std::string(std::format("{:.{}f}", Value, Decimal_points_numbers));
    }
}
