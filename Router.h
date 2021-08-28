#pragma once
#include<iostream>
#include<vector>
#include<queue>

using namespace std;

//moved this into router.h to prevent circular include
class FakeIPPacket {
public:

	FakeIPPacket() {
		TOS = 0;
		length = 0;
		time = 0;
	}

	FakeIPPacket(unsigned char tos, unsigned short l, unsigned short t) {
		TOS = tos;
		length = l;
		time = t;
	}

	void assignTOS(int tos) { TOS = tos; };
	void assignLength(int l) { length = l; };
	void assignTime(int t) { time = t; };
	unsigned char getTOS() { return TOS; };
	unsigned short getLength() { return length; };
	unsigned short getTime() { return time; };

private:
	// see https://tools.ietf.org/html/rfc791#section-3.1 for details of a real IP packet
	unsigned char TOS;
	unsigned short length;
	unsigned short time;

};

class Router {
	protected:
		vector<queue<FakeIPPacket>> heap; //this is the heap keeping a vector of queue's of FakeIPPackets

	private:
		int Evaluate(FakeIPPacket pkt);//returns the priority value for the packet
		int queueTime[7];//array holding total time in queue for each queue 0 through 6
		int numPackets[7];//array holding total packets added to heap in each queue 0 through 6
		int totalInQueue;//number of packets currently in the queue. Used in Stochastic dequeue

	public:
		Router();
		void enqueue(FakeIPPacket pkt);//puts the packet into the queue based on its priority
		virtual FakeIPPacket dequeue(int time);//removes and returns a packet from the highest priority queue
		void PrintStatistics();//prints the average amount of time each packet stays in the queue for each priority value and overall amount of time each packet is in the queue
		void swap(queue<FakeIPPacket> &a, queue<FakeIPPacket> &b);//swaps two queues in a vector
		void heapify(int i);//rebuilds heap to put all queues in correct order (max heap)
		void bottomUpHeapify(int i);//rebuilds heap bottom up to correct max heap order
		bool notEmpty();//returns weather or not the heap is empty
		int getTotalInQueue();//returns number of packets currently in queue
		void decrementTotal();//every time we dequeue it decreases this number by 1 to keep track of how many packets in queue
		void addQueueTime(int time, int indice);
		void incrementVector(int indice);
		virtual ~Router();
};

