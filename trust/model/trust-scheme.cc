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
 
#include "ns3/trust-scheme.h"
#include "ns3/simulator.h"
#include <iostream>
#include <numeric> // to sum the content of a vector using accumulate method


namespace ns3 
{

TrustScheme::TrustScheme ()
{
	m_lambda = 0.9;
	m_malicious = 0;
	m_cycle = 0;
	m_threshold1 = 0.5; 
	m_threshold2 = 0.85; 

	
	

	
}

TrustScheme::~TrustScheme ()
{
}
	
void 
TrustScheme::TrustSchemeManager(Ptr<Socket> source, uint16_t dstPort, std::vector<TrustNode *> * v, std::string trusteeMac, std::string srcIp, uint32_t seq, uint32_t timeUnit)
{
	this->DirectTrustEvaluation (v, seq);
	this->UncertaintyEvalaution (v, seq);
	if (this->IndirectTrustEvaluation (v, seq))
	{
		// Build the message buffer
		//std::cout<<"The serialization has been called"<<std::endl;
	
		std::stringstream ssint;
		ssint<<seq;
		std::string seqString = ssint.str();
		//trusteeMac.pop_back (); // The mac and ip have special character at the end. pop_back to remove the last char
		//srcIp.pop_back ();
		std::string reqType = "<Req>";
	
		std::string fullString =  reqType + '<' + trusteeMac + '>' + '<' + srcIp + '>'  + '<' + seqString + '>';
		std::vector<uint8_t> myVector(fullString.begin(), fullString.end());
		uint8_t *p = &myVector[0];
		int size = myVector.size();
	
		uint8_t buffer[size];
		for(int i=0; i<=size-1; i++)
		{
		buffer[i] = p[i];
		//std::cout<<buffer[i]<<std::endl;
		

		
		}
		
		//1. Call Recommendation Request Sender method
		this->RecommendationSender (source, dstPort, buffer, size);
		
		//2. Update the  m_recommendationSent variable
		TrustNode *tn = v->at(seq-1);
		tn->SetRecommendationSent (true);
		
		//3. Schedule an event to evaluate the received recommendations
		std::vector<ReceivedRecommendations *> * recomm = v->at(seq-1)->GetRecommendations ();
		//std::cout<<"The recomm is "<<recomm<<std::endl;
		Simulator::Schedule (Seconds(timeUnit), &TrustScheme::RecommendationsEvaluation, this, recomm, tn);
		Simulator::Schedule (Seconds(timeUnit), &TrustScheme::OverallTrustEvaluation, this, tn);
		
	}
		
}
	
// Beta distribution implementation for direct trust.
/*
void 
TrustScheme::DirectTrustEvaluation (std::vector<TrustNode *> * v, uint32_t seq)
{
	TrustNode *tn = v->at(seq-1);
	SchemeParameters *sp = tn->GetSchemeParameters ();
	
	if (seq == 1)
	{
		sp->m_alpha = tn->GetForwardedPackets ();
		sp->m_beta = tn->GetDroppedPackets ();
		if (sp->m_beta == 0 && sp->m_alpha == 0)
		{
			tn->SetDirectTrustValue (0);
		}
		else
		{
			double trustValue = (sp->m_alpha) / (sp->m_alpha + sp->m_beta);
			trustValue = round(trustValue * 1000)/1000;
			tn->SetDirectTrustValue (trustValue);

		}
	}
	else
	{
		TrustNode *previousTn = v->at(seq-2);
		SchemeParameters *previousSp = previousTn->GetSchemeParameters ();
		sp->m_alpha =  m_lambda * previousSp->m_alpha + tn->GetForwardedPackets ();
		sp->m_beta = m_lambda * previousSp->m_beta + tn->GetDroppedPackets ();
		
		if (sp->m_beta == 0 && sp->m_alpha == 0)
		{
			tn->SetDirectTrustValue (0);
		}
		else
		{
			double trustValue = (sp->m_alpha) / (sp->m_alpha + sp->m_beta);
			trustValue = round (trustValue * 1000)/1000;
			tn->SetDirectTrustValue (trustValue);
		}
			
	}

}
*/
//LTMS(1) Implementation
/*
void 
TrustScheme::DirectTrustEvaluation (std::vector<TrustNode *> * v, uint32_t seq)
{
	TrustNode *tn = v->at(seq-1);
	SchemeParameters *sp = tn->GetSchemeParameters ();
	
	if (seq == 1)
	{
		sp->m_alpha = tn->GetForwardedPackets ();
		sp->m_beta = tn->GetDroppedPackets ();
		sp->b = 0;
		sp->d = 0;
		if (sp->m_alpha == 0)
		{
			tn->SetDirectTrustValue (0);
		}
		else
		{
			double trustValue = (sp->m_alpha) / (sp->m_alpha + sp->m_beta);
			trustValue = round(trustValue * 1000)/1000;
			tn->SetDirectTrustValue (trustValue);

		}
	}
	else
	{
		TrustNode *previousTn = v->at(seq-2);
		SchemeParameters *previousSp = previousTn->GetSchemeParameters ();
		if (previousSp->b <= 0 && previousSp->d > 0)
		{
			sp->m_alpha =  m_lambda * (previousSp->m_alpha + previousSp->b) + tn->GetForwardedPackets ();
			sp->m_beta = m_lambda * (previousSp->m_beta + previousSp->d) + tn->GetDroppedPackets ();
		
			sp->b = sp->m_alpha - previousSp->m_alpha;
			sp->d = sp->m_beta - previousSp->m_beta;
		}
		else 
		{
			sp->m_alpha =  m_lambda * (previousSp->m_alpha) + tn->GetForwardedPackets ();
			sp->m_beta = m_lambda * (previousSp->m_beta) + tn->GetDroppedPackets ();
		
			sp->b = sp->m_alpha - previousSp->m_alpha;
			sp->d = sp->m_beta - previousSp->m_beta;
		}
		
		
		if (sp->m_alpha > 0)
		{
			double trustValue = (sp->m_alpha) / (sp->m_alpha + sp->m_beta);
			trustValue = round (trustValue * 1000)/1000;
			tn->SetDirectTrustValue (trustValue);
		}
		else
		{
			tn->SetDirectTrustValue (0);
		}
			
	}

}
*/

// LTMS(2) Implementation
void 
TrustScheme::DirectTrustEvaluation (std::vector<TrustNode *> * v, uint32_t seq)
{
	TrustNode *tn = v->at(seq-1);
	SchemeParameters *sp = tn->GetSchemeParameters ();
	
	if (seq == 1)
	{
		sp->m_alpha = m_lambda + tn->GetForwardedPackets (); // alpha(t-1) =1 becasue there is a predefined time unit of (1,1)
		sp->m_beta = m_lambda + tn->GetDroppedPackets (); // beta(t-1) =1
		sp->b = sp->m_alpha -1;
		sp->d = sp->m_beta -1;
		if (sp->m_alpha == 0)
		{
			tn->SetDirectTrustValue (0);
		}
		else
		{
			double trustValue = (sp->m_alpha) / (sp->m_alpha + sp->m_beta);
			trustValue = round(trustValue * 1000)/1000;
			tn->SetDirectTrustValue (trustValue);
		}
		return;
	}
	else
	{
		TrustNode *previousTn = v->at(seq-2);
		SchemeParameters *previousSp = previousTn->GetSchemeParameters ();
		if (previousSp->b <= 0 && previousSp->d > 0)
		{
			sp->m_alpha =  m_lambda * (previousSp->m_alpha + previousSp->b) + tn->GetForwardedPackets ();
			sp->m_beta = m_lambda * (previousSp->m_beta + previousSp->d) + tn->GetDroppedPackets ();
		
			sp->b = sp->m_alpha - previousSp->m_alpha;
			sp->d = sp->m_beta - previousSp->m_beta;
		}
		else 
		{
			sp->m_alpha =  m_lambda * (previousSp->m_alpha) + tn->GetForwardedPackets ();
			sp->m_beta = m_lambda * (previousSp->m_beta) + tn->GetDroppedPackets ();
		
			sp->b = sp->m_alpha - previousSp->m_alpha;
			sp->d = sp->m_beta - previousSp->m_beta;
		}
		
		
		if (sp->m_alpha > 0)
		{
			double trustValue = (sp->m_alpha) / (sp->m_alpha + sp->m_beta);
			trustValue = round (trustValue * 1000)/1000;
			tn->SetDirectTrustValue (trustValue);
		}
		else
		{
			tn->SetDirectTrustValue (0);
		}
			
	}
	
	// On-Off detector
	TrustNode *previousTn = v->at(seq-2);
	if (previousTn->GetDirectTrustValue () >= m_threshold1 && tn->GetDirectTrustValue () < m_threshold1)
	{
		if (m_malicious > 0)
		{
			m_cycle = seq - m_malicious - 1; //
			m_malicious = 0;
		}
		else 
		{
			m_malicious = seq - 1; //
		}
		std::cout<<"The malicious parameter = "<<m_malicious<<std::endl;
		std::cout<<"The cycle parameter = "<<m_cycle<<std::endl;
		
	}
	
	//Trust Evaluation
	if (m_cycle > 0 && previousTn->GetDirectTrustValue () < m_threshold2)
	{
		std::vector<double> trustVector;
		for (uint16_t i = seq - m_cycle - 1; i<=seq-1; i++) 
		{
			TrustNode *tempTn = v->at(i);
			trustVector.push_back(tempTn->GetDirectTrustValue ());
		}
		std::cout<<"Period: "<<m_cycle<<" Seq: "<<seq<<std::endl;
		
		double sum = std::accumulate(trustVector.begin (), trustVector.end (), 0.0);
		//std::cout<<"The sum value is: "<<sum<<std::endl;
		double shortTermTrust = sum/trustVector.size();
		//std::cout<<"The short term trust is: "<<shortTermTrust<<std::endl;
		double ltmsTrust = std::min (shortTermTrust, tn->GetDirectTrustValue ());
		//std::cout<<"The LTMS Trust is: "<<ltmsTrust<<std::endl;
		ltmsTrust = round (ltmsTrust * 1000)/1000;
		tn->SetDirectTrustValue (ltmsTrust);
	}
	else
	{
		m_cycle = 0;
	}

}

////////////////////////////




void
TrustScheme::UncertaintyEvalaution (std::vector<TrustNode *> * v, uint32_t seq)
{
	TrustNode *tn = v->at(seq-1);
	double directTrust = tn->GetDirectTrustValue ();
	double uncertainty;
	if (directTrust == 0 || directTrust == 1)
	{
		uncertainty = 0;
	}
	else
	{
		uncertainty = -(directTrust * log2 (directTrust) + (1-directTrust) * log2 (1-directTrust));
	}
	uncertainty = round (uncertainty*1000)/1000;
	tn->SetUncertainty (uncertainty);
}


bool 
TrustScheme::IndirectTrustEvaluation (std::vector<TrustNode *> * v, uint32_t seq)
{
	TrustNode *tn = v->at(seq-1);
	double uncertainty = tn->GetUncertainty ();
	if (uncertainty > 0.5)
		return false;     //////////////////////////////////////////////// To disable indirect trust evalution make it false
	else
		return false;
}


void 
TrustScheme::RecommendationSender (Ptr<Socket> source, uint16_t dstPort, uint8_t buffer[], int pktSize)
{
	InetSocketAddress remote = InetSocketAddress (Ipv4Address ("255.255.255.255"), dstPort);
	source->SetAllowBroadcast (true);
	source->Connect (remote);
	
	//Sending the request
	if (pktSize>0)
	{
		source->Send (Create<Packet> (buffer, pktSize));
		source->Close();
		//std::cout<<"A packet with the followin size has been sent"<<pktSize<<std::endl;
	}
	
	
}
	

void 
TrustScheme::RecommendationsEvaluation (std::vector<ReceivedRecommendations *> * recomm, TrustNode *tn)
{
	// Check if the vector is empty
	if (recomm->empty ())
	{
		//std::cout<<"RR The vector is empty"<<std::endl;
		tn->SetIndirectTrustValue (0);
		return;
	}
	else
	{
		double indirectTrust;
		double uncertainty;
		double sumRecomm = 0;
		double sumUncertainty = 0;
		for (uint32_t i =0; i<=recomm->size()-1; i++)
		{
			sumRecomm = sumRecomm + recomm->at(i)->GetReceivedTrust();
			sumUncertainty = sumUncertainty + recomm->at(i)->GetReceivedUncertainty();
			
			//std::cout<<"The received uncertianty is: "<<recomm->at(i)->GetReceivedUncertainty()<<std::endl;
		}
		
		indirectTrust = sumRecomm / recomm->size();
		uncertainty = sumUncertainty / recomm->size();
		
		tn->SetIndirectTrustValue(indirectTrust);
		//std::cout<<"The indirect uncertainty is "<<uncertainty<<std::endl;
		tn->SetIndirectUncertainty (uncertainty);
	}
	
	
}



void 
TrustScheme::RecommendationRequestProcesser(std::vector<TrustNode *> *v, std::string trusteeMac, std::string srcIp, std::string seq, Ptr<Socket> source, uint16_t dstPort)
{
	
	// Prepare the parameters
	//uint32_t seq_int = std::stoi(seq);
	//std::cout<<"RecommendationRequestProcesser has been called"<<std::endl;
	uint32_t v_size = v->size();
	if (v_size >= 2)
	{
		TrustNode *tn = v->at(v_size-2); // the index of last TN is (size-1); However, the trust value is still not calculated so we use at(size-2)
		double recommendation = tn->GetDirectTrustValue ();
		double uncertainty = tn->GetUncertainty ();
	
		recommendation = round (recommendation*1000); // without division by 100 to send int as a sting in the response packet
		uncertainty = round (uncertainty*1000);
	
		std::string r_string = std::to_string(int(recommendation));
		std::string u_string = std::to_string(int(uncertainty));	
	
	
	
	
		// Build the response packet buffer
		//std::cout<<"The serialization has been called"<<std::endl;
	
	
		//trusteeMac.pop_back (); // The mac and ip have special character at the end. pop_back to remove the last char
		//srcIp.pop_back ();
		
		std::string reqType = "<Res>";
	
		std::string fullString =  reqType + '<' + trusteeMac + '>' + '<' + srcIp + '>'  + '<' + seq + '>' + '<' + r_string + '>' + '<' + u_string + '>';
	
		std::vector<uint8_t> myVector(fullString.begin(), fullString.end());
		uint8_t *p = &myVector[0];
		uint32_t size = myVector.size();
	
		uint8_t buffer[size];
		for(uint32_t i=0; i<=size-1; i++)
		{
		buffer[i] = p[i];
		//std::cout<<buffer[i]<<std::endl;
		}
	
		this->RecommendationSender (source, dstPort, buffer, size);
	}
	else 
		return;

}


void 
TrustScheme::RecommendationResponseProcesser (std::vector<TrustNode *> * v, uint32_t seq, double recommendation, double uncertainty)
{
	//std::cout<<"RecommendationResponseProcesser"<<std::endl;
	TrustNode *tn = v->at(seq - 1);
	std::vector <ReceivedRecommendations *> * recommendationsVector = tn->GetRecommendations ();
	//std::cout<<"The recommendation vector size is : "<<recommendationsVector->size()<<std::endl;
	ReceivedRecommendations *rr = new ReceivedRecommendations ();
	rr->SetSeq();
	rr->SetReceivedTrust(recommendation);
	rr->SetReceivedUncertainty(uncertainty);
	recommendationsVector->push_back(rr);
	
}


void 
TrustScheme::OverallTrustEvaluation (TrustNode * tn)
{
	if (tn->GetRecommendationSent ())
	{
		if (tn->GetIndirectUncertainty () == -1)
		{
			tn->SetOverallTrust (tn->GetDirectTrustValue ());
		}
		else
		{
			double totalUncertainty = 2 - tn->GetUncertainty () - tn->GetIndirectUncertainty ();  // Calculate the overall certainty
			double overallTrust = ((1 - tn->GetUncertainty ()) / totalUncertainty) * tn->GetDirectTrustValue () + ((1 - tn->GetIndirectUncertainty ()) / totalUncertainty) * tn->GetIndirectTrustValue (); 
			tn->SetOverallTrust(overallTrust);
		
		}
		
	}
	else
	{
		tn->SetIndirectTrustValue (0);
		tn->SetOverallTrust (tn->GetDirectTrustValue ());
	}
}




}

