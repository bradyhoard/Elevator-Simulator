#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "button.h"
#include "ecs.h"
#include "elevator.h"
#include <QQueue>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);


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
    //set the minimum for the spin box
    ui->elevators_num->setMinimum(1);
    ui->floors_num->setMinimum(1);


    connect(ui->initiateButton, &QPushButton::clicked, [=]() {

        //this initates all the values for the ui

        QList<Elevator*> *elevators = new QList<Elevator*>();
        QList<Floor*> *floors = new QList<Floor*>();

        int elevs_num = ui->elevators_num->value();
        int floors_num = ui->floors_num->value();

        //add the floors numbers to the combo box and the passengers cab
        for (int i = 1; i <= elevs_num; ++i) {
            ui->carComboBox->addItem(QString::number(i));
             ui->passengers_cab->addItem(QString::number(i));
        }
        ui->textBrowser->append(QString::number(elevs_num) + " elevators added");


        //add the floors numbers to the combo box
        for (int i = 0; i < floors_num; ++i) {
            ui->floorComboBox->addItem(QString::number(i));
            ui->carFloorComboBox->addItem(QString::number(i));
        }

        ui->textBrowser->append(QString::number(floors_num) + " floors added");
        ui->textBrowser->append(QString::number(floors_num) +  " floors added to elevator panel");

        //add the passengers to the combo box
        QList<int> passengers = {0,1,2,3,4,5,6};
        for (int i = 0; i < passengers.count(); ++i) {
            ui->passengers_on->addItem(QString::number(passengers[i]));
            ui->passengers_off->addItem(QString::number(passengers[i]));
        }

       //all the floors added to the ECS
       for(int i = 0; i < floors_num; i++) {
           Floor* floor = new Floor(ui->textBrowser, i, this);
           floors->push_back(floor);
       }

      //all the elevators added to the ECS
      for(int i = 1; i <= elevs_num; i++) {
          QQueue<int> floor_q;
          Elevator* elevator = new Elevator(ui->textBrowser, true , "Stopped", 0 ,0, i,floor_q, this);
          elevators->push_back(elevator);
      }
       ui->textBrowser->append("-----------------------------");

       //initalize the ECS
        ecs = new ECS(ui->textBrowser, elevators, *floors, this);

        //disbale initiate and other inputs used to set up the ui
        ui->initiateButton->setEnabled(false);
        ui->elevators_num->setEnabled(false);
        ui->floors_num->setEnabled(false);

        //enable all the other buttons
        ui->PowerOutageButton->setEnabled(true);
        ui->FireButton->setEnabled(true);
        ui->DoorObstaclesButton->setEnabled(true);
        //this is done for a reason , the way to "activate" this button is by loading too many people onto the elevator
        //ui->OverLoadButton->setEnabled(true);
        ui->HelpButton->setEnabled(true);
        ui->GoButton->setEnabled(true);
        ui->floorUp->setEnabled(true);
        ui->floorDown->setEnabled(true);
        ui->ElevatorStatusButton->setEnabled(true);
    });

    //floor up button onClick event function
    connect(ui->floorUp, &QPushButton::clicked, this, [=]() {
            int floor_number = ui->floorComboBox->currentText().toInt();
            ecs->find_elevator(ui->passengers_on , ui->passengers_off ,ui->PassengersConfirm , ui->passengers_cab,  floor_number , "Up");
        });

    //floor down button onClick event function
    connect(ui->floorDown, &QPushButton::clicked, this, [=]() {
            int floor_number = ui->floorComboBox->currentText().toInt();
            ecs->find_elevator(ui->passengers_on , ui->passengers_off ,ui->PassengersConfirm , ui->passengers_cab , floor_number , "Down");
        });


    //go button onClick event function
    connect(ui->GoButton, &QPushButton::clicked, this, [=]() {
            int cab = ui->carComboBox->currentText().toInt() -1;
            int floor = ui->carFloorComboBox->currentText().toInt();

            // Create a new thread and move the elevator in that thread
            QThread* thread = new QThread();
            connect(thread, &QThread::started, ecs, [=]() {
                ecs->move_elevator(ui->passengers_on , ui->passengers_off , ui->PassengersConfirm ,ui->passengers_cab , cab , floor);
                thread->quit();
            });

            // When the thread is finished, delete it
            connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            thread->start();
        });

    //elevator status onClick event function
    connect(ui->ElevatorStatusButton, &QPushButton::clicked, this, [=]() {
            int cab = ui->carComboBox->currentText().toInt() -1;
            QList<Elevator*> elevators = ecs->get_elevators();
            Elevator* elevator = elevators[cab];
            elevator->status();
        });

    //emergency - fire -  onClick event function
    connect(ui->FireButton, &QPushButton::clicked, this, [=]() {
            QString em = "fire!";
            ecs->emergency(em , ui->PassengersConfirm);
        });

    //emergency - poweroutage -  onClick event function
    connect(ui->PowerOutageButton, &QPushButton::clicked, this, [=]() {
            QString em = "power outage!";
            ecs->emergency(em, ui->PassengersConfirm);
        });
    //emergency - door obstacles -  onClick event function
    connect(ui->DoorObstaclesButton, &QPushButton::clicked, this, [=]() {
            QString em = "door obstacles!";
            int cab = ui->carComboBox->currentText().toInt();
            ecs->emergency(em, ui->PassengersConfirm , cab);
        });

    //this function is non clickable shown in the group of emergnecy buttons
    //emergency - overload  -  onClick event function
    connect(ui->OverLoadButton, &QPushButton::clicked, this, [=]() {
            QString em = "overload!";
            ecs->emergency(em, ui->PassengersConfirm);
        });

    //emergency - help  -  onClick event function
    connect(ui->HelpButton, &QPushButton::clicked, this, [=]() {
            int cab = ui->carComboBox->currentText().toInt();
            ecs->help(cab);
        });

}



MainWindow::~MainWindow()
{
    delete ui;
}



