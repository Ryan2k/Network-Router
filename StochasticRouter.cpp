#include "StochasticRouter.h"

/*No need to implement a new constructor. : Router means that it implements the router Super class
same as implements in java and using "super." This comment is just for myself as i just learned this*/
StochasticRouter::StochasticRouter() : Router() {

}

/*Sets the weight for the dequeue. If the weight is 1, it follows the proper priority, if the weight is .5
there is a 50 percent chance it dequeues by priority and 50 that it is random, at 0 it is totally random 
and so on. Throws error if the weight is not between 1 and 0 as it wouldnt make sense*/
void StochasticRouter::SetPriorityWeight(float weight) {
	if (weight < 0 || weight > 1) {
		throw std::runtime_error("Float Weight Must Be Between 0 and 1");
	}
	this->weight = weight;
}

/*There is a weight * 100% chance that we dequeu regularly which is represented by creating a random double between 0 and 1, and if it is less
than weight, we dequeue regularly. Otherwise we select a random packet in the entire heap. We do this by creating a random integer between 1 and the
total num of packets in the heap, and starting at queue at heap[0] front item, moving up until we find the correct packet. For example, if there are 15 packets
in the heap (5 in 0, 3 in 1, and 7 in 2), and the random num is 9. We select the 1st item in the third heap. The first while loop represents selecting the queue
in the heap and the second represents the position in the queue by emptying out the queue, dequeueing the correct packet, and re inserting them in the queue in new correct order*/
FakeIPPacket StochasticRouter::dequeue(int time) {
	double randomNum = (double)rand() / RAND_MAX;
	if (randomNum <= weight) {
		return Router::dequeue(time);
	}

	int randomPacket = std::rand() % (getTotalInQueue());
	int indice = 0;

	while (randomPacket >= heap[indice].size()) {
		randomPacket -= heap[indice].size();
		indice++;
	}

	if (heap[indice].size() ==1) {
		FakeIPPacket result = heap[indice].front();
		swap(heap[indice], heap[heap.size() - 1]);
		heap.pop_back();
		heapify(indice);
		decrementTotal();
		addQueueTime(time - result.getTime(), indice);
		incrementVector(indice);
		return result;
	}

	queue<FakeIPPacket> newQueue;
	while (randomPacket > 0) {
		newQueue.push(heap[indice].front());
		heap[indice].pop();
		randomPacket--;
	}

	FakeIPPacket result = heap[indice].front();
	heap[indice].pop();

	while (!heap[indice].empty()) {
		newQueue.push(heap[indice].front());
		heap[indice].pop();
	}

	heap[indice] = newQueue;
	addQueueTime(time - result.getTime(), indice);
	incrementVector(indice);
	decrementTotal();

	return result;

}

/*Again, no need to destruct anything as there is no dynamic memory. Vectors are destructed automatically by c++*/
StochasticRouter:: ~StochasticRouter() {

}
