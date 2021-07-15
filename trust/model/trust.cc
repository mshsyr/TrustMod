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
 
#include "trust.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include <map>
#include "ns3/aodv-module.h"
#include "ns3/aodv-neighbor.h"
#include <fstream>
#include "ns3/wifi-module.h"
#include "ns3/arp-header.h"




//#define PERIODIC_CHECK_INTERVAL (Seconds (1))

namespace ns3 {

/* ... */

NS_LOG_COMPONENT_DEFINE ("Trust");

NS_OBJECT_ENSURE_REGISTERED (Trust);

TypeId 
Trust::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Trust")
    .SetParent<Object> ()
    .SetGroupName ("Trust")
    .AddConstructor<Trust> ()
    .AddAttribute ("MaxPerHopDelay", ("The maximum per-hop delay that should be considered.  "
                                      "Packets still not received after this delay are to be considered lost."),
                   TimeValue (Seconds (10.0)),
                   MakeTimeAccessor (&Trust::m_maxPerHopDelay),
                   MakeTimeChecker ())
	.AddAttribute("SimulationTimeAttribute", "An attribute to set the simulation time",
					UintegerValue(0),
					MakeUintegerAccessor(&Trust::SetSimulationTime,
										&Trust::GetSimulationTime),
					MakeUintegerChecker<uint16_t>())
	.AddAttribute("TimeUnitAttribute", "An attribute to set the time unit",
					UintegerValue(1),
					MakeUintegerAccessor(&Trust::SetTimeUnit,
										&Trust::GetTimeUnit),
					MakeUintegerChecker<uint16_t>())
	.AddAttribute ("FileAttribute", "Trust Serialization File Name",
                   StringValue ("TrustData.txt"),
                   MakeStringAccessor (&Trust::SetFileName, &Trust::GetFileName),
                   MakeStringChecker())
	.AddAttribute("RecommendationsListenerPortAttribute", "An attribute to set the port number",
					UintegerValue(50000),
					MakeUintegerAccessor(&Trust::SetListenerPort,
										&Trust::GetListenerPort),
					MakeUintegerChecker<uint16_t>())
  ;
  return tid;
}

TypeId 
Trust::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

Trust::Trust ()
{
  NS_LOG_FUNCTION (this);
}

void
Trust::NotifyConstructionCompleted ()
{
  Object::NotifyConstructionCompleted ();
  //Simulator::Schedule (PERIODIC_CHECK_INTERVAL, &Trust::PeriodicCheckForForwardedPackets, this);
}

void 
Trust::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Object::DoDispose ();
}


void
Trust::ForwardedInput (Ptr<const Packet>  packet, Address from) const
{
	// Address is an NS3 MAC Address + type +length. So, it need to be converted to Mac48Address
	Ptr<Node> node = GetNode ();
	Ptr<Trust> trust = node->GetObject<Trust> ();
	
	
	// 1. Convert from Address to Mac48Address
	Mac48Address srcMac = Mac48Address::ConvertFrom (from);	
	
	//2. Check if packet source ip equal to node IP and the packet source mac is not equal to node mac
	Ptr<Packet> p = packet->Copy();
	Ipv4Header packetHeader;
	p->RemoveHeader (packetHeader);
	if (packetHeader.GetSource () == trust->GetSrcIp () && srcMac != trust->GetSrcMac () )
		trust->UpdateForwarded (srcMac);	
}

