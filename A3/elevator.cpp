#include "elevator.h"
#include "floor.h"
#include <QTimer>
#include <QEventLoop>

Elevator::Elevator(QTextBrowser *browser, bool idle, QString direction, int floor_number,int passengers, int elevator_id , QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_idle = idle;
    m_direction = direction;
    m_floor_number = floor_number;
    m_elevator_id = elevator_id;
    m_passengers = passengers;

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

bool Elevator::ring()
{
    m_direction = "Stopped";
    m_idle = true;
    m_browser->append("Elevator ringing! We have reached the Floor");
    return true;
    QTimer *timer = new QTimer(this);
        timer->setInterval(1000);
        QObject::connect(timer, &QTimer::timeout, [=]() {
            timer->stop();
            timer->deleteLater();
            open_cab();
        });
        timer->start();
}

void Elevator::move(const int to_Floor)
{
    m_browser->append("Elevator: " + QString::number(m_elevator_id) + " - ");
    m_browser->insertPlainText("At floor: " + QString::number(m_floor_number) + " || To floor: " + QString::number(to_Floor));

    if (m_floor_number < to_Floor){
        m_direction = "Up";
        m_idle = false;
        QTimer *timer = new QTimer(this);
            timer->setInterval(1000);
            QObject::connect(timer, &QTimer::timeout, [=]() {
                m_browser->append("Moving up to: ");
                m_browser->insertPlainText(QString::number(m_floor_number));
                m_floor_number++;
                if (m_floor_number > to_Floor) {
                    m_floor_number--;
                    timer->stop();
                    timer->deleteLater();
                    ring();
                }
            });
            for(int i = m_floor_number + 1; i <= to_Floor; i++)
            {
                m_floor_number = i;
                timer->start();
                QEventLoop loop;
                QObject::connect(timer, &QTimer::timeout, &loop, &QEventLoop::quit);
                loop.exec();
            }
    }
    else if (m_floor_number > to_Floor){
        m_direction = "Down";
        m_idle = false;
        QTimer *timer = new QTimer(this);
            timer->setInterval(1000);
            QObject::connect(timer, &QTimer::timeout, [=]() {
                m_browser->append("Moving down to: ");
                m_browser->insertPlainText(QString::number(m_floor_number));
                m_floor_number--;
                if (m_floor_number < to_Floor) {
                    m_floor_number++;
                    timer->stop();
                    timer->deleteLater();
                    ring();
                }
            });
            for(int i = m_floor_number - 1; i >= to_Floor; i--)
            {
                m_floor_number = i;
                timer->start();
                QEventLoop loop;
                QObject::connect(timer, &QTimer::timeout, &loop, &QEventLoop::quit);
                loop.exec();
            }

        }

    else{
         m_browser->append("Currently at the request Floor");
         ring();
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
    m_browser->append("Cab Door opened");
}

void Elevator::close_cab()
{
    m_browser->append("Cab Door closed");
}


