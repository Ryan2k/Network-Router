#include "Router.h"

/*Constructor for Router class taking in no parameters. There are seven possible queues when the heap is full
therefor it loops through the queuetime array and the numpackets array 7 times initializing all values 
to a starting point of zero, and adding to these values when we dequeue. Also sets the integer tracking
total packets in queue to 0*/
Router::Router() {
    totalInQueue = 0;
    for (int i = 0; i < 7; i++) {
        queueTime[i] = 0;
        numPackets[i] = 0;
    }
}

/*This function creates a priority for a FakeIPPacket based on its TOS and Length
There are 255 possible TOS' and 65,535 possible Length's all starting form 0 and incrementing by 1 and multiplying them creates 16,711,425 possibilites
I then divide that by 2.7M and because it is an integer, it creates 7 possible priorities which we will use to select which queue it gets enqueued into.
The reason for choosing 7 is because it is a perfectly balanced 3 height tree and we are queueing via a maxheap which is a tree.*/
int Router::Evaluate(FakeIPPacket pkt) {
    int result = (int)pkt.getTOS() * (int)pkt.getLength();//255 possible unsigned char's and 65,535 unsigned short't all starting from zero and incrementing by one
    result /= 2700000;
    return result; 
}

/*Enqueu adds a FakeIPPacket taken in as a parameter to the heap. Loops through from zero to the current size of the heap to check
if there is currently a heap that has a queue with packets with equivalent priority to the new packet. If it finds it, it simply
pushes the packet into that queue. If not, it creates a new queue, pushes the packet into that queue, pushes the queue into the heap,
then, if it is not the only queue in the heap, it calls bottomUpHeapify to re arrange the heap vector back into a max heap.*/
void Router::enqueue(FakeIPPacket pkt) {
    cout << "Enqueued: " << Evaluate(pkt) << endl;
    totalInQueue++;
    for (int i = 0; i < heap.size(); i++) {
        if (!heap[i].empty() && Evaluate(heap[i].front()) == Evaluate(pkt)) {
            heap[i].push(pkt);
            return;//if the queue already exists, no need to create a new queue and re organize
        }
    }

    queue<FakeIPPacket> newQueue;
    newQueue.push(pkt);
    heap.push_back(newQueue);

    if (heap.size() != 1) {//no need to re heapify if newQueue is the only queue in the vector
        bottomUpHeapify(heap.size() - 1);
    }
}

/*Dequeue takes in an integer time as a parameter which is the amount of time the packet we are dequeueing has been kept in the heap.
This value is added to queueTime at the position with the dequeueing packets priority, and we are also incrementing numPackets at that
position as another packet with that priority has been dequeued. If the heap is empty nothing can be duqueued so i through an error if this
is attempted. The packet that is next to be dequeued is always at the queue in position zero in the heap vector as the 0 indice holds the max
priority. It will be the front of this queue as the dequeueing at this point is first in first out so i pop that packet. I then checked to see
if the queue we popped from is now empty, and if it is, i swap it with the queue at the end of the heap to pop it, then call heapify to re
arrange it back to a max heap.*/
FakeIPPacket Router::dequeue(int time) {
    totalInQueue--;
    if (heap.empty()) {
        throw std::runtime_error("Nothing to Dequeue");
    }

    FakeIPPacket result = heap[0].front();
    queueTime[Evaluate(result)] += (time - result.getTime());//time when it is being dequeued - time it got queued
    numPackets[Evaluate(result)]++;
    heap[0].pop();

    if (heap[0].empty()) {
        swap(heap[0], heap[heap.size() - 1]);
        heap.pop_back();
        heapify(0);
    }
    cout << "Dequeued: " << Evaluate(result) << endl;
    return result;
}

//swaps two queues positions in the vector taking in their refrences, creating a temp holding a, assigning b to a, and assigning temp to b
void Router::swap(queue<FakeIPPacket>& a, queue<FakeIPPacket>& b) {
    queue<FakeIPPacket> temp = a;
    a = b;
    b = temp;
}

