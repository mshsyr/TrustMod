#ifndef RECEIVED_RECOMMENDATIONS_H
#define RECEIVED_RECOMMENDATIONS_H

#include <stdint.h>

namespace ns3
{

class ReceivedRecommendations
{
private:
	uint32_t m_seq;
	double m_receivedTrust;  
	double m_receivedUncertainty; 
		
public:
	ReceivedRecommendations ();
	virtual ~ReceivedRecommendations ();
	
	uint32_t GetSeq();
	void SetSeq ();
	
	double GetReceivedTrust ();
	void SetReceivedTrust (double);
	
	double GetReceivedUncertainty ();
	void SetReceivedUncertainty (double);
	
	
};

}
#endif 