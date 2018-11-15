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
                            gr::io_signature::make(1, 1, sizeof(gr.sizeof_char)),
                            gr::io_signature::make(1, 1, sizeof(gr.sizeof_char)))
        {
            d_tag_key = tag_name;
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

            // Variables declaration
            unsigned n_bytes_available, n_bits_available, tag_offset_rel,n_bytes_left, n_bits_to_copy, n_bits_left;
            uint64_t nitemsRead = this->nitems_read(0);

            // Getting tags
            std::vector<tag_t> tags;
            this->get_tags_in_range(tags,0,nitemsRead,nitemsRead+noutput_items, d_tag_key);


            // Tell runtime system how many output items we produced.
            return noutput_items;
        }

    } /* namespace numeric */
} /* namespace gr */

