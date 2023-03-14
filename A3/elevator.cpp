#include "elevator.h"
#include "floor.h"
#include <QTimer>
#include <QEventLoop>
#include <QMutex>
#include <QTextEdit>

Elevator::Elevator(QTextBrowser *browser, bool idle, QString direction, int floor_number,int passengers, int elevator_id , QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_idle = idle;
    m_direction = direction;
    m_floor_number = floor_number;
    m_elevator_id = elevator_id;
    m_passengers = passengers;
    ele_timer = new QTimer(nullptr);

}

void Elevator::status()
{
    m_browser->append("_____________");
    m_browser->append("Elevator: " + QString::number(m_elevator_id));
    m_browser->append("At floor: " + QString::number(m_floor_number));
    m_browser->append(QString("Idle: %1").arg(m_idle ? "true" : "false"));
    m_browser->append("Direction: " + m_direction);
    m_browser->append("# of passengers: " + QString::number(m_passengers));
    m_browser->append("_____________");

}

void Elevator::ring()
{
    m_direction = "Stopped";
    m_browser->append("Elevator ringing!");

}

void Elevator::move(int to_Floor) {
    m_browser->append("Elevator: " + QString::number(m_elevator_id) + " - ");
       m_browser->insertPlainText("At floor: " + QString::number(m_floor_number) + " || To floor: " + QString::number(to_Floor));

       if (m_floor_number < to_Floor){
           m_direction = "Up";
           m_idle = false;
               ele_timer = new QTimer(this);
               ele_timer->setInterval(1000);
               QObject::connect(ele_timer, &QTimer::timeout, [=]() {
                   m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") - moving up to: " + QString::number(m_floor_number));

               });
               for(int i = m_floor_number +1; i <= to_Floor; i++)
               {
                   m_floor_number = i;
                   ele_timer->start();
                   QEventLoop loop;
                   QObject::connect(ele_timer, &QTimer::timeout, &loop, &QEventLoop::quit);
                   loop.exec();
                   if (m_floor_number >= to_Floor) {
                       ele_timer->stop();
                       ring();
                       break;
                   }
                   else if (m_direction == "Stopped" && i != to_Floor) {
                       m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") - stopped");
                       ele_timer->stop();
                       break;
                   }

                   m_floor_number++;
               }
       }
       else if (m_floor_number > to_Floor){
           m_direction = "Down";
           m_idle = false;
               ele_timer = new QTimer(this);
               ele_timer->setInterval(1000);
               QObject::connect(ele_timer, &QTimer::timeout, [=]() {
                   m_browser->append("Elevator(" + QString::number(m_elevator_id) + ") - moving down to: " + QString::number(m_floor_number));

               });
               for(int i = m_floor_number - 1; i >= to_Floor; i--)
               {
                   m_floor_number = i;
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
                       ele_timer->stop();
                       break;
                   }

                   m_floor_number--;
               }
           }

       else{
            m_browser->append("Currently at the request Floor");
            ring();
       }
}

void Elevator::voice_connection()
{
    m_browser->append("Cab(" + QString::number(m_elevator_id) + ") Opened a voice connection...");

}

void Elevator::open_cab()
{
    m_browser->append("Cab(" + QString::number(m_elevator_id) + ") door opened");
}

bool Elevator::change_passengers(int on , int off)
{
    m_browser->append("Passengers getting on: " + QString::number(on));
    m_browser->append("Passengers getting off: " + QString::number(off));
    m_passengers -= off;
    if (m_passengers < 0){
        m_browser->append("Invalid passenger operation. Setting passengers to 0");
        m_passengers = 0;
    }
    m_passengers += on;
    //assume the overload capacity is 6 people
    if (m_passengers >= 6){
        return true;
    }
    else if (m_passengers == 0){
        m_idle = true;
        return false;
    }
    else{
        return false;
    }
}


void Elevator::close_cab()
{
    m_browser->append("Cab(" + QString::number(m_elevator_id) + ") door closed");
}


