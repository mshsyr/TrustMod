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
 
#include "ns3/trust-node.h"
#include "ns3/simulator.h"
#include <iostream>

namespace ns3 
{

TrustNode::TrustNode ()
{
	m_sentPackets = 0;
	m_forwardedPackets = 0;
	m_droppedPackets = 0;
	m_uncertainty = 1;
	m_directTrustValue = 0;
	m_indirectTrustValue = -1;
	m_indirectUncertainty = -1;
	m_overallTrust = -1;
	m_time = 0;
	m_recommendationSent = false;
	m_sp = new SchemeParameters ();
	m_recommendations = new std::vector<ReceivedRecommendations *> ();
	
}

TrustNode::~TrustNode ()
{
}


// m_sentPackets
uint32_t 
TrustNode::GetSentPackets ()
{
	return m_sentPackets;
}

void 
TrustNode::UpdateSentPackets ()
{
	//std::cout<<"Inside the sent Packets Update before " <<m_sentPackets<<std::endl;
	m_sentPackets++;
	//std::cout<<"Inside the sent Packets Update " <<m_sentPackets<<std::endl;
}


// m_forwardedPackets
uint32_t 
TrustNode::GetForwardedPackets ()
{
	return m_forwardedPackets;
}

void 
TrustNode::UpdateForwardedPackets ()
{
	m_forwardedPackets++;
}




// m_time
uint32_t 
TrustNode::GetTime ()
{
	return m_time;
}

void 
TrustNode::SetTime (uint32_t time)
{
	m_time = time;
}

//m_seq
uint32_t 
TrustNode::GetSeq ()
{
	return m_seq;
}

void 
TrustNode::SetSeq (uint32_t seq)
{
	m_seq = seq;
}


//m_droppedPackets
uint32_t 
TrustNode::GetDroppedPackets ()
{
	return m_droppedPackets;
}

void 
TrustNode::SetDroppedPackets ()
{

	//std::cout<<"I have been called, time is: "<<Simulator::Now().GetSeconds()<<std::endl;
	if(m_sentPackets == 0)
	{
		m_droppedPackets =0;
		//std::cout<<"SetDroppedPackets- first if: "<<m_droppedPackets<<std::endl;
	}
	else if (m_forwardedPackets >= m_sentPackets)
	{
		m_droppedPackets = 0;
		//std::cout<<"SetDroppedPackets- second if: "<<m_droppedPackets<<std::endl;
	}
	else
	{
		m_droppedPackets = m_sentPackets - m_forwardedPackets;
		//std::cout<<m_time<<" SetDroppedPackets- else: "<<m_droppedPackets<<"Sent "<<m_sentPackets<<"Forwarded "<<m_forwardedPackets<<"Sequence "<<m_seq<<std::endl;
	}
}	



//Get a pointer to the scheme parameters object
SchemeParameters *
TrustNode::GetSchemeParameters ()
{
	return m_sp;
}


//m_directTrustValue
void
TrustNode::SetDirectTrustValue (double directTrustValue)
{
	m_directTrustValue = directTrustValue;
}

double
TrustNode::GetDirectTrustValue ()
{
	return m_directTrustValue;
}


//m_uncertainty
double 
TrustNode::GetUncertainty ()
{
	return m_uncertainty;
}

void 
TrustNode::SetUncertainty (double uncertainty)
{
	m_uncertainty = uncertainty;
}


//m_recommendationSent
bool 
TrustNode::GetRecommendationSent ()
{
	return m_uncertainty;
}

void 
TrustNode::SetRecommendationSent (bool recommendationSent)
{
	m_recommendationSent = recommendationSent;
}


//m_recommendations
std::vector <ReceivedRecommendations *> *
TrustNode::GetRecommendations ()
{
	return m_recommendations;
}



//m_indirectTrustValue
void
TrustNode::SetIndirectTrustValue (double indirectTrust)
{
	m_indirectTrustValue = indirectTrust;
}

double
TrustNode::GetIndirectTrustValue ()
{
	return m_indirectTrustValue;
}


//m_overallTrust
double 
TrustNode::GetOverallTrust ()
{
	return m_overallTrust;
}

void 
TrustNode::SetOverallTrust (double overallTrust)
{
	overallTrust = round (overallTrust * 100) / 100;
	m_overallTrust = overallTrust;
}


//m_indirectUncertainty
double 
TrustNode::GetIndirectUncertainty ()
{
	//std::cout<<"From the Get method "<<m_indirectUncertainty<<std::endl;
	return m_indirectUncertainty;
}

void 
TrustNode::SetIndirectUncertainty (double indirectUncertainty)
{
	
	//std::cout<<"before the round function "<<indirectUncertainty<<std::endl;
	indirectUncertainty = round (indirectUncertainty *100)/100;
	//std::cout<<"Aftert the round function "<<indirectUncertainty<<std::endl;
	m_indirectUncertainty = indirectUncertainty;
}


}