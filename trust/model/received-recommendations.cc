#include "ns3/received-recommendations.h"
#include <iostream>


namespace ns3 
{

ReceivedRecommendations::ReceivedRecommendations ()
{
	m_seq = 0;
	m_receivedTrust = 0;
	m_receivedUncertainty = 0;

	
}

ReceivedRecommendations::~ReceivedRecommendations ()
{
}
	

//m_seq
uint32_t 
ReceivedRecommendations::GetSeq()
{
	return m_seq;
}

void 
ReceivedRecommendations::SetSeq ()
{
	m_seq++;
}
	
	
//m_receivedTrust	
double 
ReceivedRecommendations::GetReceivedTrust ()
{
	return m_receivedTrust;
}

void 
ReceivedRecommendations::SetReceivedTrust (double receivedTrust)
{
	m_receivedTrust = receivedTrust;
}


//m_receivedUncertainty
double 
ReceivedRecommendations::GetReceivedUncertainty ()
{
	return m_receivedUncertainty;
}


void 
ReceivedRecommendations::SetReceivedUncertainty (double receivedUncertainty)
{
	m_receivedUncertainty = receivedUncertainty;
}




}