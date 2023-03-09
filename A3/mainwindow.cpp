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
    Elevator* elevator = new Elevator(ui->textBrowser, false , 0 , 1, this);
    ECS* ecs = new ECS(ui->textBrowser, this);


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



    //elevator->move(4);




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

MainWindow::~MainWindow()
{
    delete ui;
}

