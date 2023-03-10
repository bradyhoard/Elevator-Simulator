#include "elevator.h"
#include <QDebug>
#include <QTimer>

Elevator::Elevator(QTextBrowser *browser, bool idle,int floor, int elevator_id , QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_idle = idle;
    m_floor = floor;
    m_elevator_id = elevator_id;
}

void Elevator::ring()
{
    m_browser->append("Elevator ringing!");
}

void Elevator::move(const int to_Floor)
{
    m_browser->append("Elevator " + QString::number(m_elevator_id) + " - ");
    m_browser->insertPlainText("At floor - " + QString::number(m_floor) + " || To floor - " + QString::number(to_Floor));

    for(int i=m_floor +1;i<=to_Floor;i++)
    {

        QTimer::singleShot(2000, [=]() {
            m_browser->append("Moving to - ");
            m_browser->insertPlainText(QString::number(i));
            m_floor = i;
        });
    }

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
    m_browser->append("Cab Door opening...");
}

void Elevator::close_cab()
{
    m_browser->append("Cab Door closing...");
}

