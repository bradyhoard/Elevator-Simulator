#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QTextBrowser>

class Elevator : public QObject
{
    Q_OBJECT
public:
    explicit Elevator(QTextBrowser *browser, bool idle, int floor, int elevator_id , QObject *parent = nullptr);

public slots:
    void ring();
    void move(const int to_Floor);
    void open_cab();
    void close_cab();


private:
    QTextBrowser *m_browser;
    bool m_idle;
    int m_floor;
    int m_elevator_id;
};

#endif // ELEVATOR_H
