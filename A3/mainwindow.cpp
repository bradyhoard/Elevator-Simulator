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
    Floor* floor5 = new Floor(ui->textBrowser, 5, this);
    Floor* floor6 = new Floor(ui->textBrowser, 6, this);
    floors->push_back(floor0);
    floors->push_back(floor1);
    floors->push_back(floor2);
    floors->push_back(floor3);
    floors->push_back(floor4);
    floors->push_back(floor5);
    floors->push_back(floor6);



    //all the elevators added to the ECS
    Elevator* elevator1 = new Elevator(ui->textBrowser, true , "Stopped", 0 ,0, 1, this);
    Elevator* elevator2 = new Elevator(ui->textBrowser, true , "Stopped", 0 ,0, 2, this);
    Elevator* elevator3 = new Elevator(ui->textBrowser, true , "Stopped", 0 ,0, 3, this);
    Elevator* elevator4 = new Elevator(ui->textBrowser, true , "Stopped", 0 ,0, 4, this);
    elevators->push_back(elevator1);
    elevators->push_back(elevator2);
    elevators->push_back(elevator3);
    elevators->push_back(elevator4);

    ECS* ecs = new ECS(ui->textBrowser, elevators, *floors, this);

    //disable all buttons until initaite is called
    ui->PowerOutageButton->setEnabled(false);
    ui->FireButton->setEnabled(false);
    ui->DoorObstaclesButton->setEnabled(false);
    ui->OverLoadButton->setEnabled(false);
    ui->Help->setEnabled(false);
    ui->GoButton->setEnabled(false);
    ui->floorUp->setEnabled(false);
    ui->floorDown->setEnabled(false);
    ui->ElevatorStatusButton->setEnabled(false);


    connect(ui->initiateButton, &QPushButton::clicked, this, &MainWindow::initiate);

    connect(ui->floorUp, &QPushButton::clicked, this, [=]() {
            int floor_number = ui->floorComboBox->currentText().toInt();
            ecs->find_elevator(floor_number , "Up");
        });

    connect(ui->floorDown, &QPushButton::clicked, this, [=]() {
            int floor_number = ui->floorComboBox->currentText().toInt();
            ecs->find_elevator(floor_number , "Down");
        });



    connect(ui->GoButton, &QPushButton::clicked, this, [=]() {
            int car = ui->carComboBox->currentText().toInt() -1;
            int floor = ui->carFloorComboBox->currentText().toInt();
            Elevator *elevator = elevators->at(car);
            elevator->move(floor);
        });

    connect(ui->ElevatorStatusButton, &QPushButton::clicked, this, [=]() {
            int car = ui->carComboBox->currentText().toInt() -1;
            Elevator *elevator = elevators->at(car);
            elevator->status();
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
    ui->ElevatorStatusButton->setEnabled(true);

}


MainWindow::~MainWindow()
{
    delete ui;
}