void
Trust::SentInput (Ptr<const Packet> packet, const Mac48Address to) const
{
	
	
	//1. Copy the packet and get the header
	//std::cout<<"(Trust.cc->SentInput) The packet pointer is "<<packet<<std::endl;
	Ptr<Packet> p = packet->Copy();
	Ipv4Header packetHeader;
	p->RemoveHeader (packetHeader);
	
	//2. Check if the destination ip represents a relay node or not. (Using AODV neighbor)
	Ptr<Node> node = GetNode ();
	Ptr<aodv::RoutingProtocol> aodv = node->GetObject<aodv::RoutingProtocol> ();
	Ipv4Address dstIp = packetHeader.GetDestination();
	bool neighbor = aodv->IsNeighbor(dstIp);
	//std::cout<<"The neighbor value is: "<<neighbor<<std::endl;
	
	//Temp to monitor the packets: (To be removed)
	//std::cout<<"At "<<Simulator::Now ().GetSeconds ()<<" Source IP: "<<packetHeader.GetSource ()<<" Dst Mac: "<<to<<std::endl;

	
	//3. If it is a relayNode, update the relevant trust record
	Ptr<Trust> trust = node->GetObject<Trust> ();
	if(!neighbor && trust->GetSrcIp () == packetHeader.GetSource () && to != Mac48Address("ff:ff:ff:ff:ff:ff"))
	{
		//std::cout<<"The trust pointer: "<<trust<<" The source IP of the sent packet: "<<packetHeader.GetSource ()<<" The destination IP of the sent packet is: "<<packetHeader.GetDestination () <<" The source ip of the node: "<<trust->GetSrcIp ()<<std::endl;
		trust->UpdateSent(to);
	}	

}

// A method to find the begin of the current time unit
uint32_t 
Trust::FindTimeSlot (uint32_t time, uint32_t timeUnit)
{
	uint32_t q = time/timeUnit;
	uint32_t timeSlotBegin = q * timeUnit;
	return timeSlotBegin;
}

// Serialize to ASCII
void 
Trust::SerializeToAsciiFile ()
{
	NS_LOG_FUNCTION (this << "Test Output"); // To be filled later with appropiate text
	Ptr<Node> node = GetNode ();
	Ptr<Trust> trust = node->GetObject<Trust> ();
	std::string str = trust->GetFileName();
	std::cout<<"The Name of the trust file is "<<str<<std::endl;
	std::ofstream trustFile;
	trustFile.open(str.c_str(), std::ios::out | std::ios::app); //c_str to convert to standard c string (array of char), not necessary here.
	trustFile <<"The node ID is: "<<GetNodeId ()<<std::endl;
	trustFile <<"----------------- "<<std::endl;
	//trustFile <<"The Source Mac is: "<<GetSrcMac ()<<std::endl;
	
	if (m_trustMap.empty())
	{
		trustFile <<"The Map is empty for this node"<<std::endl;
		trustFile <<"**************************************************************************\n"<<std::endl;
	}
	else
	{
		for (auto &el1: m_trustMap) // el1 represents a row (element) in the map
		{
			trustFile <<"Trustee MAC address: "<<el1.first<<std::endl;
			
			
			
			std::vector<TrustNode *> *v = el1.second;
			std::vector<TrustNode *> ::iterator iter;
			trustFile<<"Sequence: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetSeq ()<<", ";
			}
			
			trustFile<<"\n\nTime Unit: ";
			for (iter = v->begin(); iter != v->end(); iter++) 
			{
				trustFile <<(*iter)->GetTime ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nSent Packets: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetSentPackets ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nForwarded Packets: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetForwardedPackets ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nDropped Packets: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetDroppedPackets ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nDirectTrust: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetDirectTrustValue ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nUncertainty: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetUncertainty ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nIndirectUncertainty: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetIndirectUncertainty ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nIndirectTrust: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetIndirectTrustValue ()<<", ";
			}
			
			//std::cout<<std::endl;
			trustFile<<"\n\nOverallTrust: ";
			for (iter = v->begin(); iter != v->end(); iter++)
			{
				trustFile <<(*iter)->GetOverallTrust ()<<", ";
			}
			
			
			
			/*
			for (auto &el2: el1.second) //el2 represents a  trust node of the vector (of the element el1) 
			{
				trustFile <<el2->GetSeq ()<<" ";
			}
			
			
			trustFile<<"\n\nTime Unit: ";
			for (auto &el2: el1.second) 
			{
				trustFile <<el2->GetTime ()<<" ";
			}
			
			std::cout<<std::endl;
			trustFile<<"\n\nSent Packets: ";
			for (auto &el2: el1.second)
			{
				trustFile <<el2->GetSentPackets ()<<" ";
			}
			
			std::cout<<std::endl;
			trustFile<<"\n\nForwarded Packets: ";
			for (auto &el2: el1.second)
			{
				trustFile <<el2->GetForwardedPackets ()<<" ";
			}
			
			std::cout<<std::endl;
			trustFile<<"\n\nDropped Packets: ";
			for (auto &el2: el1.second)
			{
				trustFile <<el2->GetDroppedPackets ()<<" ";
			}
			
			std::cout<<std::endl;
			trustFile<<"\n\nDirectTrust: ";
			for (auto &el2: el1.second)
			{
				trustFile <<el2->GetDirectTrustValue ()<<" ";
			}
			*/
			trustFile<<"\n\n";
			
		}
		trustFile <<std::endl;
		trustFile<<"**************************************************************************\n"<<std::endl;
	}
		
	
	
	trustFile.close ();
	std::cout<<"Serialization is Done"<<std::endl;

}

