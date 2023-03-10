#ifndef ECS_H
#define ECS_H

#include <QObject>
#include <QTextBrowser>
#include "elevator.h"

class ECS : public QObject
{
    Q_OBJECT
public:
    explicit ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QObject *parent = nullptr);

public slots:
    void emergency(const QString& em);

private:
    QTextBrowser *m_browser;
    QList<Elevator*> m_elevators;
};

#endif // ECS_H
