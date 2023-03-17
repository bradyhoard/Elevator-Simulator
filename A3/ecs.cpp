#include "ecs.h"
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QEventLoop>
#include <algorithm> // for std::sort
#include <functional> // for std::greater

//Elevator control system constructor
ECS::ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QList<Floor*> floors,  QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_elevators = *elevators;
    m_floors = floors;
    button = new Button(browser);


}

/*
 * Name :
 *      help
 *
 * Params :
 *      int index : the elevator that called for help
 *
 * returns :
 *      void
 *
 */
void ECS::help(const int index)
{
    m_browser->append("Received help signal from elevator:   " + QString::number(index));
    m_elevators.at(index-1)->voice_connection();

    //assume in all scenrios that the user does not respond in 5 seconds , displays another message
    QTimer::singleShot(5000, [=]() {
        m_browser->append("There has not been a response in 5 seconds 911 has been placed in cab ("  + QString::number(index) + ")");
    });

}

/*
 * Name :
 *      emergecny
 *
 * Params :
 *      QString em : name of the emergency
 *      QPushButton confirmButton: used to control actions on the GUI that will help in fixing issues
 *      int index: index of the elevator/cab
 *
 * returns :
 *      void
 *
 */
void ECS::emergency(const QString& em , QPushButton *confirmButton, int index)
{
    m_browser->append("Alert , there is a: " + em + " (this is a text and audio message)");

    //NOTE: in all scenarios , the safe floor is 2

    //handle the door obstacles emergecny
    if (em == "door obstacles!"){
        m_browser->append("Light sensor has been interrupted when closing");
        m_elevators.at(index-1)->m_direction = "Stopped";
        m_elevators.at(index-1)->open_cab();

    }
    //handle the fire emergecny
    else if (em == "fire!"){
        m_browser->append("Moving all elevators to Floor 2");
        for (int i = 0; i < m_elevators.count(); ++i) {
            m_elevators[i]->move(2);
            m_elevators[i]->open_cab();
            m_floors[1]->open_Door();
        }
    }
    //handle the overload emergecny
    else if (em == "overload!"){
         m_browser->append("On elevator : " + QString::number(index+1));
         m_elevators.at(index+1)->m_direction = "Stopped";
         m_browser->append("Please remove the carrying capacity (this is a text and audio message)");
         communicate_doors(index , confirmButton , 2);

    }
    //handle the power outage emergecny
    else {
        m_browser->append("Running off the battery backup");
        m_browser->append("Moving all elevators to Floor 2");
        for (int i = 0; i < m_elevators.count(); ++i) {
            m_elevators[i]->move(2);
            m_elevators[i]->open_cab();
            m_floors[1]->open_Door();
        }

    }
}
/*
 * Name :
 *      find_elevator
 *
 * Params :
 *      QComboBox passengersOn : passegnergers combo box to be able to pass through in order to track the passengers on
 *      QComboBox passengersOff:  passegnergers combo box to be able to pass through in order to track the passengers off
 *      QPushButton confirmButton: passed throuhg in order to confim passengers
 *      QComboBox cab: passed throuhg in order to confim cab number for passegners
 *      int floor: requested floor to go to
 *      QString direction: direction requested to go
 *
 * returns :
 *      void
 *
 */