void
Trust::SetNodeId ()
{
	Ptr<Node> node = GetNode();
	m_nodeId = node->GetId (); 
}

uint16_t
Trust::GetNodeId ()
{
	return m_nodeId;
}


void
Trust::SetSrcMac ()
{
	Ptr<Node> node = GetNode();
	Ptr<NetDevice> wifiNetDevice = node->GetDevice (1);
	m_srcMac = Mac48Address::ConvertFrom(wifiNetDevice->GetAddress());
	//std::cout<<"The source MacAddress is "<<Mac48Address::ConvertFrom(wifiNetDevice->GetAddress())<<std::endl;
}

Mac48Address
Trust::GetSrcMac ()
{
	return m_srcMac;
}


void
Trust::SetSrcIp ()
{
	Ptr<Node> node = GetNode ();
    Ptr<NetDevice> dev = node->GetDevice(1); // assuming only one device. Loop through devices if needed 
    Ptr<Ipv4> ip = node->GetObject<Ipv4>();  // Get Ipv4 Object
    uint32_t interfaceId = ip->GetInterfaceForDevice(dev); //Get interface id
    Ipv4InterfaceAddress addrI = ip->GetAddress(interfaceId, 0); //address index 0, might be different
    m_srcIp = addrI.GetLocal();
	//std::cout<<"Trust the source ip address of the node is: "<<m_srcIp<<std::endl; 
}

