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

#include "libedhel/elffile.h"
#include "libedhel/section.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>


MainWindow::
MainWindow(QString const& file_name, QWidget *parent)
: QMainWindow{parent}
, ui_{std::make_unique<Ui::MainWindow>()}
, tree_model_(new QStandardItemModel(this))
{
    ui_->setupUi(this);
    this->set_current_file(file_name);
    tree_model_->setHorizontalHeaderLabels(QStringList{tr("Field"),
                                                       tr("Value"),
                                                       tr("Description")});
}


MainWindow::
~MainWindow()
{
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
    tree_model_->clear();
    if (file_name.isEmpty()) {
        return;
    }

    try
    {
        elf_file_ = std::make_unique<ElfFile>(file_name.toStdString());
    } catch (std::exception &ex) {
        QMessageBox::critical(this,
                              tr("Error opening %1").arg(file_name),
                              QString(ex.what()));
        return;
    }

    QStandardItem* root = tree_model_->invisibleRootItem();
    QList<QStandardItem*> file_row{this->prepare_row(file_name, "")};

    root->appendRow(file_row);
    file_row.first()->appendRow(this->display_elf_header());
    file_row.first()->appendRow(this->display_sections());
    ui_->tree_view_->setModel(tree_model_);
    ui_->tree_view_->expandAll();
    ui_->tree_view_->resizeColumnToContents(0);
    ui_->tree_view_->resizeColumnToContents(1);
    ui_->tree_view_->expandToDepth(1);
}


QStandardItem* MainWindow::
display_elf_header() const
{
    ElfHeader const& elf_header = elf_file_->elf_header();
    QStandardItem* header = new QStandardItem("ELF Header");
    header->appendRow(prepare_row("eh_type:", QString::fromStdString(elf_header.type_string())));
    header->appendRow(prepare_row("e_entry:", QString("0x%1").arg(elf_header.entry(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_phoff:", QString("0x%1").arg(elf_header.phoff(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_shoff:", QString("0x%1").arg(elf_header.shoff(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_flags:", QString("0x%1").arg(elf_header.flags(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_ehsize:", QString("0x%1").arg(elf_header.ehsize(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_phentsize:", QString("0x%1").arg(elf_header.phentsize(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_phnum:", QString("0x%1").arg(elf_header.phnum(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_shentsize:", QString("0x%1").arg(elf_header.shentsize(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_shnum:", QString("0x%1").arg(elf_header.shnum(), 8, 16, QChar('0'))));
    header->appendRow(prepare_row("e_shstrndx:", QString("0x%1").arg(elf_header.shstrndx(), 8, 16, QChar('0'))));
    return header;
}


QStandardItem* MainWindow::
display_sections() const
{
    QStandardItem* sections = new QStandardItem("Sections");
    elf_file_->section_table().iterate_sections([&](Section const& section){
            QStandardItem* sec = new QStandardItem(QString::fromStdString(section.name_string()));
            sec->appendRow(this->prepare_row("sh_type:", QString::fromStdString(section.type_string())));
            sec->appendRow(this->prepare_row("sh_flags:", QString("0x%1").arg(section.flags(), 8, 16, QChar('0'))));
            sec->appendRow(this->prepare_row("sh_addr:", QString("0x%1").arg(section.addr(), 8, 16, QChar('0'))));
            sec->appendRow(this->prepare_row("sh_offset:", QString("0x%1").arg(section.offset(), 8, 16, QChar('0'))));
            sec->appendRow(this->prepare_row("sh_size:", QString("0x%1").arg(section.size(), 8, 10)));
            sec->appendRow(this->prepare_row("sh_link:", QString("0x%1").arg(section.link(), 8, 16, QChar('0'))));
            sec->appendRow(this->prepare_row("sh_info:", QString("0x%1").arg(section.info(), 8, 16, QChar('0'))));

            sections->appendRow(sec);
       });
    return sections;
}


QList<QStandardItem*> MainWindow::
prepare_row(QString const& label, QString const& value) const
{
    return {new QStandardItem(label), new QStandardItem(value)};
}

