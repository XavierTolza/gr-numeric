//
// Created by xavier on 27/07/18.
//

#include "time_tracker.h"

time_tracker_t::time_tracker_t(gr::block *parent, uint8_t input, uint64_t rx_rate) {
    this->parent = parent;
    input_index = input;
    this->rx_rate = rx_rate;
    last_tag_index = 0;
    seconds_frac = 0;
    seconds_int = 0;
#ifdef TIME_TRACKER_TRACK_INITIALIZED
    initialized = 0x0;
#endif
}


void time_tracker_t::refresh(std::vector<gr::tag_t> tags) {
    pmt::pmt_t item;
    std::string tag_key;

    for(std::vector<gr::tag_t>::iterator tag = tags.begin();tag != tags.end(); tag++){
        tag_key = pmt::symbol_to_string(tag->key);
        if(tag_key == "rx_time"){
            item = tag->value;
            if(pmt::is_tuple(item)){
                last_tag_index = tag->offset;
                seconds_int = pmt::to_uint64(pmt::tuple_ref(item,0));
                seconds_frac = pmt::to_double(pmt::tuple_ref(item,1));
#ifdef TIME_TRACKER_TRACK_INITIALIZED
                initialized |= 1<<0;
#endif
            }
        }
        if(tag_key == "rx_rate"){
            if(pmt::is_integer(tag->value)){
                rx_rate = pmt::to_uint64(tag->value);
            }
            if(pmt::is_number(tag->value)){
                rx_rate = (uint64_t) pmt::to_float(tag->value);
            }
#ifdef TIME_TRACKER_TRACK_INITIALIZED
            initialized |= 1<<1;
#endif
        }
    }

}

time_tracker_t::time_t time_tracker_t::get_sample_time_rel(uint32_t relative_sample_index) {
    uint64_t abs_sample_index = parent->nitems_read(input_index) + relative_sample_index;
    return this->get_sample_time_abs(abs_sample_index);
}

time_tracker_t::time_t time_tracker_t::get_sample_time_abs(uint64_t absolute_sample_index) {
    time_tracker_t::time_t result;
    uint64_t delta_samples = absolute_sample_index - this->last_tag_index;
    time_tracker_t::time_t delta_sec;
    delta_sec.ent = 0;
    delta_sec.frac = 0;

    delta_sec.ent = delta_samples / rx_rate;
    delta_sec.frac = (delta_samples % rx_rate) / (double)rx_rate;

    result = add_time(delta_sec,get_last_tag_value());
    return result;
}

time_tracker_t::time_t time_tracker_t::add_time(time_tracker_t::time_t t1, time_tracker_t::time_t t2) {
    time_t result;
    result.ent = t1.ent + t2.ent;
    result.frac = t1.frac + t2.frac;
    while(result.frac >= 1){
        result.ent ++;
        result.frac -= 1;
    }
    while(result.frac <= -1){
        result.ent --;
        result.frac += 1;
    }
    return result;
}

time_tracker_t::time_t time_tracker_t::get_last_tag_value() {
#ifdef TIME_TRACKER_TRACK_INITIALIZED
    if(!this->is_initialized()){
        throw std::exception();
    }
#endif
    time_t result;
    result.ent = seconds_int;
    result.frac = seconds_frac;
    return result;
}
#ifdef TIME_TRACKER_TRACK_INITIALIZED
bool time_tracker_t::is_initialized() {
    return initialized == 3;
}
#endif