Ipv4Address
Trust::GetSrcIp ()
{
	return m_srcIp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////UpdateForwarded
void
Trust::UpdateForwarded (Mac48Address from)
{
	//1. Convert from Mac48Address to a string to be used as an ID for trustee
	std::stringstream ss;
    ss << from;
	std::string srcMac = ss.str();
	//srcMac.pop_back();
	//std::cout<<"The src mac address is: "<<srcMac<<std::endl;

	
	//2. Check if there is a map element for this node
	Ptr<Node> node = GetNode ();
	Ptr<Trust> trust = node->GetObject<Trust> ();
	
	
	
	if (trust->m_trustMap.count(srcMac) == 0) // return the number of elements with key, which will be 0 or 1. It is like the item exist or not
	{	// Create an element with the key dstMac
		std::vector<TrustNode *> *v = new std::vector<TrustNode *> ();
		TrustNode *tn = new TrustNode();
		tn->SetSeq(1);
		uint32_t timeSlotBegin = trust->FindTimeSlot(Simulator::Now ().GetSeconds (), GetTimeUnit ());
		tn->SetTime(timeSlotBegin); ///
		tn->UpdateForwardedPackets ();
		v->push_back(tn);
		trust->m_trustMap[srcMac] = v;
		
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::SetDroppedPackets, trust, tn);
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::TrustEvaluation, trust, srcMac, 1);
		return;
	}
	
	// There is a map element (vector), but need an new trust node
	std::vector<TrustNode*> *v = trust->m_trustMap[srcMac];
	TrustNode *tn = v->back();
	uint32_t timeSlotBegin = trust->FindTimeSlot(Simulator::Now ().GetSeconds (), GetTimeUnit ());
	
	if (tn->GetTime () < timeSlotBegin)
	{
		TrustNode *newTn = new TrustNode ();
		newTn->SetSeq (tn->GetSeq () + 1);
		newTn->SetTime (timeSlotBegin);
		newTn->UpdateForwardedPackets ();
		v->push_back(newTn);
		//trust->m_trustMap[srcMac] = v;
		
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::SetDroppedPackets, trust, newTn);
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::TrustEvaluation, trust, srcMac, newTn->GetSeq ());

	}
	else
	{
		v->back()->UpdateForwardedPackets ();
		//trust->m_trustMap[srcMac] = v;
		//std::cout<<"Trust Node tn value:"<<tn->GetSeq()<<"SRC MAC: "<<srcMac<<"Node ID: "<<node->GetId ()<<std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////UpdateSent
void
Trust::UpdateSent (Mac48Address to)
{
	
	//1. Convert dst Mac48Address to a string to be used as an ID for trustee
	std::stringstream ss;
    ss << to;
	std::string dstMac = ss.str();
	//dstMac.pop_back();
	//std::cout<<"The dst mac address is: "<<dstMac<<std::endl;
	
	//2. Check if there is a map element for this node
	Ptr<Node> node = GetNode ();
	Ptr<Trust> trust = node->GetObject<Trust> ();
	
	
	
	if (trust->m_trustMap.count(dstMac) == 0) // return the number of elements with key, which will be 0 or 1. It is like the item exist or not
	{	// Create an element with the key dstMac
		std::vector<TrustNode*> *v = new std::vector<TrustNode *> ();
		TrustNode *tn = new TrustNode ();
		tn->SetSeq(1);
		uint32_t timeSlotBegin = trust->FindTimeSlot(Simulator::Now ().GetSeconds (), GetTimeUnit ());
		tn->SetTime(timeSlotBegin); ///
		std::cout<<"First time set for trust node"<<timeSlotBegin<<std::endl;
		tn->UpdateSentPackets ();
		v->push_back(tn);
		trust->m_trustMap[dstMac] = v;
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::SetDroppedPackets, trust, tn);
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::TrustEvaluation, trust, dstMac, 1);
		return;
	}
	// There is a map element (vector), but need an new trust node
	std::vector<TrustNode *> *v = trust->m_trustMap[dstMac];
	TrustNode *tn = v->back();
	uint32_t timeSlotBegin = trust->FindTimeSlot(Simulator::Now ().GetSeconds (), GetTimeUnit ());
	//std::cout<<"Time Slot Begin"<<timeSlotBegin<<std::endl;
	if (tn->GetTime () < timeSlotBegin)
	{
		//std::cout<<"Inside TN current time: "<<tnn.GetTime()<<" The timeSlotBegin: "<<timeSlotBegin<<std::endl;
		TrustNode *newTn = new TrustNode ();
		newTn->SetSeq (tn->GetSeq () + 1);
		newTn->SetTime (timeSlotBegin);
		//std::cout<<"The time of the trust node is: "<<newTn->GetTime()<<std::endl;
		
		newTn->UpdateSentPackets ();
		v->push_back(newTn);
		//trust->m_trustMap[dstMac] = v;
		
		//Simulator::Schedule (Seconds(newTn->GetTime () + trust->GetTimeUnit ()), &Trust::SetDroppedPackets, trust, newTn);
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::SetDroppedPackets, trust, newTn);
		Simulator::Schedule (Seconds(trust->GetTimeUnit ()), &Trust::TrustEvaluation, trust, dstMac, newTn->GetSeq ());
		
		
	}
	else
	{
		v->back()->UpdateSentPackets ();
		//trust->m_trustMap[dstMac] = v;
		//std::cout<<"Trust Node tn value:"<<tn->GetSeq()<<"DST MAC: "<<dstMac<<"Node ID: "<<node->GetId ()<<std::endl;
	}
	
}



