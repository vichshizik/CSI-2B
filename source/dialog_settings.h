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
    void on_Potential_R_min_Box_textChanged(const QString &arg1);

    void on_Potential_R_max_Box_textChanged(const QString &arg1);

    void on_Potential_Step_Box_textChanged(const QString &arg1);

    void on_Decimal_point_Box_valueChanged(int arg1);

    void on_EnergyLimit_Box_textChanged(const QString &arg1);

    void on_Switch_EnergyLimit_stateChanged(int arg1);

private:
    Ui::Dialog_settings *ui;


public:
    QLineEdit* get_Potential_R_min_Box();

    QLineEdit* get_Potential_R_max_Box();

    QLineEdit* get_Potential_Step_Box();

    QLineEdit* get_Potential_EnergyLimit_Box();

    int get_Decimal_point_Box();

signals:
    void transfer_Potential_R_min_Box_textChanged(QString, QLineEdit*);

    void transfer_Potential_R_max_Box_textChanged(QString, QLineEdit*);

    void transfer_Potential_Step_Box_textChanged(QString, QLineEdit*);

    void transfer_EnergyLimit_Box_textChanged(QString, QLineEdit*);

    void transfer_Decimal_point_Box_valueChanged(int arg1);

    void transfer_Switch_EnergyLimit_stateChanged(int arg1);
};

#endif // DIALOG_SETTINGS_H
