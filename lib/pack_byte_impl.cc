/* -*- c++ -*- */
/* 
 * Copyright 2018 xaviertolza.
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
#include "pack_byte_impl.h"

namespace gr {
    namespace numeric {

        pack_byte::sptr
        pack_byte::make(bool msb,std::string tag_name)
        {
            return gnuradio::get_initial_sptr
                    (new pack_byte_impl(msb, tag_name));
        }

        /*
         * The private constructor
         */
        pack_byte_impl::pack_byte_impl(bool msb,std::string tag_name)
                : gr::block("pack_byte",
                            gr::io_signature::make(1, 1, sizeof(uint8_t)),
                            gr::io_signature::make(1, 1, sizeof(uint8_t)))
        {
            d_tag_key = pmt::string_to_symbol(tag_name);
            buffer=0;
            bit_index=0;
            d_msb=msb;
        }

        /*
         * Our virtual destructor.
         */
        pack_byte_impl::~pack_byte_impl()
        {
        }

        void
        pack_byte_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
        {
            /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        }

        int
        pack_byte_impl::general_work (int noutput_items,
                                      gr_vector_int &ninput_items,
                                      gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &output_items)
        {
            const uint8_t *in = (const uint8_t *) input_items[0];
            uint8_t *out = (uint8_t *) output_items[0];

            // Variables declaration
            uint64_t nitemsRead = this->nitems_read(0);
            unsigned n_in = ninput_items[0];
            uint8_t sample;
            unsigned sample_index;
            uint64_t out_index=0;
            uint64_t next_tag=noutput_items;

            // Getting tags
            std::vector<tag_t> tags;
            this->get_tags_in_window(tags,0,0,n_in, d_tag_key);
            std::vector<tag_t>::iterator tag = tags.begin();
            if(tag!=tags.end()){
                next_tag=(*tag).offset - nitemsRead;
            }

            for(sample_index=0;sample_index<n_in;sample_index++){
                sample=in[sample_index]>0;
                if (next_tag == sample_index){
                    bit_index=0;
                    buffer=0;
                    tag++;
                    if(tag!=tags.end()){
                        next_tag=(*tag).offset - nitemsRead;
                    }
                    else{
                        next_tag=noutput_items;
                    }
                }
                if(d_msb){
                    buffer |= sample << (7-bit_index);
                }
                else{
                    buffer |= sample << bit_index;
                }
                bit_index++;
                if(bit_index>=8){
                    bit_index=0;
                    out[out_index] = buffer;
                    out_index++;
                    buffer=0;
                }
            }

            this->consume_each(n_in);
            // Tell runtime system how many output items we produced.
            return out_index;
        }

    } /* namespace numeric */
} /* namespace gr */

