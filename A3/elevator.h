#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QTextBrowser>
#include "floor.h"

class Elevator : public QObject
{
    Q_OBJECT
public:
    explicit Elevator(QTextBrowser *browser, bool idle, QString m_direction, int floor_number,  int m_passengers, int elevator_id , QObject *parent = nullptr);
    QString m_direction;
    bool m_idle;

public slots:
    bool ring();
    bool change_passengers(int passengers_on , int passengers_off);
    void voice_connection();
    void status();
    void move( const int to_Floor);
    void open_cab();
    void close_cab();



private:
    QTextBrowser *m_browser;
    int m_floor_number;
    int m_elevator_id;
    //number of passengeres that are in the elevator
    int m_passengers;


};

#endif // ELEVATOR_H
