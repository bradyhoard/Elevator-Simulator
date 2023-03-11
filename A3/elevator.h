#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QTextBrowser>
#include "floor.h"

class Elevator : public QObject
{
    Q_OBJECT
public:
    explicit Elevator(QTextBrowser *browser, bool idle, int floor_number,  int m_passengers, int elevator_id , QList<Floor*> floors ,  QObject *parent = nullptr);

public slots:
    void ring();
    void status();
    void move( const int to_Floor);
    void open_cab();
    void close_cab();


private:
    QTextBrowser *m_browser;
    bool m_idle;
    int m_floor_number;
    int m_elevator_id;
    //number of passengeres that are in the elevator
    int m_passengers;
     QList<Floor*> m_floors;

};

#endif // ELEVATOR_H
