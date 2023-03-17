#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QTextBrowser>
#include <QTimer>
#include <QQueue>
#include "floor.h"

class Elevator : public QObject
{
    Q_OBJECT
public:
    explicit Elevator(QTextBrowser *browser, bool idle, QString m_direction, int floor_number,  int m_passengers, int elevator_id , QQueue<int> floors_queue , QObject *parent = nullptr);
    QString m_direction;
    bool m_idle;
    int m_floor_number;
    QQueue<int> m_floors_queue;
    QTextBrowser *m_browser;
    //number of passengeres that are in the elevator
    int m_passengers;

public slots:
    void ring();
    void change_passengers(int passengers_on , int passengers_off);
    void voice_connection();
    void status();
    void move( const int to_Floor);
    void open_cab();
    void close_cab();


private:

    int m_elevator_id;
    QTimer *ele_timer;


};

#endif // ELEVATOR_H
