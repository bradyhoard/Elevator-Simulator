#include "ecs.h"
#include <QDebug>
#include <QTimer>
#include <QPushButton>

ECS::ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QList<Floor*> floors,  QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_elevators = *elevators;
    m_floors = floors;


}

void ECS::help(const int index)
{
    m_browser->append("Received help signal from elevator:   " + QString::number(index));

    QTimer* timer = new QTimer(this);

    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, [=]() {
        m_elevators.at(index-1)->voice_connection();
        timer->stop();
    });

    timer->start();

}


void ECS::emergency(const QString& em , int index)
{
    m_browser->append("Alert , there is a: " + em);


    //in all scenarios , the safe floor is 2
    m_browser->append("Moving all elevators to Floor 2");


    if (em == "door obstacles!"){
        m_browser->append("door obstacles");
    }
    else if (em == "fire!"){
        m_browser->append("fire");
    }
    else if (em == "overload!"){
         m_browser->append("On elevator : " + QString::number(index +1));

    }
    else {
        m_browser->append("power out");
    }
/*
    Elevator *elevator1 = m_elevators.at(0);
    Elevator *elevator2 = m_elevators.at(1);
    Elevator *elevator3 = m_elevators.at(2);
    Elevator *elevator4 = m_elevators.at(3);

    elevator1->move(1);
    elevator2->move(1);
    elevator3->move(1);
    elevator4->move(1);

    Floor *floor0 = m_floors.at(0);
    floor0->open_Door();
*/

}

void ECS::find_elevator(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton , const int floor , const QString direction){

    m_browser->append("Floor number: " + QString::number(floor) + " | Requested to go : " + direction);


    //find the first idle elevator

    for(int i = 0; i < m_elevators.size(); i++)
    {
       if(m_elevators.at(i)->m_idle)
       {
           allocation_strategyA(passengersOn , passengersOff ,confirmButton ,i, floor);
           break;
       }
    }


    //find the elevator going in the same direction

    for(int i = 0; i < m_elevators.size(); i++)
    {
       if(m_elevators.at(i)->m_direction == direction)
       {
           allocation_strategyB(passengersOn , passengersOff ,confirmButton , i, floor);
           break;
       }
    }


}


void ECS::move_elevator(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton , const int elevator_index, const int to_floor){
    m_elevators.at(elevator_index)->move(to_floor);
    QTimer* timer = new QTimer(this);

    timer->setInterval(500);

    connect(timer, &QTimer::timeout, this, [=]() {
        communiate_doors(passengersOn , passengersOff  ,confirmButton , elevator_index , to_floor);
    });

    timer->start();




}


void ECS::communiate_doors(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton , const int index , const int floor){
    if (m_elevators.at(index)->m_direction == "Stopped"){
         m_elevators.at(index)->open_cab();
         m_floors.at(floor)->open_Door();
         confirmButton->setEnabled(true);



         QTimer* timer = qobject_cast<QTimer*>(sender());

             if (timer != nullptr)
             {
                 timer->stop();
             }


         connect(confirmButton, &QPushButton::clicked, this, [=]() {
                 int on = passengersOn->currentText().toInt();
                 int off = passengersOff->currentText().toInt();
                 if (m_elevators.at(index)->change_passengers(on , off)){
                     QString em = "overload!";
                     emergency(em , index);
                 }
             });



         QTimer* timerclose = new QTimer(this);

         timerclose->setInterval(10000);

         connect(timerclose, &QTimer::timeout, this, [=]() {
             m_elevators.at(index)->ring();
             m_elevators.at(index)->close_cab();
             m_floors.at(floor)->close_Door();
             confirmButton->setEnabled(false);
             timerclose->stop();
         });

         timerclose->start();

    }

}

void ECS::allocation_strategyA(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton , const int index , const int floor){

    m_elevators.at(index)->move(floor);
    QTimer* timer = new QTimer(this);

    timer->setInterval(500);

    connect(timer, &QTimer::timeout, this, [=]() {
        communiate_doors(passengersOn , passengersOff  ,confirmButton , index , floor);
    });

    timer->start();


}

void ECS::allocation_strategyB(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton , const int index ,const int floor){
    m_elevators.at(index)->move(floor);
    QTimer* timer = new QTimer(this);

    timer->setInterval(500);

    connect(timer, &QTimer::timeout, this, [=]() {
        communiate_doors(passengersOn , passengersOff  , confirmButton , index , floor);
    });

    timer->start();

}

