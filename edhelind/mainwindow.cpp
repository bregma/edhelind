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
#include "edhelind_config.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::
MainWindow(QString const& file_name, QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->set_current_file(file_name);
}


MainWindow::
~MainWindow()
{
    delete ui;
}


void MainWindow::
on_action_open_triggered()
{
    this->set_current_file(QFileDialog::getOpenFileName(this));
}


void MainWindow::
on_action_about_triggered()
{
   QMessageBox::about(this,
                      tr("About edhelind"),
                      tr("An ELF file explorer\n\nVersion %1")
                        .arg(EDHELIND_VERSION));
}


void MainWindow::
on_action_exit_triggered()
{
    QApplication::closeAllWindows();
}


void MainWindow::
set_current_file(QString const& file_name)
{
    if (file_name.isEmpty()) {
        return;
    }
}

