#include "ecs.h"
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QEventLoop>

ECS::ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QList<Floor*> floors,  QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_elevators = *elevators;
    m_floors = floors;


}

void ECS::help(const int index)
{
    m_browser->append("Received help signal from elevator:   " + QString::number(index));
    m_elevators.at(index-1)->voice_connection();

    QTimer::singleShot(5000, [=]() {
        m_browser->append("There has not been a response in 5 seconds 911 has been placed");
    });

}


void ECS::emergency(const QString& em , QPushButton *confirmButton, int index)
{
    m_browser->append("Alert , there is a: " + em + "(this is a text and audio message");


    if (em == "door obstacles!"){
        m_browser->append("Light sensor has been interrupted when closing");
        m_elevators.at(index+1)->m_direction = "Stopped";
        m_elevators.at(index+1)->open_cab();

    }
    else if (em == "fire!"){
        //in all scenarios , the safe floor is 2
        m_browser->append("Moving all elevators to Floor 2");
        for (int i = 0; i < m_elevators.count(); ++i) {
            m_elevators[i]->move(2);
            m_elevators[i]->open_cab();
            m_floors[1]->open_Door();
        }
    }
    else if (em == "overload!"){
         m_browser->append("On elevator : " + QString::number(index +1));
         m_elevators.at(index+1)->m_direction = "Stopped";
         m_browser->append("Please remove the carrying capacity (this is a text and audio message)");
         communicate_doors(index , confirmButton , 2);

    }
    else {
        //in all scenarios , the safe floor is 2
        m_browser->append("Moving all elevators to Floor 2");
        m_browser->append("There is a power outage (this is a text and audio message)");
        for (int i = 0; i < m_elevators.count(); ++i) {
            m_elevators[i]->move(2);
            m_elevators[i]->open_cab();
            m_floors[1]->open_Door();
        }

    }
}

void ECS::find_elevator(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton ,  QComboBox *cab , const int floor , const QString direction){

    m_browser->append("Floor number: " + QString::number(floor) + " | Requested to go : " + direction);
    bool found_elevator = false;


    //find the first idle elevator

    for(int i = 0; i < m_elevators.size(); i++)
    {
       if(m_elevators.at(i)->m_idle)
       {
           m_browser->append("Allocation_strategy (A) has been activated | found an elevator that is idle");
           QTimer::singleShot(2500, [=]() {
               allocation_strategy_move(passengersOn , passengersOff ,confirmButton , cab, i, floor);
           });
           found_elevator = true;
           break;
       }
    }


    //find the elevator going in the same direction if not found from above
    if (!found_elevator)
    {



        for(int i = 0; i < m_elevators.size(); i++)
        {
           if((m_elevators.at(i)->m_direction == direction && direction =="Up" && m_elevators.at(i)->m_floor_number < floor)|| (m_elevators.at(i)->m_direction == direction && direction =="Down" && m_elevators.at(i)->m_floor_number > floor))
           {
               m_browser->append("Allocation_strategy (B) has been activated | found an elevator that is going in the same direciton and approaching the desired floor");
               m_elevators.at(i)->m_direction = "Stopped";
               //wait 2.5 seconds before the elevator can make any movements and be able to change directions


               QTimer::singleShot(2500, [=]() {
                   allocation_strategy_move(passengersOn , passengersOff ,confirmButton , cab, i, floor);
               });
               break;

           }
        }
    }


}


void ECS::move_elevator(QComboBox* passengersOn, QComboBox* passengersOff, QPushButton* confirmButton,  QComboBox *cab , const int elevator_index, const int to_floor) {
    Elevator* elevator = m_elevators.at(elevator_index);

    // Only move the elevator if it is not currently in motion
    if (elevator->m_direction == "Stopped") {
        elevator->move(to_floor);
        communicate_doors(passengersOn, passengersOff, confirmButton, cab, elevator_index, to_floor);


    }
}


void ECS::communicate_doors(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton ,  QComboBox *cab , const int index , const int floor){
    if (m_elevators.at(index)->m_direction == "Stopped"){
         m_elevators.at(index)->open_cab();
         m_floors.at(floor)->open_Door();
         confirmButton->setEnabled(true);



         connect(confirmButton, &QPushButton::clicked, this, [=]() {
                 int on = passengersOn->currentText().toInt();
                 int off = passengersOff->currentText().toInt();
                 int cab_number = cab->currentText().toInt() -1;
                 if (m_elevators.at(cab_number)->change_passengers(on , off)){
                     QString em = "overload!";
                     emergency(em , confirmButton,index);
                 }
             });



         QTimer* timerclose = new QTimer(this);

         timerclose->setInterval(1000);

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

void ECS::communicate_doors(const int index , QPushButton *confirmButton, const int floor){

         m_elevators.at(index)->open_cab();
         m_floors.at(floor)->open_Door();
         confirmButton->setEnabled(true);
         confirmButton->setText("Fix");


         connect(confirmButton, &QPushButton::clicked, this, [=]() {
             m_browser->append("Overload Error Fixed");
                 confirmButton->setText("Confirm");
                 m_elevators.at(index)->ring();
                 m_elevators.at(index)->close_cab();
                 m_floors.at(floor)->close_Door();
                 confirmButton->setEnabled(false);
             });

}


void ECS::allocation_strategy_move(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton ,  QComboBox *cab , const int index ,const int floor){
    Elevator* elevator = m_elevators.at(index);
    elevator->move(floor);
    communicate_doors(passengersOn, passengersOff, confirmButton, cab, index, floor);
}

