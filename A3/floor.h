#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>
#include <QTextBrowser>

class Floor : public QObject
{
    Q_OBJECT
public:
    explicit Floor(QTextBrowser *browser, QObject *parent = nullptr);

public slots:
    void open_Door();
    void close_Door();

private:
    QTextBrowser *m_browser;
};

#endif // FLOOR_H
