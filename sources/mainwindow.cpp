#include "mainwindow.h"
#include <iostream>

void MainWindow::getDevices() {
	for (size_t i = 0; i < 8; ++i) {
		device_list.push_back(new DeviceWidget(QString("Device ") + QString::number(i)));
	}
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	this->setCentralWidget(main_widget);
	main_widget->setLayout(main_grid);
	main_widget->setMinimumSize(750, 300);
	QVBoxLayout* left_bar = new QVBoxLayout;
	main_grid->addLayout(left_bar, 1, 0);
	QPushButton* settings_button = new QPushButton("Settings");	 // Creating settings button.
	settings_button->setFixedWidth(250);
	settings_button->setFixedHeight(60);
	connect(settings_button, SIGNAL(clicked()), this, SLOT(onSettingsButtonPressed()));
	left_bar->addWidget(settings_button);
	QPushButton* scan_network_button = new QPushButton("Scan Network");	 // Creating settings button.
	scan_network_button->setFixedWidth(250);
	scan_network_button->setFixedHeight(60);
	connect(scan_network_button, SIGNAL(clicked()), this, SLOT(onScanNetworkButtonPressed()));
	left_bar->addWidget(scan_network_button);
	QTextEdit* input_box = new QTextEdit("Text box");  // Creating input box.
	left_bar->addWidget(input_box);
	QVBoxLayout* devices_layout = new QVBoxLayout();  // Creating devices layout with scroll area.
	getDevices();
	for (auto& it : device_list) {
		devices_layout->addLayout(it);
	}
	devices_layout->setContentsMargins(0, 0, 0, 0);
	QFrame* devices_layout_container = new QFrame;
	devices_layout_container->setLayout(devices_layout);
	QScrollArea* devices_scroll_area = new QScrollArea();
	devices_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	devices_scroll_area->setAlignment(Qt::AlignRight);
	devices_scroll_area->setFixedWidth(500);
	devices_scroll_area->setWidget(devices_layout_container);
	main_grid->addWidget(devices_scroll_area, 1, 1);

	main_widget->show();
}

void MainWindow::onSettingsButtonPressed() {}

void MainWindow::onScanNetworkButtonPressed() {}
