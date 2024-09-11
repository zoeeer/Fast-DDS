// Copyright (c) 2024 MINIEYE

#ifndef INTRADELIVERY_SETTINGS_HPP_
#define INTRADELIVERY_SETTINGS_HPP_

#include "fastdds/LibrarySettings.hpp"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

using LibrarySettings = eprosima::fastdds::LibrarySettings;
using eprosima::fastdds::IntraprocessDeliveryType;
using eprosima::fastdds::dds::DomainParticipantFactory;

/**
 * @brief Set the intra-process communication mode for Fast-DDS library.
 *
 * The mode can be set to one of the following:
 * - 0: INTRAPROCESS_OFF
 * - 1: INTRAPROCESS_USER_DATA_ONLY
 * - 2: INTRAPROCESS_FULL
 *
 * @param mode An integer representing the desired intra-process communication mode.
 */
void set_intra_process_mode(int mode) {
    LibrarySettings library_settings;
    switch (mode) {
        case 0:
            library_settings.intraprocess_delivery = IntraprocessDeliveryType::INTRAPROCESS_OFF;
            break;
        case 1:
            library_settings.intraprocess_delivery = IntraprocessDeliveryType::INTRAPROCESS_USER_DATA_ONLY;
            break;
        case 2:
            library_settings.intraprocess_delivery = IntraprocessDeliveryType::INTRAPROCESS_FULL;
            break;
        default:
            break;
    }

    auto factory = DomainParticipantFactory::get_instance();
    factory->set_library_settings(library_settings);

}

#endif  // INTRADELIVERY_SETTINGS_HPP_
