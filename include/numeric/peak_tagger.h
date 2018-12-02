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


#ifndef INCLUDED_numeric_PEAK_TAGGER_H
#define INCLUDED_numeric_PEAK_TAGGER_H

#include <numeric/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace numeric {

    /*!
     * \brief <+description of block+>
     * \ingroup numeric
     *
     */
    class NUMERIC_API peak_tagger : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<peak_tagger> sptr;

      /*!
       * \brief Find the maximum of a peak
       *
       * Find the maximum of a peak by cubic interpolation and put a tag on the closest sample with the epoch. The
       * signal on which you want to search for the peak must be on input 1 and the signal to tag on input 0. Input
       * 0 will then be forwarded to ouput0. A peak is detected when input 1 is > to the trigger value
       */
      static sptr make(std::string tag_name="peak",bool forward_tags=false,
              pmt::pmt_t additional_dict = pmt::make_dict());
    };

  } // namespace numeric
} // namespace gr

#endif /* INCLUDED_numeric_PEAK_TAGGER_H */

