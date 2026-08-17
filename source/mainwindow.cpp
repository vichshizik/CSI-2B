#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "splinetwobody_eigen.h"
#include "dialog_settings.h"
#include "dialog_result.h"
#include "dialog_savefile.h"
#include "dialog_charge.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Settings_Window = new Dialog_settings(this);
    Program_settings_path = qApp->applicationDirPath() + "/config.ini";

    //Description of the variables
    ui->Rmin_Label->setToolTip("The lower limit of the calculation mesh");
    ui->Rmax_Label->setToolTip("The upper limit of the calculation mesh");
    ui->Step_Label->setToolTip("The step of the calculation mesh");
    ui->StepSpline_Label->setToolTip("The step of spline interpolation.\n"
                                     "It must be less than the step of the mesh");
    ui->l_Label->setToolTip("The orbital quantum number");
    ui->m1_Label->setToolTip("The mass of the first particle");
    ui->m2_Label->setToolTip("The mass of the second particle");
    ui->Radius1_Label->setToolTip("The rms charge radii of the first particle");
    ui->Radius2_Label->setToolTip("The rms charge radii of the second particle");
    ui->m2_Label->setToolTip("The mass of the second particle");
    ui->Units_Label->setToolTip("Units selection for output results\n");
    ui->Potential_Label->setToolTip("Potential input.\n"
                                    "* - for multiplication.\n"
                                    "^ - for raising a number to a power.\n"
                                    "/ - for division.\n"
                                    "+/- - for addition/substraction.\n"
                                    "It also supports functions like erf(), abs(), sin(), cos() etc.");


    //Units select add types
    ui->UnitsType_Select->addItem("GeV|fm");
    ui->UnitsType_Select->addItem("Atomic units");

    ui->Calc_settings_Radius1_Box->setEnabled(false);
    ui->Calc_settings_Radius1_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: gray; color: black;}");
    ui->Calc_settings_Radius2_Box->setEnabled(false);
    ui->Calc_settings_Radius2_Box->setStyleSheet("QLineEdit { border: 1px solid; border-color: rgb(0, 0, 0); border-radius: 2px; background-color: gray; color: black;}");

    connect(Settings_Window, &Dialog_settings::transfer_Visual_R_min_Box_textEdited, this, &MainWindow::receive_Visual_R_min_Box_textEdited);
    connect(Settings_Window, &Dialog_settings::transfer_Visual_R_max_Box_textEdited, this, &MainWindow::receive_Visual_R_max_Box_textEdited);
    connect(Settings_Window, &Dialog_settings::transfer_Visual_Step_Box_textEdited, this, &MainWindow::receive_Visual_Step_Box_textEdited);
    connect(Settings_Window, &Dialog_settings::transfer_Decimal_point_Box_valueChanged, this, &MainWindow::receive_Decimal_point_Box_valueChanged);
    connect(Settings_Window, &Dialog_settings::transfer_EnergyLimit_Box_textEdited, this, &MainWindow::receive_EnergyLimit_Box_textEdited);
    connect(Settings_Window, &Dialog_settings::transfer_Switch_EnergyLimit_stateChanged, this, &MainWindow::receive_Switch_EnergyLimit_stateChanged);
    connect(ui->PlotPotential, SIGNAL(mouseMove(QMouseEvent*)), SLOT(MouseMovePlotPotential(QMouseEvent*)));
    connect(ui->PlotPotential, SIGNAL(mouseWheel(QWheelEvent*)),  SLOT(MouseWheelPlotPotential(QWheelEvent*)));


    //Create and fill Success flags for LineEdit boxes
    Calc_settings_success.resize(9);
    Calc_settings_success.fill(true);
    Visual_settings_success.resize(4);
    Visual_settings_success.fill(true);
    Potential_success = true;
    EnergyLimit_Enabled = false;
    Charge_Enabled = false;


    //Set to zero progress bar
    ui->progressBar->setValue(0);

    //Settings Window
    Visual_R_max_d = Settings_Window->get_Visual_R_max_Box()->text().toDouble();
    Visual_R_min_d = Settings_Window->get_Visual_R_min_Box()->text().toDouble();
    Visual_Step_d = Settings_Window->get_Visual_Step_Box()->text().toDouble();
    Decimal_points_numbers = Settings_Window->get_Decimal_point_Box();
    Sci_times_notation = 1e4;

    //Settings for PlotPotential
    Set_Graph_Style(ui->PlotPotential);
    ui->PlotPotential->addGraph();
    ui->PlotPotential->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->PlotPotential->graph(0)->setLineStyle(QCPGraph::LineStyle(QCPCurve::lsLine));
    ui->PlotPotential->graph(0)->setPen(QPen(Qt::blue, 1.5));
    ui->PlotPotential->xAxis->setLabelFont(QFont("Segoe UI", 12));
    ui->PlotPotential->yAxis->setLabelFont(QFont("Segoe UI", 12));
    ui->PlotPotential->xAxis->setLabel("r");
    ui->PlotPotential->xAxis->setLabelColor(Qt::black);
    ui->PlotPotential->yAxis->setLabel("U(r)");
    ui->PlotPotential->yAxis->setLabelColor(Qt::black);
    ui->PlotPotential->rescaleAxes();

    Potential_Equation_Parser.settings().disable_all_control_structures();
    Potential_Equation_Parser.settings().disable_all_logic_ops();
    Potential_Equation_Table.add_variable("r", Potential_Equation_Parse_R);
    Potential_Equation_Expression.register_symbol_table(Potential_Equation_Table);


    //Settings for display step points
    ui->PlotPotential->addGraph();
    ui->PlotPotential->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->PlotPotential->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->PlotPotential->graph(1)->setPen(QPen(Qt::red, 1.5, Qt::SolidLine));


    //Settings for display Coordinates
    ui->PlotPotential->addGraph();
    ui->PlotPotential->addGraph();
    ui->PlotPotential->graph(2)->setLineStyle(QCPGraph::LineStyle(QCPCurve::lsLine));
    ui->PlotPotential->graph(2)->setPen(QPen(Qt::blue, 0.5, Qt::DashLine));
    ui->PlotPotential->graph(3)->setLineStyle(QCPGraph::LineStyle(QCPCurve::lsLine));
    ui->PlotPotential->graph(3)->setPen(QPen(Qt::blue, 0.5, Qt::DashLine));
    CoordText = new QCPItemText(ui->PlotPotential);
    CoordText->setText(QString("(%1, %2)").arg(0).arg(0));
    CoordText->position->setCoords(QPointF(ui->PlotPotential->xAxis->pixelToCoord(50), ui->PlotPotential->yAxis->pixelToCoord(-10)));
    CoordText->setFont(QFont(font().family(), 10));

    LoadSettings(Program_settings_path);
    Calc_Settings_textEdited();
    Visual_Settings_textEdited();
    Potential_Equation_Parse();

    //Calculate needed RAM (max)
    CalculateRAMNeeded();

    //Display calc mesh
    DrawMesh();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Settings_Window;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton ExitMessageBox = QMessageBox::question( this, "Exiting the app",
                                                               tr("Are you sure?\n"),
                                                               QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    switch (ExitMessageBox){
        case(QMessageBox::Yes):
            SaveSettings(Program_settings_path);
            event->accept();
        break;
        case(QMessageBox::No):
            event->ignore();
            break;
        default:
            event->accept();
            break;
        }
}
