#ifndef ECS_H
#define ECS_H

#include <QObject>
#include <QTextBrowser>
#include "elevator.h"
#include "floor.h"

class ECS : public QObject
{
    Q_OBJECT
public:
    explicit ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QList<Floor*> floors ,  QObject *parent = nullptr);

public slots:
     void help(const int index);
    void emergency(const QString& em);
    void find_elevator(const int floor , const QString direction);
    void communiate_doors(const int index , const int floor);
    void allocation_strategyA(const int elevator ,const int floor);
    void allocation_strategyB(const int elevator ,const int floor);

private:
    QTextBrowser *m_browser;
    QList<Elevator*> m_elevators;
    QList<Floor*> m_floors;
};

#endif // ECS_H
