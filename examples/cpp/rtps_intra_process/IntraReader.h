/**
 * @file IntraReader.h
 *
 */

#ifndef INTRADELIVERY_INTRAREADER_H_
#define INTRADELIVERY_INTRAREADER_H_

#include <fastdds/rtps/participant/RTPSParticipant.hpp>
#include <fastdds/rtps/history/ReaderHistory.hpp>
#include "fastdds/rtps/reader/ReaderListener.hpp"

class IntraReader
{
public:

    IntraReader();
    virtual ~IntraReader();
    eprosima::fastdds::rtps::RTPSParticipant* mp_participant;
    eprosima::fastdds::rtps::RTPSReader* mp_reader;
    eprosima::fastdds::rtps::ReaderHistory* mp_history;
    bool init(); //Initialization
    bool reg(); //Register
    void run(); //Run
    void stop();

    class MyListener : public eprosima::fastdds::rtps::ReaderListener
    {
    public:

        MyListener()
            : n_received(0)
            , n_matched(0)
        {
        }

        ~MyListener()
        {
        }

        void on_new_cache_change_added(
                eprosima::fastdds::rtps::RTPSReader* reader,
                const eprosima::fastdds::rtps::CacheChange_t* const change) override;
        void on_reader_matched(
                eprosima::fastdds::rtps::RTPSReader*,
                const eprosima::fastdds::rtps::MatchingInfo& info) override
        {
            if (info.status == eprosima::fastdds::rtps::MATCHED_MATCHING)
            {
                n_matched++;
            }
        }

        uint32_t n_received;
        uint32_t n_matched;

    private:

        using eprosima::fastdds::rtps::ReaderListener::on_reader_matched;
    }
    m_listener;

private:
    bool running_;
};

#endif /* INTRADELIVERY_INTRAREADER_H_ */
