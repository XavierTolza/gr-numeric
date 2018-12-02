/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_numeric_PEAK_TAGGER_IMPL_H
#define INCLUDED_numeric_PEAK_TAGGER_IMPL_H

#include <numeric/peak_tagger.h>
#include "time_tracker.h"

#define numeric_PEAK_TAGGER_N_SAMPLES_INTERP   3
//#define numeric_PEAK_TAGGER_DEBUG

namespace gr {
    namespace numeric {

        typedef enum {
            WAITING_PEAK_END=0,
            WAITING_PEAK_START=1,
            SEARCHING_MAXIMUM=2,
        }peak_tagger_status_t;

        class peak_tagger_impl : public peak_tagger
        {
        private:
            pmt::pmt_t tag_name, additional_dict;
            const uint8_t input_size=4; // float input
            peak_tagger_status_t status;
            float previous_sample;
            float samples_buffer[3];
            bool need_save_next_sample;
            uint64_t max_index;
            time_tracker_t *time_tracker;

        public:
            peak_tagger_impl(std::string tag_name,bool forward_tags, pmt::pmt_t additional_dict);
            ~peak_tagger_impl();

            void compute_max_and_add_tag(uint64_t max_index);

            // Where all the action really happens
            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };

    } // namespace numeric
} // namespace gr

#endif /* INCLUDED_numeric_PEAK_TAGGER_IMPL_H */

