//Controller.cpp
//Class files for controller

#include "Controller.h"
#include "NVDIMM.h"

using namespace NVDSim;

Controller::Controller(NVDIMM* parent){
	parentNVDIMM= parent;

	channelXferCyclesLeft= vector<uint>(NUM_PACKAGES, 0);

	channelQueues= vector<queue <ChannelPacket *> >(NUM_PACKAGES, queue<ChannelPacket *>());
	outgoingPackets= vector<ChannelPacket *>(NUM_PACKAGES, NULL);

	currentClockCycle= 0;
}

void Controller::attachPackages(vector<Package> *packages){
	this->packages= packages;
}

void Controller::returnReadData(const FlashTransaction  &trans){
	if(parentNVDIMM->ReturnReadData!=NULL){
		(*parentNVDIMM->ReturnReadData)(parentNVDIMM->systemID, trans.address, currentClockCycle);
	}
	parentNVDIMM->numReads++;
}

void Controller::returnIdlePower(vector<double> idle_energy) {
  if(parentNVDIMM->ReturnIdlePower!=NULL){
                vector<uint64_t> idle_power = vector<uint64_t>(NUM_PACKAGES, 0.0);
		for(uint i = 0; i < NUM_PACKAGES; i++)
		  {
		    idle_power[i] = idle_energy[i] * VCC;
		  }
                (*parentNVDIMM->ReturnIdlePower)(parentNVDIMM->systemID, idle_energy, currentClockCycle);
  }
}

void Controller::returnAccessPower(vector<double> access_energy) {
  if(parentNVDIMM->ReturnAccessPower!=NULL){
                vector<uint64_t> access_power = vector<uint64_t>(NUM_PACKAGES, 0.0);
		for(uint i = 0; i < NUM_PACKAGES; i++)
		  {
		    access_power[i] = access_energy[i] * VCC;
		  }
		(*parentNVDIMM->ReturnAccessPower)(parentNVDIMM->systemID, access_energy, currentClockCycle);
  }
}

void Controller::returnErasePower(vector<double> erase_energy) {
  if(parentNVDIMM->ReturnErasePower!=NULL){
                vector<uint64_t> erase_power = vector<uint64_t>(NUM_PACKAGES, 0.0);
		for(uint i = 0; i < NUM_PACKAGES; i++)
		  {
		    erase_power[i] = erase_energy[i] * VCC;
		  }
		(*parentNVDIMM->ReturnErasePower)(parentNVDIMM->systemID, erase_energy, currentClockCycle);
  }
}

void Controller::receiveFromChannel(ChannelPacket *busPacket){
	returnTransaction.push_back(FlashTransaction(RETURN_DATA, busPacket->virtualAddress, busPacket->data));
	delete(busPacket);
}

bool Controller::addPacket(ChannelPacket *p){
	channelQueues[p->package].push(p);
	return true;
}

void Controller::update(void){
	uint i;
	
	//Check for commands/data on a channel. If there is, see if it is done on channel
	for (i= 0; i < outgoingPackets.size(); i++){
		if (outgoingPackets[i] != NULL && (*packages)[outgoingPackets[i]->package].channel->hasChannel(CONTROLLER, 0)){

			channelXferCyclesLeft[i]--;
			if (channelXferCyclesLeft[i] == 0){
				(*packages)[outgoingPackets[i]->package].channel->sendToDie(outgoingPackets[i]);
				(*packages)[outgoingPackets[i]->package].channel->releaseChannel(CONTROLLER, 0);
				outgoingPackets[i]= NULL;
			}
		}
	}
	
	//Look through queues and send oldest packets to the appropriate channel
	for (i= 0; i < channelQueues.size(); i++){
		if (!channelQueues[i].empty() && outgoingPackets[i]==NULL){
			//if we can get the channel (channel contention not implemented yet)
			if ((*packages)[i].channel->obtainChannel(0, CONTROLLER, channelQueues[i].front())){
				outgoingPackets[i]= channelQueues[i].front();
				channelQueues[i].pop();
				switch (outgoingPackets[i]->busPacketType){
					case DATA:
						channelXferCyclesLeft[i]= DATA_TIME;
						break;
					default:
						channelXferCyclesLeft[i]= COMMAND_TIME;
						break;
				}
			}
		}
	}
	//See if any read data is ready to return
	while (!returnTransaction.empty()){
		//call return callback
		returnReadData(returnTransaction.back());
		returnTransaction.pop_back();
	}
}
