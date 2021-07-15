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
 
#ifndef TRUST_SCHEME_H
#define TRUST_SCHEME_H

#include "ns3/socket.h"
#include "ns3/trust-node.h"
#include <stdint.h>
#include "ns3/ptr.h"



namespace ns3
{

class TrustScheme
{
private:
	uint32_t m_alpha;  
	uint32_t m_beta; 
	double m_lambda; 
	double m_uncertainty;
	double m_directTrustValue;
	double m_indirectTrustValue;
	double m_trustValue;
	uint32_t m_malicious; //LTMS(2)
	uint32_t m_cycle; //LTMS(2)
	double m_threshold1 = 0.5; //LTMS(2)
	double m_threshold2 = 0.85; //LTMS(2)
	
public:
	TrustScheme ();
	virtual ~TrustScheme ();
	void TrustSchemeManager (Ptr<Socket>, uint16_t, std::vector<TrustNode *> *, std::string, std::string, uint32_t, uint32_t); //String for the trustee, string for the src IP, int for seq
	void DirectTrustEvaluation (std::vector<TrustNode *> *, uint32_t);
	void UncertaintyEvalaution (std::vector<TrustNode *> *, uint32_t);
	bool IndirectTrustEvaluation (std::vector<TrustNode *> *, uint32_t);
	void RecommendationSender (Ptr<Socket>, uint16_t, uint8_t [], int);
	void RecommendationsEvaluation (std::vector<ReceivedRecommendations *> *, TrustNode *);
	void RecommendationRequestProcesser (std::vector<TrustNode *> *, std::string, std::string, std::string, Ptr<Socket>, uint16_t); // To receive and process the recomm req and call the recommedation sender
	void RecommendationResponseProcesser (std::vector<TrustNode *> *, uint32_t, double, double);
	void OverallTrustEvaluation (TrustNode *);
	
};

}
#endif 