void ECS::find_elevator(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton ,  QComboBox *cab , const int floor , const QString direction){

    button->illuminate(floor , "Floor");
    m_browser->append("Floor number: " + QString::number(floor) + " | Requested to go : " + direction);
    bool found_elevator = false;


    //find the first idle elevator (this is the first allocation strategy)

    for(int i = 0; i < m_elevators.size(); i++)
    {
       if(m_elevators.at(i)->m_idle)
       {
           m_browser->append("Allocation_strategy (A) has been activated | found an elevator that is idle - Elevator : " + QString::number(i+1));
           QTimer::singleShot(2500, [=]() {
               allocation_strategy_move(passengersOn , passengersOff ,confirmButton , cab, i, floor);
           });

           found_elevator = true;
           break;
       }
    }


    //find the elevator going in the same direction if not found from above
    if (!found_elevator)
    {


        //(this is the second allocation strategy)
        for(int i = 0; i < m_elevators.size(); i++)
        {
           if((m_elevators.at(i)->m_direction == direction && direction =="Up" && m_elevators.at(i)->m_floor_number < floor)|| (m_elevators.at(i)->m_direction == direction && direction =="Down" && m_elevators.at(i)->m_floor_number > floor))
           {
               m_browser->append("Allocation_strategy (B) has been activated | found an elevator that is going in the same direciton and approaching the desired floor - Elevator : " + QString::number(i+1));
               m_elevators.at(i)->m_direction = "Stopped";

               //wait 1.5 seconds before the elevator can make any movements and be able to change destinations
               QTimer::singleShot(1500, [=]() {
                   allocation_strategy_move(passengersOn , passengersOff ,confirmButton , cab, i, floor);
               });

               found_elevator = true;
               break;

           }
        }
    }

    //if the two options above fail , this will alway return en elevator
    //get the first elevator (in some scenrios if the first elevator were to be broken , ECS would would adapt and call the next elevator
    if (!found_elevator){
        //as a fail safe
        m_browser->append("Allocation_strategy (C aka fail safe) has been activated | found - Elevator : " + QString::number(1));
        m_elevators.at(0)->m_direction = "Stopped";

        //wait 1.5 seconds before the elevator can make any movements and be able to change destinations
        QTimer::singleShot(1500, [=]() {
            allocation_strategy_move(passengersOn , passengersOff ,confirmButton , cab, 0, floor);
        });

        found_elevator = true;

    }


}


/*
 * Name :
 *      move_elevator
 *
 * Params :
 *      QComboBox passengersOn : passegnergers combo box to be able to pass through in order to track the passengers on
 *      QComboBox passengersOff:  passegnergers combo box to be able to pass through in order to track the passengers off
 *      QPushButton confirmButton: passed throuhg in order to confim passengers
 *      QComboBox cab: passed throuhg in order to confim cab number for passegners
 *      int elevator_index: cab number to wants to move the elevator
 *      int floor: requested floor to go to
 *
 *
 * returns :
 *      void
 *
 */
void ECS::move_elevator(QComboBox* passengersOn, QComboBox* passengersOff, QPushButton* confirmButton,  QComboBox *cab , const int elevator_index, const int to_floor) {
    Elevator* elevator = m_elevators.at(elevator_index);


    // Only move the elevator if it is not currently in motion
    if (elevator->m_idle) {
        button->illuminate(elevator_index , "Elevator" , to_floor);
        elevator->move(to_floor);
        //once it reaches the requested floor
        if (to_floor == elevator->m_floor_number){
            button->deilluminate(elevator_index , "Elevator" ,  to_floor);
            //open cab and floors doors at (roughly) the same time
            communicate_doors(passengersOn, passengersOff, confirmButton, cab, elevator_index, to_floor);
        }


    }

    //if its not stopped
    else{
        //add the elevator to the queue of stops
        elevator->m_floors_queue.enqueue(to_floor);
        m_browser->append("Floor (" + QString::number(to_floor) + ") added to cab (" + QString::number(elevator_index+1) + ") queue of floors");
    }
}

/*
 * Name :
 *      communicate_doors (override)
 *
 * Params :
 *      QComboBox passengersOn : passegnergers combo box to be able to pass through in order to track the passengers on
 *      QComboBox passengersOff:  passegnergers combo box to be able to pass through in order to track the passengers off
 *      QPushButton confirmButton: passed throuhg in order to confim passengers
 *      QComboBox cab: passed throuhg in order to confim cab number for passegners
 *      int index: cab number to wants to move the elevator
 *      int floor: requested floor to go to
 *
 *
 * returns :
 *      void
 *
 */