/*heapify rebuilds the vector, heap, to represent a Max Heap
It takes in an integer, i, which is the position of the root of our tree/sub tree and assigns it to largest with left and right
being the root nodes left and right children respectively. It then checks to see if either of the children's TOS are larger than it,
meaning they need to replace it as the root, and if so, we recursively call heapify to reorganize the entire side of the subree/ whole tree.
Because each queue has the same TOS on each FakeIPPacket, we can simply access that via the front item in the queue*/
void Router::heapify(int i) {
    //Need to change it from comparing TOS to comparing priority value once i figure out how to do that
    int largest = i;
    int left = i * 2 + 1;//always position of left child
    int right = i * 2 + 2;//always position of right child

    if (left < heap.size() && Evaluate(heap[left].front()) > Evaluate(heap[largest].front())) {
        largest = left;
    }

    if (right < heap.size() && Evaluate(heap[right].front()) > Evaluate(heap[largest].front())) {
        largest = right;
    }

    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapify(largest);
    }
}

/*Same concept as heapify except it works from the last position up. Takes in the position of the child node in the vector as a parameter,
it then finds its parents position which is always (i-1 / 2), and checks to see if the priority value of the child is less than its parent.
If it is not, it swaps there positions, and recursively calls the function until we are back in a max heap. This only gets called when we
enqueue an item as it will always be pushed to the back and we only need to worry about its comparison to its parent as the relationship 
to its siblings and cousins do not matter in a max heap*/
void Router::bottomUpHeapify(int i) {
    if (i <= 0) {
        return;
    }

    int smallest = i;
    int parent = (i - 1) / 2;

    if (Evaluate(heap[parent].front()) < Evaluate(heap[smallest].front())) {
        swap(heap[smallest], heap[parent]);
        bottomUpHeapify(parent);
    }
}

/*Returns false if the heap is empty, and true otherwise*/
bool Router::notEmpty() {
    return heap.empty();
}

int Router::getTotalInQueue() {//returns number of packets currently in queue
    return totalInQueue;
}

/*PrintStatistics prints out the average time each packet was in each queue as well as all packets within the heap. It does this by 
dividing the total time each packet was in the queue (kept track in the array queueTime which has each priority as an indice and adds
the time when dequeueing a packet), and divides it by the number of packets that have been passed through each queue (only dequeued).*/
void Router::PrintStatistics() {
    int totalQueueTimes = 0;
    int totalNumPackets = 0;

    for (int i = 0; i < 7; i++) {
        totalQueueTimes += queueTime[i];
        totalNumPackets += numPackets[i];
        if (numPackets[i] != 0) {
            cout << "Priority " << i << " Has an Average Queue Time of: " << double(queueTime[i]) / numPackets[i] << "ms" << endl;
        }
        else {
            cout << "Priority " << i << " Did Not Queue Any Packets" << endl;
        }
    }

    //cout << "QueueTimeOfZero: " << queueTime[0] << " NumPacketsOfZero: " << numPackets[0] << endl;//for testing purposes
    cout << "Average Time of all FakeIPPackets in Queue is: " << (double)totalQueueTimes / totalNumPackets << "ms" << endl;
    
}
void Router::decrementTotal(){//every time we dequeue it decreases this number by 1 to keep track of how many packets in queue
    totalInQueue--;
}

void Router::addQueueTime(int time, int indice) {//used to add queue time in stochastic (same as a setter)
    queueTime[indice] += time;
}

void Router::incrementVector(int indice) {//same as a setter but just needed to increment. Used in stochastic
    numPackets[indice]++;
}

/*No need to delete anything because we are not using dynamic memory or anything on the heap. The arrays initialized in
the constructor are given a size and are on the stack so c++ delete them automatically*/
Router::~Router(){
   
}