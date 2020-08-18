/**
 * Main program entry point
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

#include "edhelind/mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>


int
main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(EDHELIND_PROJECT_NAME);
    QCoreApplication::setApplicationName(EDHELIND_PROJECT_NAME);
    QCoreApplication::setApplicationVersion(EDHELIND_VERSION);

    QCommandLineParser cli_parser;
    cli_parser.setApplicationDescription(QCoreApplication::applicationName());
    cli_parser.addHelpOption();
    cli_parser.addVersionOption();
    cli_parser.addPositionalArgument("FILE", "the ELF file to open");
    cli_parser.process(app);

    MainWindow main_window(cli_parser.positionalArguments().isEmpty() ? ""
                           : cli_parser.positionalArguments().first());
    main_window.show();
    return app.exec();
}
