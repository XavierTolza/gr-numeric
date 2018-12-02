/* -*- c++ -*- */
/* 
 * Copyright 2018 gr-numeric author.
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

#ifndef INCLUDED_NUMERIC_SYNCHRONIZED_DECIMATOR_IMPL_H
#define INCLUDED_NUMERIC_SYNCHRONIZED_DECIMATOR_IMPL_H
#define ITYPE uint8_t
#define OTYPE ITYPE
#define DEFAULT_INPUT 0

#include <numeric/synchronized_decimator.h>

namespace gr {
    namespace numeric {

        class synchronized_decimator_impl : public synchronized_decimator {
        private:
            unsigned d_decim_ratio;
            char *d_tag_name;
            unsigned d_selected_phase;
            uint8_t d_item_size;
            bool d_match_if_tag_true;
            bool d_propagate_tags;

        public:
            synchronized_decimator_impl(unsigned decimation_ratio, char *tag_name,
                                        uint8_t item_size, bool propagate_tags, bool tag_value_must_be_true);

            ~synchronized_decimator_impl();

            void forecast(int noutput_items, gr_vector_int &ninput_items_required);

            // Where all the action really happens
            int general_work(int noutput_items, gr_vector_int &ninput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items);
        };

    } // namespace numeric
} // namespace gr

#endif /* INCLUDED_NUMERIC_SYNCHRONIZED_DECIMATOR_IMPL_H */

