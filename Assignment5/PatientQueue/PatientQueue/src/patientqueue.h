// This is the H file you will edit and turn in. (TODO: Remove this comment!)

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include "patientnode.h"
using namespace std;

class PatientQueue {
public:
    PatientQueue();

    // deep copy, to prevent memory from being lost (create our own objects on heap)
    // so that if the original's is destroyed, ours remain
    PatientQueue(const PatientQueue &other);

    /**
     * @brief PatientQueue::~PatientQueue - destructor, call clear on queue so nodes are removed from heap
     */
    ~PatientQueue();

    /**
     * @brief PatientQueue::clear - clears the queue
     */
    void clear();

    /**
     * @brief PatientQueue::frontName
     * @return name of the patient at the front of the queue
     * @pre - queue must not be empty
     */
    string frontName();

    /**
     * @brief PatientQueue::frontPriority
     * @return priority of the patient at the front of the liist
     * @pre queue must not be empty
     */
    int frontPriority();

    /**
     * @brief PatientQueue::isEmpty
     * @return true if queue is empty, false otherwise
     */
    bool isEmpty();

    /**
     * @brief PatientQueue::newPatient
     * @param name - name of patiient to insert
     * @param priority - priiority of patient to insert
     */
    void newPatient(string name, int priority);

    /**
     * @brief PatientQueue::processPatient
     * @return - the name of the patient processed
     * @pre - queue must not be empty
     */
    string processPatient();

    /**
     * @brief PatientQueue::upgradePatient - upgrades the first patient with name name
     * @param name - name of patient to upgrade
     * @param newPriority - new priority of patient
     * @pre newPriority is < old priority of patient
     */
    void upgradePatient(string name, int newPriority);


    /**
     * @brief PatientQueue::operator = deep copy, so that we have our own copy of the nodes on the heap
     * rather than just a copy of the other's pointers (prevents our nodes from dying when the original
     * nodes are deleted)
     */
    PatientQueue& operator=(const PatientQueue &other);

private:

    // pointer to front of queue
    PatientNode* front = nullptr;
    // would probably have liked to implement a sort of skip list here to make inserting, upgrading, etc faster
    // but specifications forbade it

    // print
    friend ostream& operator <<(ostream& out, const PatientQueue& queue);

};



