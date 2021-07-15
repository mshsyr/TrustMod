 /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
 //
 //
 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License version 2 as
 // published by the Free Software Foundation;
 //
 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 // GNU General Public License for more details.
 //
 // You should have received a copy of the GNU General Public License
 // along with this program; if not, write to the Free Software
 // Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 //
 // Author: Muhammad Shadi Hajar  <m.hajar@rgu.ac.uk> <msh.syr@gmail.com>
 //
 
#ifndef TRUST_H
#define TRUST_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include <stdint.h>
#include "ns3/uinteger.h"
#include "ns3/trust-node.h"
#include "ns3/aodv-rtable.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/trust-scheme.h"



#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/address.h"
namespace ns3 {

/* ... */


class Trust : public Application
{
public:
	static TypeId GetTypeId ();
	virtual TypeId GetInstanceTypeId () const;
	Trust ();
	void ForwardedInput (Ptr<const Packet>, Address) const; // Address is an NS3 MAC Address + type +length.
	void SentInput (Ptr<const Packet>, const Mac48Address) const;
	uint32_t FindTimeSlot (uint32_t,uint32_t);
	void SerializeToAsciiFile ();
	void SetNodeId ();
	uint16_t GetNodeId ();
	void SetSrcIp ();
	Ipv4Address GetSrcIp ();
	void SetSrcMac ();
	Mac48Address GetSrcMac ();
	void UpdateForwarded (Mac48Address);
	void UpdateSent (Mac48Address);
	void TrustEvaluation (std::string, uint32_t);
	void SetDroppedPackets (TrustNode*) ;
	void RunListener ();
	void RecommendationReceive (Ptr<Socket>);

	//////////////////////////////////////////////////////////// Attributes methods
	void SetSimulationTime(int att) {
		m_simulationTime = att;
	}

	uint16_t GetSimulationTime() const {
		return m_simulationTime;
	}
	
	
	void SetTimeUnit(int att) {
		m_timeUnit = att;
	}

	int GetTimeUnit() const {
		return m_timeUnit;
	}
	
	
	void SetFileName(std::string  att) {
		m_fileName = att;
	}

	std::string GetFileName() const {
		return m_fileName;
	}
	
	void SetListenerPort(int att) {
		m_listenerPort = att;
	}

	uint16_t GetListenerPort() const {
		return m_listenerPort;
	}
	
	//////////////////////////////////////////////////////////// 
	
protected:
	virtual void NotifyConstructionCompleted ();
	virtual void DoDispose (void);
	
private:
	//Ipv4Address sip;
	//Ipv4Address dip;
	//Mac48Address RelayNodeMac;
	//bool m_enabled;
	Time m_maxPerHopDelay;
	//void PeriodicCheckForForwardedPackets ();
	uint16_t m_simulationTime;
	uint16_t m_timeUnit;
	std::map <std::string, std::vector <TrustNode*> *> m_trustMap;
	std::string m_fileName;
	Mac48Address m_srcMac;
	Ipv4Address m_srcIp;
	uint16_t  m_nodeId;
	uint16_t  m_listenerPort;
	TrustScheme m_ts;
	
	

	




};
}

#endif /* TRUST_H */


