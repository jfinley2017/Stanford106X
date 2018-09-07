// This is the H file you will edit and turn in. (TODO: Remove this comment!)

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "patientnode.h"



using namespace std;

class PatientQueue {
public:
    PatientQueue();
    ~PatientQueue();

    /**
     * @brief PatientQueue::clear iterates over the heap, deleted any created memory
     */
    void clear();

    /**
     * @brief PatientQueue::frontName
     * @return name of the element at the root of the heap
     * @pre - assumes heap is not empty
     */
    string frontName();

    /**
     * @brief PatientQueue::frontPriority
     * @return priority of the element at the root of the heap
     * @pre - assumes heap is not empty
     */
    int frontPriority();

    /**
     * @brief PatientQueue::isEmpty
     * @return true if heap is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * @brief PatientQueue::newPatient - adds a new patient to the queue
     * @param name - name of new patient
     * @param priority - priority of new patient
     */
    void newPatient(string name, int priority);

    /**
     * @brief PatientQueue::processPatient - processes the first patient in the queue
     * @return name of patient first in the queue
     */
    string processPatient();

    /**
     * @brief PatientQueue::upgradePatient - used to increase a patient's priority
     * @param name - name of patient
     * @param newPriority - new priority of patient
     * @pre - assumes the patient current has a priority which is greater than new priority
     */
    void upgradePatient(string name, int newPriority);


private:

    /**
     * @brief heap - vector representation of the heap
     * index 0 = root
     * index i*2+1 = left child
     * index i*2+2 = right child
     */
    vector<PatientNode*> heap;

    /**
     * @brief PatientQueue::heapifyUp - compares each node to that of its parents, swaps if
     * the current node is less than the parent
     * @param position - position to start the heapifyUp at
     */
    void heapifyUp(int position);

    /**
     * @brief PatientQueue::heapifyDown - compares each parent to that of its children, swaps
     * with child if needed, recursive
     * @param position
     */
    void heapifyDown(int position);

    /**
     * @brief PatientQueue::swap - swaps two elements
     */
    void swap(int pos1, int pos2);

    /**
     * @brief PatientQueue::heapRemove - removes element from heap by swapping the root element and the last element
     * and then heapifying the root down.
     * @param pos - element to remove, also the root of heapifyDown
     */
    void heapRemove(int pos);


    friend ostream& operator <<(ostream& out, const PatientQueue& queue);
};