void ECS::communicate_doors(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton ,  QComboBox *cab , const int index , const int floor){

    //open cab and floor doors and allow the confirm to be interactive on the GUI
     m_elevators.at(index)->open_cab();
     m_floors.at(floor)->open_Door();
     confirmButton->setEnabled(true);

     //connect function for when the confirm button is clicked
     disconnect(confirmButton, &QPushButton::clicked, this, nullptr);
     connect(confirmButton, &QPushButton::clicked, this, [=]() {
         //all this does is add passegners to the cab number indicated , if change_passengers returns true , there is an overload emergency
         int on = passengersOn->currentText().toInt();
         int off = passengersOff->currentText().toInt();
         int cab_number = cab->currentText().toInt();
         m_elevators.at(cab_number-1)->change_passengers(on , off);
         if (m_elevators.at(cab_number-1)->m_passengers >=6){
             QString em = "overload!";
             emergency(em , confirmButton,index);
         }
     });

     //after 10 seconds (put to 4 seconds for testing) of the doors opening , ring again , and close both floor and cab doors
     QTimer::singleShot(10000, [=]() {
         m_elevators.at(index)->ring();
         m_elevators.at(index)->close_cab();
         m_floors.at(floor)->close_Door();
         confirmButton->setEnabled(false);
         Elevator* elevator = m_elevators.at(index);
         //if the queue is not empty after closing the doors , move to the next floor in the queue
         if (!(elevator->m_floors_queue.isEmpty())) {
             //remove the head of the queue
             int temp_floor = elevator->m_floors_queue.head();
             elevator->m_floors_queue.dequeue();
             elevator->move(temp_floor);

             //once its reached , communicate doors once again
             if (temp_floor == elevator->m_floor_number){
                 button->deilluminate(index , "Elevator" ,  temp_floor);
                 communicate_doors(passengersOn, passengersOff, confirmButton, cab, index, temp_floor);
             }

         }
     });
}

/*
 * Name :
 *      communicate_doors
 *
 * Params :
 *      QPushButton confirmButton: passed throuhg in order to confim passengers
 *      int index: cab number to wants to move the elevator
 *      int floor: requested floor to go to
 *
 *
 * returns :
 *      void
 *
 */
//mainly used from the emergency function
void ECS::communicate_doors(const int index , QPushButton *confirmButton, const int floor){

         m_elevators.at(index)->open_cab();
         m_floors.at(floor)->open_Door();
         confirmButton->setEnabled(true);
         //the confirm button changed text in order to fix the "issue" that occured
         confirmButton->setText("Fix");


         //once clicked the program will return to its normal state
         disconnect(confirmButton, &QPushButton::clicked, this, nullptr);
         connect(confirmButton, &QPushButton::clicked, this, [=]() {
             m_browser->append("Overload Error Fixed");
                 confirmButton->setText("Confirm");
             });

}

/*
 * Name :
 *      allocation_strategy_move
 *
 * Params :
 *      QComboBox passengersOn : passegnergers combo box to be able to pass through in order to track the passengers on
 *      QComboBox passengersOff:  passegnergers combo box to be able to pass through in order to track the passengers off
 *      QPushButton confirmButton: passed throuhg in order to confim passengers
 *      QComboBox cab: passed throuhg in order to confim cab number for passegners
 *      int index: cab number to wants to move the elevator
 *      int floor: requested floor to go to
 *
 *
 * returns :
 *      void
 *
 */
void ECS::allocation_strategy_move(QComboBox *passengersOn , QComboBox *passengersOff ,QPushButton *confirmButton ,  QComboBox *cab , const int index ,const int floor){
    Elevator* elevator = m_elevators.at(index);
    //after a given elevator has been found using a allocation strategy , this function will call the elevator
    elevator->move(floor);
    button->deilluminate(floor , "Floor");
    communicate_doors(passengersOn, passengersOff, confirmButton, cab, index, floor);
}

/*
 * Name :
 *      get_elevators
 *
 * Params :
 *      none
 *
 *
 * returns :
 *      QList<Elevator*>
 *
 */
QList<Elevator*> ECS::get_elevators() {
    return m_elevators;
}

