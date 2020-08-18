/**
 * Main program window
 */
/*
 * Copyright 2020 Stephen M. Webb <stephen.webb@bregmasoft.ca>
 *
 * This file is part of Edhelind.
 *
 * Edhelind is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Edhelind is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Edhelind.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef EDHELIND_MAINWINDOW_H
#define EDHELIND_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow
: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString const& file_name, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void
    on_action_open_triggered();

    void
    on_action_about_triggered();

    void
    on_action_exit_triggered();

private:
    void
    set_current_file(QString const& file_name);

private:
    Ui::MainWindow* ui;
};

#endif /* EDHELIND_MAINWINDOW_H */
