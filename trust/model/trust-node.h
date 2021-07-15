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
 
#ifndef TRUST_NODE_H
#define TRUST_NODE_H

#include "ns3/scheme-parameters.h"
#include "ns3/received-recommendations.h"
#include <stdint.h>

namespace ns3
{

class TrustNode
{
private:
	uint32_t m_sentPackets;  // Number of sent packet per time unit
	uint32_t m_forwardedPackets; // Number of forwarded packet per time unit
	uint32_t m_droppedPackets; // Number of dropped packet per time unit
	double m_uncertainty;
	double m_directTrustValue;
	double m_indirectTrustValue;
	double m_indirectUncertainty;
	double m_overallTrust;
	uint32_t m_time;
	uint32_t m_seq; 
	bool m_recommendationSent;
	std::vector<ReceivedRecommendations *> *m_recommendations;
	SchemeParameters *m_sp;

public:
	TrustNode ();
	virtual ~TrustNode ();
	
	uint32_t GetSentPackets ();
	void UpdateSentPackets ();
	
	uint32_t GetForwardedPackets ();
	void UpdateForwardedPackets ();
	
	uint32_t GetDroppedPackets ();
	void SetDroppedPackets (); // To calculate the drop packets at the end of the time unit
	
	double GetUncertainty ();
	void SetUncertainty (double);

    double GetDirectTrustValue ();
	void SetDirectTrustValue (double);

	double GetIndirectTrustValue ();
	void SetIndirectTrustValue (double);

	double GetTrustValue ();
	void SetTrustValue ();
	
	uint32_t GetTime ();
	void SetTime (uint32_t);
	
	uint32_t GetSeq ();
	void SetSeq (uint32_t);
	
	bool GetRecommendationSent ();
	void SetRecommendationSent (bool);
	
	std::vector <ReceivedRecommendations *> * GetRecommendations ();
	SchemeParameters *GetSchemeParameters ();
	
	double GetOverallTrust ();
	void SetOverallTrust (double);
	
	double GetIndirectUncertainty ();
	void SetIndirectUncertainty (double);
	
};

}
#endif 