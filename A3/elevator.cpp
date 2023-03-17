#include "elevator.h"
#include "floor.h"
#include <QTimer>
#include <QEventLoop>
#include <QMutex>
#include <QTextEdit>
#include <QQueue>

//elevator constructor
Elevator::Elevator(QTextBrowser *browser, bool idle, QString direction, int floor_number,int passengers, int elevator_id ,  QQueue<int> floors_queue , QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_idle = idle;
    m_direction = direction;
    m_floor_number = floor_number;
    m_elevator_id = elevator_id;
    m_passengers = passengers;
    ele_timer = new QTimer(this);
    m_floors_queue = floors_queue;

}
/*
 * Name :
 *      Status
 *
 * Params :
 *      none
 *
 * returns :
 *      void
 *
 */
void Elevator::status()
{
    m_browser->append("_____________");
    m_browser->append("Elevator: " + QString::number(m_elevator_id));
    m_browser->append("At floor: " + QString::number(m_floor_number));
    QString floors_list = "Floors to visit: ";

    // iterate over the queue and add each integer to the floors_str
    foreach (int floor, m_floors_queue) {
        floors_list += QString::number(floor) + " ";
    }

    // display the floors_list in the user interface
    m_browser->append(floors_list);

    m_browser->append(QString("Idle: %1").arg(m_idle ? "true" : "false"));
    m_browser->append("Direction: " + m_direction);
    m_browser->append("# of passengers: " + QString::number(m_passengers));
    m_browser->append("_____________");

}
/*
 * Name :
 *      ring
 *
 * Params :
 *      none
 *
 * returns :
 *      void
 *
 */
void Elevator::ring()
{
    m_direction = "Stopped";
    m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") ringing!");

}
/*
 * Name :
 *      Status
 *
 * Params :
 *      int to_Floor : a floor requested by the user to go to
 *
 * returns :
 *      void
 *
 */
void Elevator::move(int to_Floor) {
    m_browser->append("Elevator: " + QString::number(m_elevator_id) + " - ");
       m_browser->insertPlainText("At floor: " + QString::number(m_floor_number) + " || To floor: " + QString::number(to_Floor));

       //the elevator is moving up
       if (m_floor_number < to_Floor){
           m_direction = "Up";
           m_idle = false;
               ele_timer = new QTimer(this);
               //display messages every 2 seconds
               ele_timer->setInterval(2000);
               QObject::connect(ele_timer, &QTimer::timeout, [=]() {
                   m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") - moving up to: " + QString::number(m_floor_number));

               });
               //loop over every floor to the requested floor , increasingly
               for(int i = m_floor_number +1; i <= to_Floor; i++)
               {
                   //each time the elevator moves to a new floor , update the floor number , this is technically the senser
                   m_floor_number = i;
                   ele_timer->stop();
                   ele_timer->start();
                   QEventLoop loop;
                   QObject::connect(ele_timer, &QTimer::timeout, &loop, &QEventLoop::quit);
                   loop.exec();
                   //if the elvator has reached its floor , exit
                   if (m_floor_number >= to_Floor) {
                       ele_timer->stop();
                       ring();
                       break;
                   }
                   //if it recvives a signal from the ECS that it should stop for some reason
                   else if (m_direction == "Stopped" && i != to_Floor) {

                       m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") - stopped");
                       //since it did not reach its floor , keep that value and put it in the queue
                       m_floors_queue.enqueue(to_Floor);
                       m_browser->append("Floor (" + QString::number(to_Floor) + ") added to cab (" + QString::number(m_elevator_id) + ") queue of floors - since the elevator changed floors");
                       ele_timer->stop();
                       break;
                   }
                   m_floor_number++;
               }




       }
       //the elevator is moving up
       else if (m_floor_number > to_Floor){
           m_direction = "Down";
           m_idle = false;
           ele_timer = new QTimer(this);
           //display messages every 2 seconds
           ele_timer->setInterval(2000);
           QObject::connect(ele_timer, &QTimer::timeout, [=]() {
               m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") - moving down to: " + QString::number(m_floor_number));
           });

           //loop over every floor to the requested floor , decreasingly
           for(int i = m_floor_number - 1; i >= to_Floor; i--)
           {
               m_floor_number = i;
               ele_timer->stop();
               ele_timer->start();
               QEventLoop loop;
               QObject::connect(ele_timer, &QTimer::timeout, &loop, &QEventLoop::quit);
               loop.exec();
               if (m_floor_number <= to_Floor) {
                   ele_timer->stop();
                   ring();
                   break;
               }
               else if (m_direction == "Stopped" && i != to_Floor) {
                   m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") - stopped");
                   //since it did not reach its floor , keep that value and put it in the queue
                   m_floors_queue.enqueue(to_Floor);
                   m_browser->append("Floor (" + QString::number(to_Floor) + ") added to cab (" + QString::number(m_elevator_id) + ") queue of floors - since the elevator changed floors");
                   ele_timer->stop();
                   break;
               }

               m_floor_number--;
           }
           }

       //elevator is already at the requested floor
       else{
            m_browser->append("Currently at the request Floor");
            ring();
       }
}




/*
 * Name :
 *      voice_connection
 *
 * Params :
 *      none
 *
 * returns :
 *      void
 *
 */
void Elevator::voice_connection()
{
    m_browser->append("Cab(" + QString::number(m_elevator_id) + ") Opened a voice connection...");

}

/*
 * Name :
 *      open_cab
 *
 * Params :
 *      none
 *
 * returns :
 *      void
 *
 */
void Elevator::open_cab()
{
    m_browser->append("Cab(" + QString::number(m_elevator_id) + ") door opened");
}

/*
 * Name :
 *      change_passengers
 *
 * Params :
 *      int on : passengers getting on
 *      int off : passegners getting off
 *
 * returns :
 *      bool , true if overload , false if not
 *
 */
void Elevator::change_passengers(int on , int off)
{
    m_browser->append("Passengers getting on: " + QString::number(on));
    m_browser->append("Passengers getting off: " + QString::number(off));
    m_passengers -= off;
    //can never have a negitive number of passengers
    if (m_passengers < 0){
        m_browser->append("Invalid passenger operation. Setting passengers to 0");
        m_passengers = 0;
    }
    m_passengers += on;
    //the elevator becomes idle if there is nobody on the elevator and therefore , will become the next available elevator in the allocation strategy
    if (m_passengers == 0){
        m_idle = true;
    }

}

/*
 * Name :
 *      close_cab
 *
 * Params :
 *      none
 *
 * returns :
 *      void
 *
 */
void Elevator::close_cab()
{
    m_browser->append("Cab(" + QString::number(m_elevator_id) + ") door closed");
}


