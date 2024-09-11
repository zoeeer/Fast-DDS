// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file IntraReader.cpp
 *
 */

#include "IntraReader.h"

#include "fastdds/rtps/reader/RTPSReader.hpp"
#include "fastdds/rtps/participant/RTPSParticipant.hpp"
#include "fastdds/rtps/RTPSDomain.hpp"

#include "fastdds/rtps/attributes/RTPSParticipantAttributes.hpp"
#include "fastdds/rtps/attributes/ReaderAttributes.hpp"
#include "fastdds/rtps/attributes/HistoryAttributes.hpp"
#include "fastdds/rtps/history/ReaderHistory.hpp"

#include "fastdds/rtps/builtin/data/TopicDescription.hpp"
#include "fastdds/dds/subscriber/qos/ReaderQos.hpp"

using namespace eprosima::fastdds;
using namespace eprosima::fastdds::rtps;

using eprosima::fastdds::rtps::BuiltinTransports;


IntraReader::IntraReader()
    : mp_participant(nullptr)
    , mp_reader(nullptr)
    , mp_history(nullptr)
{
}

IntraReader::~IntraReader()
{
    RTPSDomain::removeRTPSParticipant(mp_participant);
    delete(mp_history);
}

bool IntraReader::init()
{
    //CREATE PARTICIPANT
    RTPSParticipantAttributes PParam;
    PParam.builtin.discovery_config.discoveryProtocol = eprosima::fastdds::rtps::DiscoveryProtocol::SIMPLE;
    // PParam.builtin.use_WriterLivelinessProtocol = true;

    /**
     * @note configure Intra-Process mode
     * need to configure via XMLProfileManager::library_settings in the process (not here).
    */

    PParam.useBuiltinTransports = true;

    // TODO: Here we want to set Participant filters to achieve intraprocess_only, the expected effect is:
    // 1. If using BuiltinTransports::DEFAULT, no SHM resources will be created
    // 2. When using BuiltinTransports::DEFAULT or UDP, the created UDP transport will set TTL to 0 (preventing data packets from being sent outside the host)
    //
    // Currently, neither 1 nor 2 seem to be effective
    PParam.builtin.discovery_config.ignoreParticipantFlags ==
        (ParticipantFilteringFlags::FILTER_DIFFERENT_HOST | ParticipantFilteringFlags::FILTER_DIFFERENT_PROCESS);
    // Use BuiltinTransports::UDPv4 to avoid creating shared memory channels, making it easier to observe traffic
    PParam.setup_transports(BuiltinTransports::UDPv4);

    mp_participant = RTPSDomain::createParticipant(0, PParam);
    if (mp_participant == nullptr)
    {
        return false;
    }

    //CREATE READERHISTORY
    HistoryAttributes hatt;
    hatt.payloadMaxSize = 255;
    mp_history = new ReaderHistory(hatt);

    //CREATE READER
    ReaderAttributes ratt;
    mp_reader = RTPSDomain::createRTPSReader(mp_participant, ratt, mp_history, &m_listener);
    if (mp_reader == nullptr)
    {
        return false;
    }

    return true;
}

bool IntraReader::reg()
{
    std::cout << "Registering Reader" << std::endl;
    TopicDescription topic_desc;
    topic_desc.type_name = "string";
    topic_desc.topic_name = "IntraSimpleDiscovery";
    eprosima::fastdds::dds::ReaderQos reader_qos;
    reader_qos.m_durability.kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
    reader_qos.m_reliability.kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
    return mp_participant->register_reader(mp_reader, topic_desc, reader_qos);
}

void IntraReader::run()
{
    running_ = true;
    printf("Press Enter to stop the Reader.\n");
    std::cin.ignore();
}

void IntraReader::stop()
{
    running_ = false;
}

void IntraReader::MyListener::on_new_cache_change_added(
        RTPSReader* reader,
        const CacheChange_t* const change)
{
    printf("Received: %s at %p\n", change->serializedPayload.data, change->serializedPayload.data);
    reader->get_history()->remove_change((CacheChange_t*)change);
    n_received++;
}
