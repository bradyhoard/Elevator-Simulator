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
    QList<Elevator*> *elevators = new QList<Elevator*>();
    QList<Floor*> *floors = new QList<Floor*>();

    /*
    for (int i = 1; i < 5; ++i) {
        Elevator* elevator = new Elevator(ui->textBrowser, false , 0 , i + 1000, this);
        elevators->push_back(elevator);
    }

    */

    //all the floors added to the
    Floor* floor0 = new Floor(ui->textBrowser, 0, this);
    Floor* floor1 = new Floor(ui->textBrowser, 1, this);
    Floor* floor2 = new Floor(ui->textBrowser, 2, this);
    Floor* floor3 = new Floor(ui->textBrowser, 3, this);
    Floor* floor4 = new Floor(ui->textBrowser, 4, this);
    floors->push_back(floor0);
    floors->push_back(floor1);
    floors->push_back(floor2);
    floors->push_back(floor3);
    floors->push_back(floor4);



    //all the elevators added to the ECS
    Elevator* elevator1 = new Elevator(ui->textBrowser, true , "Stopped", 0 ,0, 1, this);
    Elevator* elevator2 = new Elevator(ui->textBrowser, false , "Stopped", 0 ,0, 2, this);
    elevators->push_back(elevator1);
    elevators->push_back(elevator2);

    ECS* ecs = new ECS(ui->textBrowser, elevators, *floors, this);

    //disable all buttons until initaite is called
    ui->PowerOutageButton->setEnabled(false);
    ui->FireButton->setEnabled(false);
    ui->DoorObstaclesButton->setEnabled(false);
    ui->OverLoadButton->setEnabled(false);
    ui->HelpButton->setEnabled(false);
    ui->GoButton->setEnabled(false);
    ui->floorUp->setEnabled(false);
    ui->floorDown->setEnabled(false);
    ui->ElevatorStatusButton->setEnabled(false);
    ui->PassengersConfirm->setEnabled(false);


    connect(ui->initiateButton, &QPushButton::clicked, this, &MainWindow::initiate);

    connect(ui->floorUp, &QPushButton::clicked, this, [=]() {
            int floor_number = ui->floorComboBox->currentText().toInt();
            ecs->find_elevator(ui->passengers_on , ui->passengers_off ,ui->PassengersConfirm , floor_number , "Up");
        });

    connect(ui->floorDown, &QPushButton::clicked, this, [=]() {
            int floor_number = ui->floorComboBox->currentText().toInt();
            ecs->find_elevator(ui->passengers_on , ui->passengers_off ,ui->PassengersConfirm ,floor_number , "Down");
        });



    connect(ui->GoButton, &QPushButton::clicked, this, [=]() {
            int cab = ui->carComboBox->currentText().toInt() -1;
            int floor = ui->carFloorComboBox->currentText().toInt();
            ecs->move_elevator(ui->passengers_on , ui->passengers_off , ui->PassengersConfirm , cab , floor);
        });

    connect(ui->ElevatorStatusButton, &QPushButton::clicked, this, [=]() {
            int cab = ui->carComboBox->currentText().toInt() -1;
            Elevator *elevator = elevators->at(cab);
            elevator->status();
        });


    //connect(ui->FireButton, SIGNAL(clicked()), ecs, SLOT(emergency(string)));

    connect(ui->FireButton, &QPushButton::clicked, this, [=]() {
            QString em = "fire!";
            ecs->emergency(em);
        });

    connect(ui->PowerOutageButton, &QPushButton::clicked, this, [=]() {
            QString em = "power outage!";
            ecs->emergency(em);
        });

    connect(ui->DoorObstaclesButton, &QPushButton::clicked, this, [=]() {
            QString em = "door obstacle!";
            ecs->emergency(em);
        });

    connect(ui->OverLoadButton, &QPushButton::clicked, this, [=]() {
            QString em = "overload!";
            ecs->emergency(em);
        });

    connect(ui->HelpButton, &QPushButton::clicked, this, [=]() {
            int cab = ui->carComboBox->currentText().toInt();
            ecs->help(cab);
        });




}

void MainWindow::initiate()
{
    //add the floors numbers to the combo box
    QList<int> elevators = {1, 2};
    for (int i = 0; i < elevators.count(); ++i) {
        ui->carComboBox->addItem(QString::number(elevators[i]));
    }
    ui->textBrowser->append("2 elevators added");


    //add the floors numbers to the combo box
    QList<int> floors = {0,1, 2, 3,4};
    for (int i = 0; i < floors.count(); ++i) {
        ui->floorComboBox->addItem(QString::number(floors[i]));
        ui->carFloorComboBox->addItem(QString::number(floors[i]));
    }

    ui->textBrowser->append("4 floors added");
    ui->textBrowser->append("4 floors added to elevator panel");

    QList<int> passengers = {0,1,2,3};
    for (int i = 0; i < passengers.count(); ++i) {
        ui->passengers_on->addItem(QString::number(passengers[i]));
        ui->passengers_off->addItem(QString::number(passengers[i]));
    }

    //disbale initiate
    ui->initiateButton->setEnabled(false);
    //enable all the other buttons
    ui->PowerOutageButton->setEnabled(true);
    ui->FireButton->setEnabled(true);
    ui->DoorObstaclesButton->setEnabled(true);
    ui->OverLoadButton->setEnabled(true);
    ui->HelpButton->setEnabled(true);
    ui->GoButton->setEnabled(true);
    ui->floorUp->setEnabled(true);
    ui->floorDown->setEnabled(true);
    ui->ElevatorStatusButton->setEnabled(true);

}


MainWindow::~MainWindow()
{
    delete ui;
}



