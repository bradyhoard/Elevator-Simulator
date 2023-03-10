#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "button.h"
#include "ecs.h"
#include "elevator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    Button* abutton = new Button(ui->textBrowser, this);
    QList<Elevator*> *elevators = new QList<Elevator*>();

    /*
    for (int i = 1; i < 5; ++i) {
        Elevator* elevator = new Elevator(ui->textBrowser, false , 0 , i + 1000, this);
        elevators->push_back(elevator);
    }

    */
    Elevator* elevator1 = new Elevator(ui->textBrowser, false , 0 , 1001, this);
    Elevator* elevator2 = new Elevator(ui->textBrowser, false , 0 , 1002, this);
    Elevator* elevator3 = new Elevator(ui->textBrowser, false , 0 , 1003, this);
    Elevator* elevator4 = new Elevator(ui->textBrowser, false , 0 , 1004, this);
    elevators->push_back(elevator1);
    elevators->push_back(elevator2);
    elevators->push_back(elevator3);
    elevators->push_back(elevator4);

    ECS* ecs = new ECS(ui->textBrowser, elevators, this);

    //disable all buttons until initaite is called
    ui->PowerOutageButton->setEnabled(false);
    ui->FireButton->setEnabled(false);
    ui->DoorObstaclesButton->setEnabled(false);
    ui->OverLoadButton->setEnabled(false);
    ui->Help->setEnabled(false);
    ui->GoButton->setEnabled(false);
    ui->floorUp->setEnabled(false);
    ui->floorDown->setEnabled(false);


    connect(ui->initiateButton, &QPushButton::clicked, this, &MainWindow::initiate);


    //elevator1->move(2);


    connect(ui->GoButton, &QPushButton::clicked, this, [=]() {
            int car = ui->carComboBox->currentText().toInt() -1;
            int floor = ui->carFloorComboBox->currentText().toInt();
            Elevator *elevator = elevators->at(car);
            elevator->move(floor);
        });


    connect(ui->floorUp, SIGNAL(clicked()), abutton, SLOT(illuminate()));
    connect(ui->floorDown, SIGNAL(clicked()), abutton, SLOT(illuminate()));

    //connect(ui->FireButton, SIGNAL(clicked()), ecs, SLOT(emergency(string)));

    connect(ui->FireButton, &QPushButton::clicked, this, [=]() {
            QString em = "Alert , Fire!";
            ecs->emergency(em);
        });

    connect(ui->PowerOutageButton, &QPushButton::clicked, this, [=]() {
            QString em = "Alert , Power Outage!";
            ecs->emergency(em);
        });

    connect(ui->DoorObstaclesButton, &QPushButton::clicked, this, [=]() {
            QString em = "Alert , Door Obstacles!";
            ecs->emergency(em);
        });

    connect(ui->OverLoadButton, &QPushButton::clicked, this, [=]() {
            QString em = "Alert , Overload!";
            ecs->emergency(em);
        });


}

void MainWindow::initiate()
{
    //add the floors numbers to the combo box
    QList<int> floors = {0,1, 2, 3,4,5,6};
    for (int i = 0; i < floors.count(); ++i) {
        ui->floorComboBox->addItem(QString::number(floors[i]));
        ui->carFloorComboBox->addItem(QString::number(floors[i]));
    }

    //add the floors numbers to the combo box
    QList<int> elevators = {1, 2, 3,4};
    for (int i = 0; i < elevators.count(); ++i) {
        ui->carComboBox->addItem(QString::number(elevators[i]));

    }

    //disbale initiate
    ui->initiateButton->setEnabled(false);
    //enable all the other buttons
    ui->PowerOutageButton->setEnabled(true);
    ui->FireButton->setEnabled(true);
    ui->DoorObstaclesButton->setEnabled(true);
    ui->OverLoadButton->setEnabled(true);
    ui->Help->setEnabled(true);
    ui->GoButton->setEnabled(true);
    ui->floorUp->setEnabled(true);
    ui->floorDown->setEnabled(true);

}


MainWindow::~MainWindow()
{
    delete ui;
}



