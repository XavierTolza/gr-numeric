//
// Created by xavier on 27/07/18.
//

#ifndef GR_FFLY_TIME_TRACKER_H
#define GR_FFLY_TIME_TRACKER_H

#include <gnuradio/block.h>
#include <vector>
#include <gnuradio/tags.h>

#define TIME_TRACKER_TRACK_INITIALIZED

class time_tracker_t {
public:
    typedef struct time_t{
        uint64_t ent;
        double frac;
    }time_t;
private:
    gr::block *parent;
    uint64_t last_tag_index, seconds_int;
    double seconds_frac;
    uint64_t rx_rate;
    uint8_t input_index;
#ifdef TIME_TRACKER_TRACK_INITIALIZED
    uint8_t initialized;
#endif

    time_t add_time(time_t t1, time_t t2);

public:
    time_tracker_t(gr::block * parent, uint8_t input, uint64_t rx_rate);

    void refresh(std::vector<gr::tag_t> tags);

    time_t get_sample_time_rel(uint32_t relative_sample_index);
    time_t get_sample_time_abs(uint64_t absolute_sample_index);

    time_t get_last_tag_value();

#ifdef TIME_TRACKER_TRACK_INITIALIZED
    bool is_initialized();
#endif
};


#endif //GR_FFLY_TIME_TRACKER_H
