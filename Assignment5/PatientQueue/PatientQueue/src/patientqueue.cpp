#include "patientqueue.h"


PatientQueue::PatientQueue() {
    // not much to be done here i think
    // front initialized default
}

/**
 * @brief PatientQueue::PatientQueue - copy constructor
 */
PatientQueue::PatientQueue(const PatientQueue &other){

    if(!this->isEmpty())
    {
        this->clear();
    }

    PatientNode* current = other.front;
    while(current)
    {
        this->newPatient(current->name,current->priority);
        current = current->next;
    }

}

/**
 * @brief PatientQueue::~PatientQueue - destructor, call clear on queue so that nodes are removed from heap
 */
PatientQueue::~PatientQueue() {

    this->clear();
}

/**
 * @brief PatientQueue::clear - clears the queue
 */
void PatientQueue::clear() {

    if(this->isEmpty())
    {
        return;
    }

    PatientNode* current = this->front;
    while(!this->isEmpty())
    {
        this->front = current->next;
        delete current;
        current = this->front;
    }

}

/**
 * @brief PatientQueue::frontName
 * @return name of the patient at the front of the queue
 * @pre - queue must not be empty
 */
string PatientQueue::frontName() {
    return this->isEmpty() ? throw("frontName called on empty queue") : this->front->name;
}

/**
 * @brief PatientQueue::frontPriority
 * @return priority of the patient at the front of the liist
 * @pre queue must not be empty
 */
int PatientQueue::frontPriority() {
    return this->isEmpty() ? throw("frontPriority called on empty queue") : this->front->priority;
}

/**
 * @brief PatientQueue::isEmpty
 * @return true if queue is empty, false otherwise
 */
bool PatientQueue::isEmpty() {
    if(this->front == nullptr)
    {
        return true;
    }
    return false;
}

/**
 * @brief PatientQueue::newPatient
 * @param name - name of patiient to insert
 * @param priority - priiority of patient to insert
 */
void PatientQueue::newPatient(string name, int priority) {

    PatientNode* newPatient = new PatientNode(name,priority);
    if(this->isEmpty())
    {
        this->front = newPatient;
        return;
    }

    if(this->front->priority > priority)
    {
        newPatient->next = front;
        this->front = newPatient;
        return;
    }

    PatientNode* current = this->front;
    while(current->next != nullptr && current->next->priority <= priority)
    {
        current = current->next;
    }

    newPatient->next = current->next;
    current->next = newPatient;
    return;

}

/**
 * @brief PatientQueue::processPatient
 * @return - the name of the patient processed
 * @pre - queue must not be empty
 */
string PatientQueue::processPatient() {

    if(this->isEmpty())
    {
        throw("Called processPatient on empty queue");
    }

    string name = this->front->name;
    PatientNode* trash = this->front;
    this->front = this->front->next;
    delete trash;
    return name;

}

/**
 * @brief PatientQueue::upgradePatient - upgrades the first patient with name name
 * @param name - name of patient to upgrade
 * @param newPriority - new priority of patient
 * @pre newPriority is < old priority of patient
 */
void PatientQueue::upgradePatient(string name, int newPriority) {

    PatientNode* current = this->front;

    if(this->front->name == name)
    {
        PatientNode* trash = this->front;
        front = front->next;
        delete trash;
        newPatient(name,newPriority);
        return;
    }
    while(current->next)
    {
        if(current->next->name == name)
        {
            if(current->next->priority <= newPriority)
            {
                throw("Attempt to reduce priority of element which");
            }
            else
            {
                PatientNode* trash = current->next;
                current->next = current->next->next;
                delete trash;
                newPatient(name,newPriority);
                return;
            }
        }
        current = current->next;
    }
    throw("upgradePatient called with name which does not exist in queue");

}

/**
 * @brief PatientQueue::operator = deep copy, so that we have our own copy of the nodes on the heap
 * rather than just a copy of the other's pointers (prevents our nodes from dying when the original
 * nodes are deleted)
 */
PatientQueue& PatientQueue::operator=(const PatientQueue &other){

    if(this != &other)
    {
        this->clear();

        PatientNode* current  = other.front;
        while(current)
        {
            this->newPatient(current->name,current->priority);
            current = current->next;
        }
        return *this;
    }

}

/**
 * @brief operator << print
 */
ostream& operator <<(ostream& out, const PatientQueue& queue) {

    PatientNode *current = queue.front;
    out << "{";
    while(current != nullptr)
    {
        out << current->priority << ": " << current->name;

        current = current->next;
        if(current != nullptr)
        {
            out << ", ";
        }
    }
    out << "}";
    return out;

}


