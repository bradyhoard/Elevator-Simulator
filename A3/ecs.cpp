#include "ecs.h"
#include <QDebug>
#include <QTimer>

ECS::ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QList<Floor*> floors,  QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_elevators = *elevators;
    m_floors = floors;


}

void ECS::emergency(const QString& em)
{
    m_browser->append(em);
    m_browser->append("Moving all elevators to Floor 1");
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

void ECS::find_elevator(const int floor , const QString direction){

    m_browser->append("Floor number: " + QString::number(floor) + " | Requested to go : " + direction);


    //find the first idle elevator

    for(int i = 0; i < m_elevators.size(); i++)
    {
       if(m_elevators.at(i)->m_idle)
       {
           allocation_strategyA(i, floor);
           break;
       }
    }


    //find the elevator going in the same direction

    for(int i = 0; i < m_elevators.size(); i++)
    {
       if(m_elevators.at(i)->m_direction == direction)
       {
           allocation_strategyB(i, floor);
           break;
       }
    }


}




void ECS::communiate_doors(const int index){
    if (m_elevators.at(index)->m_direction == "Stopped"){
         m_browser->append("From ECS open doors");

         QTimer* timer = qobject_cast<QTimer*>(sender());

             if (timer != nullptr)
             {
                 timer->stop();
             }

    }

}

void ECS::allocation_strategyA(const int index , const int floor){

    m_elevators.at(index)->move(floor);
    QTimer* timer = new QTimer(this);

    timer->setInterval(500);

    connect(timer, &QTimer::timeout, this, [=]() {
        communiate_doors(index); // pass the variable 42 to checkFunction()
    });

    timer->start();


}

void ECS::allocation_strategyB(const int index ,const int floor){
    m_elevators.at(index)->move(floor);
    QTimer* timer = new QTimer(this);

    timer->setInterval(500);

    connect(timer, &QTimer::timeout, this, [=]() {
        communiate_doors(index); // pass the variable 42 to checkFunction()
    });

    timer->start();

}

