/**
 * @file IntraWriter.h
 *
 */

#ifndef INTRADELIVERY_INTRAWRITER_H_
#define INTRADELIVERY_INTRAWRITER_H_

#include "fastdds/rtps/writer/WriterListener.hpp"
#include <fastdds/rtps/participant/RTPSParticipant.hpp>
#include <fastdds/rtps/history/WriterHistory.hpp>

class IntraWriter
{
public:

    IntraWriter();
    virtual ~IntraWriter();
    eprosima::fastdds::rtps::RTPSParticipant* mp_participant;
    eprosima::fastdds::rtps::RTPSWriter* mp_writer;
    eprosima::fastdds::rtps::WriterHistory* mp_history;
    bool init(); //Initialize writer
    bool reg(); //Register the Writer
    void run(
            uint16_t samples,
            uint16_t interval);  //Run the Writer

private:
    class MyListener : public eprosima::fastdds::rtps::WriterListener
    {
    public:

        MyListener()
            : n_matched(0)
        {
        }

        ~MyListener()
        {
        }

        void on_writer_matched(
                eprosima::fastdds::rtps::RTPSWriter*,
                const eprosima::fastdds::rtps::MatchingInfo& info) override
        {
            if (info.status == eprosima::fastdds::rtps::MATCHED_MATCHING)
            {
                ++n_matched;
            }
        }

        int n_matched;

    private:

        using eprosima::fastdds::rtps::WriterListener::on_writer_matched;
    }
    m_listener;
};

#endif /* INTRADELIVERY_INTRAWRITER_H_ */
