#pragma once

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "FilterBase.h"
#include "FilterResult.h"

template<class TTypes>
class EventPipeline;

/*
 The base class to implement your own EventConsumer which run within an EventPipeline.
 This class gets the FilterResult, the created meta data, the settings and the input event ifself
 and can create the output you desire.
 It is not allowed to write to the settings, filer, event and meta data.

 */

template<class TTypes>
class EventConsumerBase: public boost::noncopyable {
public:

	typedef typename TTypes::event_type event_type;
	typedef typename TTypes::local_meta_type local_meta_type;
	typedef typename TTypes::global_meta_type global_meta_type;
	typedef typename TTypes::setting_type setting_type;

	virtual ~EventConsumerBase() {
	}

	/*
	 * Called before the first Event is passed to this consumer
	 */
	virtual void Init(EventPipeline<TTypes> * pipeline) {
		m_pipeline = pipeline;
	}

	/* this method is only called for events which have passed the filter imposed on the
	 * pipeline
	 */
	virtual void ProcessFilteredEvent(event_type const& event,
			global_meta_type const& globalMetaData,
			local_meta_type const& localMetaData) {
	}

	/*
	 *  this method is called for all events
	 */
	virtual void ProcessEvent(event_type const& event,
			global_meta_type const& globalMetaData,
			local_meta_type const& localMetaData, FilterResult & result) {
	}

	/*
	 *  this method is called for pipeline which do not operate on event input but on data
	 *  generated by preceeding pipelines
	 */
	virtual void Process() {
	}

	/*
	 * Called after the last event. Overwrite this to store your histograms etc. to disk
	 */
	virtual void Finish() = 0;

	/*
	 * Must return a unique id of the EventConsumer.
	 */
	virtual std::string GetId() const {
		return "default";
	}

	/*
	 * Return a reference to the settings used for this consumer
	 */
	setting_type const& GetPipelineSettings() const {
		return this->m_pipeline->GetSettings();
	}

protected:
	EventPipeline<TTypes> * m_pipeline;
};
