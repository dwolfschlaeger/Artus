
#pragma once

#include "Artus/Core/interface/ProducerBase.h"


/**
   \brief GlobalProducer, for PU weight.
   
   This producer calculates a weight for a event based on the number of pileup truth
   interaction in a MC skim (m_genEventMetadata->numPUInteractionsTruth) and the
   weights from an output file of the weightCalc.py script.
   
   This producer needs the following config tags:
    - PileupWeightFile
*/

template<class TTypes>
class PUWeightProducer: public ProducerBase<TTypes>
{

public:

	typedef typename TTypes::event_type event_type;
	typedef typename TTypes::product_type product_type;
	typedef typename TTypes::setting_type setting_type;


	virtual std::string GetProducerId() const ARTUS_CPP11_OVERRIDE {
		return "pu_weights";
	}

	virtual void Init(setting_type const& settings) ARTUS_CPP11_OVERRIDE {
		ProducerBase<TTypes>::Init(settings);
		
		const std::string s = "pileup";
		LOG(INFO) << "Loading pile-up weights (" << s << "):\n  " << settings.GetPileupWeightFile();
		TFile file(settings.GetPileupWeightFile().c_str(), "READONLY");
		TH1D* pileuphisto = (TH1D*) file.Get(s.c_str());

		m_pileupweights.clear();
		for (int i = 1; i <= pileuphisto->GetNbinsX(); ++i)
			m_pileupweights.push_back(pileuphisto->GetBinContent(i));
		m_bins = 1.0 / pileuphisto->GetBinWidth(1);
		delete pileuphisto;
		file.Close();
	}

	virtual void Produce(event_type const& event, product_type& product,
	                     setting_type const& settings) const ARTUS_CPP11_OVERRIDE
	{
		assert(event.m_genEventMetadata != NULL);
		double npu = event.m_genEventMetadata->numPUInteractionsTruth;
		if (npu < m_pileupweights.size())
			product.m_weights["puWeight"] = m_pileupweights.at(int(npu * m_bins));
		else
			product.m_weights["puWeight"] = 0.0;
	}


private:
		std::vector<double> m_pileupweights;
		double m_bins;

};

