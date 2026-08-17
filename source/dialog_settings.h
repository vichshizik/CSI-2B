#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include "qlineedit.h"
#include <QDialog>

namespace Ui {
class Dialog_settings;
}

class Dialog_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_settings(QWidget *parent = nullptr);
    ~Dialog_settings();

private slots:
    void on_Visual_R_min_Box_textEdited(const QString &arg1);

    void on_Visual_R_max_Box_textEdited(const QString &arg1);

    void on_Visual_Step_Box_textEdited(const QString &arg1);

    void on_Decimal_point_Box_valueChanged(int arg1);

    void on_EnergyLimit_Box_textEdited(const QString &arg1);

    void on_Switch_EnergyLimit_stateChanged(int arg1);

private:
    Ui::Dialog_settings *ui;


public:
    QLineEdit* get_Visual_R_min_Box();

    QLineEdit* get_Visual_R_max_Box();

    QLineEdit* get_Visual_Step_Box();

    QLineEdit* get_Potential_EnergyLimit_Box();

    int get_Decimal_point_Box();

    bool get_Switch_EnergyLimit();

    void set_Decimal_point_Box(int Decimal_points_numbers);

    void set_Visual_R_min_Box(double Visual_R_min_d);

    void set_Visual_R_max_Box(double Visual_R_max_d);

    void set_Visual_Step_Box(double Visual_Step_d);

    void set_Potential_EnergyLimit_Box(double EnergyLimit_d);

    void set_Switch_EnergyLimit(bool EnergyLimit_Enabled);

signals:
    void transfer_Visual_R_min_Box_textEdited(QString, QLineEdit*);

    void transfer_Visual_R_max_Box_textEdited(QString, QLineEdit*);

    void transfer_Visual_Step_Box_textEdited(QString, QLineEdit*);

    void transfer_EnergyLimit_Box_textEdited(QString, QLineEdit*);

    void transfer_Decimal_point_Box_valueChanged(int arg1);

    void transfer_Switch_EnergyLimit_stateChanged(int arg1);

};

#endif // DIALOG_SETTINGS_H
