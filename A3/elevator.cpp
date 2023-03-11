#include "elevator.h"
#include "floor.h"

Elevator::Elevator(QTextBrowser *browser, bool idle,int floor_number,int passengers, int elevator_id , QList<Floor*> floors, QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_idle = idle;
    m_floor_number = floor_number;
    m_elevator_id = elevator_id;
    m_passengers = passengers;
    m_floors = floors;
}

void Elevator::status()
{
    m_browser->append("_____________");
    m_browser->append("Elevator: " + QString::number(m_elevator_id));
    m_browser->append("_____________");
    m_browser->append("At floor: " + QString::number(m_floor_number));
    m_browser->append(QString("Idle: %1").arg(m_idle ? "true" : "false"));
    m_browser->append("# of passengers: " + QString::number(m_passengers));
    m_browser->append("_____________");

}

void Elevator::ring()
{
    m_browser->append("Elevator ringing! We have reached the Floor");
    open_cab();
}

void Elevator::move(const int to_Floor)
{
    m_browser->append("Elevator: " + QString::number(m_elevator_id) + " - ");
    m_browser->insertPlainText("At floor: " + QString::number(m_floor_number) + " || To floor: " + QString::number(to_Floor));

    for(int i=m_floor_number +1;i<=to_Floor;i++)
    {
        m_browser->append("Moving to: ");
        m_browser->insertPlainText(QString::number(i));
        m_floor_number = i;

    }
    ring();

/*
    //current floor of the elevator and change to the new floor
    QString cur = QString::number(m_floor);
    m_browser->append(cur);
    m_floor = to_Floor;
    QString cur2 = QString::number(m_floor);
    m_browser->append(cur2);
*/


}

void Elevator::open_cab()
{
    m_browser->append("Cab Door opened");
}

void Elevator::close_cab()
{
    m_browser->append("Cab Door closed");
}


