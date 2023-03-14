#ifndef ECS_H
#define ECS_H

#include <QObject>
#include <QTextBrowser>
#include <QPushButton>
#include <QComboBox>
#include "elevator.h"
#include "floor.h"

class ECS : public QObject
{
    Q_OBJECT
public:
    explicit ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QList<Floor*> floors ,  QObject *parent = nullptr);

public slots:
     void help(const int index);
    void emergency(const QString& em , QPushButton *cofirmButton, int elevator_index =0);
    void find_elevator(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *cofirmButton , QComboBox *cab , const int floor , const QString direction);
    void move_elevator(QComboBox *passengersOn , QComboBox *passengersOff , QPushButton *cofirmButton ,  QComboBox *cab , const int elevator_index, const int to_floor);
    void communicate_doors(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *cofirmButton ,  QComboBox *cab ,const int index , const int floor);
    void communicate_doors(const int index ,  QPushButton *cofirmButton , const int floor = 2);
    void allocation_strategyA(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *cofirmButton ,  QComboBox *cab ,const int elevator ,const int floor);
    void allocation_strategyB(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *cofirmButton ,  QComboBox *cab ,const int elevator ,const int floor);

private:
    QTextBrowser *m_browser;
    QList<Elevator*> m_elevators;
    QList<Floor*> m_floors;
};

#endif // ECS_H
