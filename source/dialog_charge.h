#ifndef DIALOG_CHARGE_H
#define DIALOG_CHARGE_H

#include <QWidget>
#include "qcustomplot.h"
namespace Ui {
class Dialog_charge;
}

class Dialog_charge : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog_charge(int Decimal_points_numbers, int Sci_times_notation, QWidget *parent = nullptr);
    ~Dialog_charge();

private slots:
    void on_ChargeDistribution_Equation_Box_textEdited(const QString &arg1);

    void MouseMovePlotChargeDistribution(QMouseEvent *event);

    void MouseWheelPlotChargeDistribution(QWheelEvent *event);

private:
    Ui::Dialog_charge *ui;
    QCPItemText *CoordText;
    int Decimal_points_numbers;
    int Sci_times_notation;

    void DrawCrossAndCoord(QPoint Cursor);
    void ChargeDistribution_Parse();
    std::string GetFormattedNumber(double Value);
};

#endif // DIALOG_CHARGE_H
