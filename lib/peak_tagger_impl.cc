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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "peak_tagger_impl.h"

namespace gr {
    namespace numeric {

        peak_tagger::sptr
        peak_tagger::make(std::string tag_name,bool forward_tags, pmt::pmt_t additional_dict)
        {
            return gnuradio::get_initial_sptr
                    (new peak_tagger_impl(tag_name, forward_tags, additional_dict));
        }

        /*
         * The private constructor
         */
        peak_tagger_impl::peak_tagger_impl(std::string tag_name,bool forward_tags, pmt::pmt_t additional_dict)
                : gr::sync_block("peak_tagger",
                                 gr::io_signature::make2(2, 2, sizeof(float), sizeof(float)),
#ifdef numeric_PEAK_TAGGER_DEBUG
                                 gr::io_signature::make2(2, 2, ((uint16_t )input_size)*((uint16_t)vect_length),
                                                         sizeof(uint8_t))

#else
                                 gr::io_signature::make(1, 1, sizeof(float))
#endif
        )
        {
            this->tag_name = pmt::string_to_symbol(tag_name);
            this->status = WAITING_PEAK_END;
            this->additional_dict = additional_dict;
            previous_sample = 0;
            need_save_next_sample = false;
            max_index = 0;
            time_tracker = new time_tracker_t(this,0,1);

            // propagate or not tags
            if(!forward_tags){
                this->set_tag_propagation_policy(this->TPP_DONT);
            }
        }

        /*
         * Our virtual destructor.
         */
        peak_tagger_impl::~peak_tagger_impl()
        {
            delete time_tracker;
        }

        int
        peak_tagger_impl::work(int noutput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items)
        {
            const uint8_t *data_in = (const uint8_t *) input_items[0];
            const float *peak_in = (const float *) input_items[0];
            const float *trigger_in = (const float *) input_items[1];
            uint8_t *out = (uint8_t *) output_items[0];
            float sample;
            uint64_t nitemsRead = this->nitems_read(0);
            bool trig_on;

            // update time tracker
            std::vector<tag_t> tags;
            this->get_tags_in_window(tags, 0, 0, (uint64_t)(noutput_items));
            time_tracker->refresh(tags);

            // first: forward data
            memcpy(out,data_in,noutput_items*this->input_size);

            // then, find peaks
            for(uint64_t sample_index=1;sample_index<noutput_items;){
                sample = peak_in[sample_index];
                trig_on = sample>trigger_in[sample_index];

                if(need_save_next_sample){
                    samples_buffer[2] = sample;
                    need_save_next_sample = false;
                }

                switch(status){
                    case WAITING_PEAK_END:
                        // on attend de redescendre en dessous du trigger sans chercher à en tagger le max
                        // (etat initial au cas où on démarre > trigger)
                        if(!trig_on){
                            status = WAITING_PEAK_START;
                        }
                        else{
                            previous_sample = sample;
                            sample_index++;
                        }
                        break;
                    case WAITING_PEAK_START:
                        if(trig_on){
                            status = SEARCHING_MAXIMUM;
                            samples_buffer[1] = -std::numeric_limits<float>::max();
                            max_index = (uint64_t)-1; // so we can check if a maximum value has been found? not compulsory
                        }
                        else{
                            previous_sample = sample;
                            sample_index++;
                        }
                        break;
                    case SEARCHING_MAXIMUM:
                        if(sample>samples_buffer[1]){
                            // looking for maximum
                            samples_buffer[0] = previous_sample;
                            samples_buffer[1] = sample;
                            need_save_next_sample = true; // next sample will be put on samples_buffer[2]
                            max_index = sample_index + nitemsRead;
                        }
                        if(!trig_on){
                            // compute maximum
                            this->compute_max_and_add_tag(max_index);

                            // finished
                            status=WAITING_PEAK_START;
                        }
                        previous_sample = sample;
                        sample_index++;
                        break;
                }

#ifdef numeric_PEAK_TAGGER_DEBUG
                ((uint8_t*) (output_items[1]))[sample_index] = status;
#endif
            }

            // Tell runtime system how many output items we produced.
            return noutput_items;
        }

        void peak_tagger_impl::compute_max_and_add_tag(uint64_t max_index) {
            float y1=samples_buffer[0],y2=samples_buffer[1],y3=samples_buffer[2];
            uint64_t nread = this->nitems_read(1);
            // on fait une interpolation avec un polynome d'ordre 2
            // Les calculs ont été faits à l'avance, on récupère direct le x maximum et le y=f(x)

            float fractional_position = (y1-y3)/(2*y3-4*y2+2*y1);
            float max_value = -(y3*(y3-8*y2-2*y1)+8*y2*(2*y2-y1)+y1*y1)/(8*y3-16*y2+8*y1);


            pmt::pmt_t tag_value = pmt::make_dict();
            if(time_tracker->is_initialized()){
                time_tracker_t::time_t time = time_tracker->get_sample_time_abs(max_index);
                pmt::pmt_t sample_time = pmt::make_tuple(pmt::from_uint64(time.ent),
                        pmt::from_double(time.frac));
                tag_value = pmt::dict_add(tag_value, pmt::string_to_symbol("sample_time"),sample_time);
            }
            tag_value = pmt::dict_add(tag_value, pmt::string_to_symbol("rel_index"),pmt::from_float(fractional_position));
            tag_value = pmt::dict_add(tag_value, pmt::string_to_symbol("max_value"),pmt::from_float(max_value));
            tag_value = pmt::dict_update(tag_value,additional_dict);

            this->add_item_tag(0,max_index,this->tag_name,tag_value);

        }

    } /* namespace numeric */
} /* namespace gr */

