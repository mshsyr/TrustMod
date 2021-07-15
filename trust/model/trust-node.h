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