void
Trust::SetDroppedPackets (TrustNode *tn)
{
	tn->SetDroppedPackets ();
	//std::cout<<"Drop Evaluation: "<<Simulator::Now().GetSeconds()<<std::endl;
	
}


void
Trust::TrustEvaluation (std::string mac, uint32_t seq) 
{
	//std::cout<<"Trust Evaluation: "<<Simulator::Now().GetSeconds()<<std::endl;
	Ptr<Node> node = GetNode ();
	Ptr<Trust> trust = node->GetObject<Trust> ();
	std::vector<TrustNode *> *v = trust->m_trustMap[mac];
	
	////////// To convert pointer to string
	const void * address = static_cast<const void*>(trust);
	std::stringstream ss;
	ss << address;  
	std::string trust1 = ss.str(); 
	
	// Convert SrcIP to string
	Ipv4Address ip = trust->GetSrcIp ();
	std::stringstream ssip;
    ssip << ip;
	std::string srcIp = ssip.str();
	//std::cout<<"The src ip address is: "<<srcIp<<std::endl;
	//std::cout<<"-I have been called now"<<trust1<<std::endl;
	
	
	
	//
	//std::cout<<"The PTR NODE POINTER ISS "<<node<<std::endl;
	
	
	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
	Ptr<Socket> source = Socket::CreateSocket (node, tid);
	uint16_t dstPort = trust->GetListenerPort ();
	
	
	m_ts.TrustSchemeManager (source, dstPort, v, mac, srcIp, seq, trust->GetTimeUnit ());
	
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Indirect Trust
void
Trust::RunListener ()
{
	Ptr<Node> node = GetNode ();
	Ptr<Trust> trust = node->GetObject<Trust> ();
	//std::cout<<"The UDP port of the listener is: "<<GetListenerPort ()<<std::endl;
	
	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
	Ptr<Socket> recvSocket = Socket::CreateSocket (node, tid);
	InetSocketAddress socketAddress = InetSocketAddress (Ipv4Address::GetAny (), GetListenerPort ()); // GetAny return 0.0.0.0 which means any address including broadcast
	recvSocket->Bind (socketAddress);
	recvSocket->SetRecvCallback (MakeCallback (&Trust::RecommendationReceive, this));

}

void 
Trust::RecommendationReceive (Ptr<Socket> socket)
{
	//std::cout<<"Callback: I have been called at "<<Simulator::Now ().GetSeconds ()<<std::endl;
	Ptr<Node> node = GetNode ();
	Ptr<Trust> trust = node->GetObject<Trust> ();
	Ptr<Packet> packet;
	
	
	
	// Prepare the parameters for the recommendation response sending
	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
	Ptr<Socket> source = Socket::CreateSocket (node, tid);
	uint16_t dstPort = trust->GetListenerPort ();
	
	
	while (packet = socket->Recv ())
    {
      Ptr<Packet> p = packet->Copy();
	  Ipv4Header packetHeader;
	  p->RemoveHeader (packetHeader);
	  
	  //std::cout<<"Callback:  The packet source IP is: "<<packetHeader.GetSource ()<<std::endl;
	  
	  //std::cout<<"The packet pointer: "<<packet<<std::endl;
	  uint8_t buffer[packet->GetSize()];
      packet->CopyData(buffer, packet->GetSize());
	  //std::cout<<"The packet size is: "<<packet->GetSize()<<std::endl;
	  
	  for (uint32_t i=0; i<=packet->GetSize()-1; i++)
	  {
		  std::cout<<buffer[i];
	  }
	  std::cout<<std::endl;
	  NS_LOG_UNCOND ("Received one packet!");
	  
	  // Processing the received packet
	  std::string packetType;
	  std::string trusteeMac;
	  std::string reqIp;
	  std::string seq;
	  
	  
	  // 1. Reading the packet content
	  uint32_t i = 1;
	  if (buffer[0] == '<')
	  {
		  
		  while (buffer[i] != '>')
		  {
			  packetType.push_back(buffer[i]);
			  i++;
		  }
		  i+=2;
		  while (buffer[i] != '>')
		  {
			  trusteeMac.push_back(buffer[i]);
			  i++;
		  }
		  i+=2;
		  while (buffer[i] != '>')
		  {
			  reqIp.push_back(buffer[i]);
			  i++;
		  }
		  i+=2;
		  while (buffer[i] != '>')
		  {
			  seq.push_back(buffer[i]);
			  i++;
		  }
		  
	  }
	  
	  //std::cout<<"The packet type: "<<packetType<<std::endl;
	  //std::cout<<"The trustee MAC: "<<trusteeMac<<std::endl;
	  //std::cout<<"The src IP: "<<reqIp<<std::endl;
	  //std::cout<<"The seq: "<<seq<<std::endl;
	  
	  // 2. Processing the packet type
	  if (packetType == "Req")
	  {
		  
		  
		  //std::cout<<"before the count function"<<std::endl;
		  
		  if (trust->m_trustMap.count(trusteeMac) == 0)
		  {
			  //std::cout<<"count function is zero: "<<trusteeMac<<std::endl;
			  return;
		  }
		  else
		  {
			  //std::cout<<"Inside the else statement before the call"<<std::endl;
			  std::vector<TrustNode *> *v = trust->m_trustMap[trusteeMac];
			  m_ts.RecommendationRequestProcesser (v, trusteeMac, reqIp, seq, source, dstPort);
		  }
		  
	  }
	  else if (packetType == "Res")
	  {
		  i+=2;
		  std::string recommendation;
		  std::string uncertainty;
		  while (buffer[i] != '>')
		  {
			  recommendation.push_back(buffer[i]);
			  i++;
		  }
		  i+=2;
		  while (buffer[i] != '>')
		  {
			  uncertainty.push_back(buffer[i]);
			  i++;
		  }
		  
		  
		  // 3. Check if this response is for me
		  // 3.1. Convert SrcIP to string
		  Ipv4Address ip = trust->GetSrcIp ();
		  std::stringstream ssip;
		  ssip << ip;
		  std::string nodeIp = ssip.str();
		  
		  if (reqIp != nodeIp)      //check the reqIp is equal to mine
		  {
			  //std::cout<<"nodeIp and reqIp are not equal"<<std::endl;
			  //std::cout<<"nodeIp is: "<<nodeIp<<std::endl;
			  //std::cout<<"reqIp is: "<<reqIp<<std::endl;
			  return;
		  }
		  
		  if (trust->m_trustMap.count(trusteeMac) == 0)  //check if there already trust record for this request
		  {
			  //std::cout<<"No trust record for the trustee in the received recommendation response"<<std::endl;
			  return;
		  }
		  else
		  {
			  std::vector<TrustNode *> *v = trust->m_trustMap[trusteeMac];
			  //std::string::size_type sz; //
			  uint32_t seq_int = std::stoi (seq);    // stoi is a function to convert a string to int
			  TrustNode *tn = v->back();
			  if (tn->GetSeq () >= seq_int)  // check if my seq is greater or equal to the receive res
			  {
				  //std::cout<<"It is a valid recommendation response"<<std::endl;  
				  double recommendation_double = std::stod(recommendation) /100;
				  double uncertainty_double = std::stod(uncertainty) /100;
				  m_ts.RecommendationResponseProcesser (v, seq_int, recommendation_double, uncertainty_double);
			  
			  }
			  
		  }
	  
    
	
		}
		else  // It is not Req nor Res
		{
			return;
		}
	}


}
}
