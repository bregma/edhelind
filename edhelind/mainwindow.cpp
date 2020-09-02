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
#include "libedhel/section_note.h"
#include "libedhel/segment.h"
#include "libedhel/segment_interp.h"
#include "libedhel/segment_note.h"
#include "mainwindow.h"
#include <sstream>
#include "ui_mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>
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

    connect(ui_->tree_view_->selectionModel(), SIGNAL(currentChanged(QModelIndex const&, QModelIndex const&)),
            this, SLOT(on_current_changed(QModelIndex const&, QModelIndex const&)));
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
on_current_changed(QModelIndex const& current, QModelIndex const&)
{
    QVariant v = tree_model_->data(current, Qt::UserRole+1);
    if (v.isValid() != true)
    {
        ui_->text_view_->clear();
    }
    else
    {
        Detailable const* displayable = static_cast<Detailable const*>(v.value<void*>());
        std::ostringstream ostr ;
        ostr << *displayable;
        ui_->text_view_->setPlainText(QString::fromStdString(ostr.str()));
    }
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
    file_row.first()->appendRow(this->display_segments());
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
    header->appendRow(prepare_row("e_ehsize:", QString("%1").arg(elf_header.ehsize(), 8, 10)));
    header->appendRow(prepare_row("e_phentsize:", QString("%1").arg(elf_header.phentsize(), 8, 10)));
    header->appendRow(prepare_row("e_phnum:", QString("%1").arg(elf_header.phnum(), 8, 10)));
    header->appendRow(prepare_row("e_shentsize:", QString("%1").arg(elf_header.shentsize(), 8, 10)));
    header->appendRow(prepare_row("e_shnum:", QString("%1").arg(elf_header.shnum(), 8, 10)));
    header->appendRow(prepare_row("e_shstrndx:", QString("%1").arg(elf_header.shstrndx(), 8, 10)));
    header->setData(QVariant::fromValue((void*)&elf_header), Qt::UserRole+1);
    return header;
}


QStandardItem* MainWindow::
display_sections() const
{
    QStandardItem* sections = new QStandardItem("Sections");
    elf_file_->section_table().iterate_sections([&](Section const& section){
            QStandardItem* sec = new QStandardItem(QString::fromStdString(section.name_string()));
            sec->appendRow(this->prepare_row("sh_type:", QString::fromStdString(section.type_string())));
            sec->appendRow(this->prepare_row("sh_flags:", QString::fromStdString(section.flags_string())));
            sec->appendRow(this->prepare_row("sh_addr:", QString("0x%1").arg(section.addr(), 8, 16, QChar('0'))));
            sec->appendRow(this->prepare_row("sh_offset:", QString("0x%1").arg(section.offset(), 8, 16, QChar('0'))));
            sec->appendRow(this->prepare_row("sh_size:", QString("%1").arg(section.size(), 8, 10)));
            sec->appendRow(this->prepare_row("sh_link:", QString("0x%1").arg(section.link(), 8, 16, QChar('0'))));
            sec->appendRow(this->prepare_row("sh_info:", QString("0x%1").arg(section.info(), 8, 16, QChar('0'))));

            switch (section.type()) {
                case SType::SHT_NOTE: {
                    auto const& pt_note = static_cast<Section_NOTE const&>(section);
                    pt_note.iterate_notes([&](Note const& note){
                            QStandardItem* note_row = new QStandardItem(QString("NOTE %1 %2")
                                                          .arg(QString::fromStdString(note.name_))
                                                          .arg(note.type_, 8, 10));
                            note_row->appendRow(this->prepare_row("name:", QString::fromStdString(note.name_)));
                            note_row->appendRow(this->prepare_row("type:", QString("%1").arg(note.type_, 8, 10)));
                            note_row->setData(QVariant::fromValue((void*)&note), Qt::UserRole+1);
                            sec->appendRow(note_row);
                        });
                    break;
                }
                default:
                    break;
            }
            sec->setData(QVariant::fromValue((void*)&section), Qt::UserRole+1);
            sections->appendRow(sec);
       });
    return sections;
}


QStandardItem* MainWindow::
display_segments() const
{
    QStandardItem* segments = new QStandardItem("Segments");
    elf_file_->segment_table().iterate_segments([&](Segment const& segment){
            QStandardItem* seg = new QStandardItem(QString::fromStdString(segment.type_string()));
            seg->appendRow(this->prepare_row("p_flags:", QString::fromStdString(segment.flags_string())));
            seg->appendRow(this->prepare_row("p_offset:", QString("0x%1").arg(segment.offset(), 8, 16, QChar('0'))));
            seg->appendRow(this->prepare_row("p_vaddr:", QString("0x%1").arg(segment.vaddr(), 8, 16, QChar('0'))));
            seg->appendRow(this->prepare_row("p_paddr:", QString("0x%1").arg(segment.paddr(), 8, 16, QChar('0'))));
            seg->appendRow(this->prepare_row("p_filesz:", QString("%1").arg(segment.filesz(), 8, 10)));
            seg->appendRow(this->prepare_row("p_memsz:", QString("%1").arg(segment.memsz(), 8, 10)));
            seg->appendRow(this->prepare_row("p_align:", QString("%1").arg(segment.align(), 8, 10)));
            switch (segment.type()) {
                case PType::PT_INTERP: {
                    auto const& pt_interp = static_cast<Segment_INTERP const&>(segment);
                    seg->appendRow(this->prepare_row("interp:", QString::fromStdString(pt_interp.interp())));
                    break;
                }
                case PType::PT_NOTE: {
                    auto const& pt_note = static_cast<Segment_NOTE const&>(segment);
                    pt_note.iterate_notes([&](Note const& note){
                            QStandardItem* note_row = new QStandardItem(QString("NOTE %1 %2")
                                                          .arg(QString::fromStdString(note.name_))
                                                          .arg(note.type_, 8, 10));
                            note_row->appendRow(this->prepare_row("name:", QString::fromStdString(note.name_)));
                            note_row->appendRow(this->prepare_row("type:", QString("%1").arg(note.type_, 8, 10)));
                            note_row->setData(QVariant::fromValue((void*)&note), Qt::UserRole+1);
                            seg->appendRow(note_row);
                        });
                    break;
                }
                default:
                    break;
            }
            seg->setData(QVariant::fromValue((void*)&segment), Qt::UserRole+1);
            segments->appendRow(seg);
       });
    return segments;
}


QList<QStandardItem*> MainWindow::
prepare_row(QString const& label, QString const& value) const
{
    return {new QStandardItem(label), new QStandardItem(value)};
}

