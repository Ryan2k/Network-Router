#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include <cstdlib>
#include <ctime>
#include "StochasticRouter.h"

#define MAX_PACKETS 5
#define TIME_TO_RUN 1000

using namespace std;

int main  () {
	srand((unsigned) time(0));
	
	int t = 0;
	StochasticRouter router;
	router.SetPriorityWeight(1);
	while (router.notEmpty() || t < TIME_TO_RUN) {  // run until there are no more packets left
		for (t = 0; t < TIME_TO_RUN; t++) {   // packet generator time loop
			int numPackets = rand() % MAX_PACKETS;
			for (int i = 0; i < numPackets; i++) { // packet generator loop
				// generate packet
				FakeIPPacket pkt;
				pkt.assignTOS(rand()%256);
				pkt.assignLength(rand()%65535 + 1);
				pkt.assignTime(t);
				// enqueue packet
				router.enqueue(pkt);
			}
			// dequeue packet, 1 per second
			router.dequeue(t);
			
		}
	}
	/*FakeIPPacket pkt1;
	pkt1.assignTOS(14);
	pkt1.assignLength(21);
	pkt1.assignTime(1);
	FakeIPPacket pkt2;
	pkt2.assignTOS(157);
	pkt2.assignLength(214);
	pkt2.assignTime(11);
	FakeIPPacket pkt3;
	pkt3.assignTOS(140);
	pkt3.assignLength(21212);
	pkt3.assignTime(91);
	FakeIPPacket pkt4;
	pkt4.assignTOS(1);
	pkt4.assignLength(1);
	pkt4.assignTime(112);
	FakeIPPacket pkt5;//copy of 1
	pkt5.assignTOS(14);
	pkt5.assignLength(21);
	pkt5.assignTime(1);
	FakeIPPacket pkt6;
	pkt6.assignTOS(255);
	pkt6.assignLength(65535);
	pkt6.assignTime(987);
	router.enqueue(pkt1);
	router.enqueue(pkt2);
	router.enqueue(pkt3);
	router.enqueue(pkt4);
	router.enqueue(pkt5);
	router.enqueue(pkt6);
	router.dequeue(14);
	router.dequeue(70);
	router.dequeue(91);
	router.dequeue(111);
	router.dequeue(212);
	router.dequeue(423);*/
	router.PrintStatistics();
}
