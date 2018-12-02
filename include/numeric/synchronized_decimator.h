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


#ifndef INCLUDED_NUMERIC_SYNCHRONIZED_DECIMATOR_H
#define INCLUDED_NUMERIC_SYNCHRONIZED_DECIMATOR_H

#include <numeric/api.h>
#include <gnuradio/block.h>

namespace gr {
    namespace numeric {

/*!
         * \brief <+description of block+>
         * \ingroup FFLY
         *
         */
        class NUMERIC_API synchronized_decimator

        : virtual
        public gr::block {
        public:
        typedef boost::shared_ptr<synchronized_decimator> sptr;

        /*!
         * \brief Return a shared_ptr to a new instance of FFLY::synchronized_decimator.
         *
         * To avoid accidental use of raw pointers, FFLY::synchronized_decimator's
         * constructor is in a private implementation
         * class. FFLY::synchronized_decimator::make is the public interface for
         * creating new instances.
         */
        static sptr make(unsigned char decimation_ratio, char *tag_name,
                         uint8_t item_size, bool propagate_tags = false, bool tag_value_must_be_true = false);
    };

} // namespace numeric
} // namespace gr

#endif /* INCLUDED_NUMERIC_SYNCHRONIZED_DECIMATOR_H */

