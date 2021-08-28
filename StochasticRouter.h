#pragma once
#include "Router.h"

using namespace std;

class StochasticRouter: public Router {
	private:
		float weight;

	public: 
		StochasticRouter();
		void SetPriorityWeight(float weight);
		FakeIPPacket dequeue(int time) override;
		virtual ~StochasticRouter();
};
