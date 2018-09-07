// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "patientqueueheap.h"


PatientQueue::PatientQueue() {

}

PatientQueue::~PatientQueue() {
    clear();
}

/**
 * @brief PatientQueue::clear iterates over the heap, deleted any created memory
 */
void PatientQueue::clear() {

    if(isEmpty())
    {
        return;
    }
    for(int i = 0; i < heap.size()-1; i++)
    {
        delete heap[i];
    }
    heap.clear();
}

/**
 * @brief PatientQueue::frontName
 * @return name of the element at the root of the heap
 * @pre - assumes heap is not empty
 */
string PatientQueue::frontName() {
   return isEmpty() ? throw("frontName called on empty queue") : heap[0]->name;
}

/**
 * @brief PatientQueue::frontPriority
 * @return priority of the element at the root of the heap
 * @pre - assumes heap is not empty
 */
int PatientQueue::frontPriority() {
    return isEmpty() ? throw("frontPriority called on empty queue") : heap[0]->priority;
}


/**
 * @brief PatientQueue::isEmpty
 * @return true if heap is empty, false otherwise
 */
bool PatientQueue::isEmpty() const{
    if(heap.size() == 0)
    {
        return true;
    }
    return false;
}

/**
 * @brief PatientQueue::newPatient - adds a new patient to the queue
 * @param name - name of new patient
 * @param priority - priority of new patient
 */
void PatientQueue::newPatient(string name, int priority) {
    PatientNode* newNode = new PatientNode(name, priority);
    // if the queue is empty, simply place at front
    if(this->isEmpty())
    {
        heap.push_back(newNode);
        return;
    }
    // otherwise, place new element at the back and heapifyUp to determine
    // correct location
    else
    {
       heap.push_back(newNode);
       heapifyUp(heap.size()-1);
    }
}

/**
 * @brief PatientQueue::processPatient - processes the first patient in the queue
 * @return name of patient first in the queue
 */
string PatientQueue::processPatient() {

    string processedName = frontName();
    int processedPriority = frontPriority();

    heapRemove(0);

    return processedName;

}

/**
 * @brief PatientQueue::upgradePatient - used to increase a patient's priority
 * @param name - name of patient
 * @param newPriority - new priority of patient
 * @pre - assumes the patient current has a priority which is greater than new priority
 */
void PatientQueue::upgradePatient(string name, int newPriority) {

    for(int i = 0; i < heap.size(); i++)
    {
        if(heap[i]->name == name)
        {
            if(heap[i]->priority <= newPriority)
            {
                throw("upgradePatient attempted to upgrade patient with equal to or lower priority ");
            }
            else
            {
                heapRemove(i);
                newPatient(name,newPriority);
            }
        }
    }
}


/**
 * @brief PatientQueue::heapifyUp - compares each node to that of its parents, swaps if
 * the current node is less than the parent
 * @param position - position to start the heapifyUp at
 */
void PatientQueue::heapifyUp(int position) {
    int currentIndex = position;
    // while we are not at the root
    while(currentIndex != 0)
    {
        int parent = (currentIndex-1) / 2;
        if(heap[parent]->priority > heap[currentIndex]->priority)
        {
            swap(currentIndex, parent);
            currentIndex = parent;
        }
        else
        {
            break;
        }
    }
}

/**
 * @brief PatientQueue::heapifyDown - compares each parent to that of its children, swaps
 * with child if needed, recursive
 * @param position
 */
void PatientQueue::heapifyDown(int position) {

    if(position*2+1 >= heap.size())
    {
        return;
    }
    else
    {
        int small = position;
        if(position*2+2 < heap.size() && heap[position*2+2]->priority < heap[small]->priority)
        {
            small = position*2+2;
        }
        if(heap[position*2+1]->priority < heap[small]->priority)
        {
            small = position*2+1;
        }

        if(small != position)
        {
            swap(small,position);
            heapifyDown(small);
        }
    }
}


/**
 * @brief PatientQueue::swap - swaps two elements
 */
void PatientQueue::swap(int pos1, int pos2)
{
    PatientNode* temp = heap[pos1];
    heap[pos1] = heap[pos2];
    heap[pos2] = temp;
}

/**
 * @brief PatientQueue::heapRemove - removes element from heap by swapping the root element and the last element
 * and then heapifying the root down.
 * @param pos - element to remove, also the root of heapifyDown
 */
void PatientQueue::heapRemove(int pos) {

     swap(pos,heap.size()-1);
     delete heap[heap.size()-1];
     heap.pop_back();
     heapifyDown(pos);

}

ostream& operator <<(ostream& out, const PatientQueue& queue) {

    out << "{";
    for(int i = 0; i < queue.heap.size(); i++)
    {
        out << queue.heap[i]->priority << ": " << queue.heap[i]->name;
        if(i != queue.heap.size() -1)
        {
            out << ", ";
        }
    }
    out << "}";
    return out;
}
