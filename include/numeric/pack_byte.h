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


#ifndef INCLUDED_NUMERIC_PACK_BYTE_H
#define INCLUDED_NUMERIC_PACK_BYTE_H

#include <numeric/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace numeric {

    /*!
     * \brief <+description of block+>
     * \ingroup numeric
     *
     */
    class NUMERIC_API pack_byte : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<pack_byte> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of numeric::pack_byte.
       *
       * To avoid accidental use of raw pointers, numeric::pack_byte's
       * constructor is in a private implementation
       * class. numeric::pack_byte::make is the public interface for
       * creating new instances.
       */
      static sptr make(bool msb,std::string tag_name);
    };

  } // namespace numeric
} // namespace gr

#endif /* INCLUDED_NUMERIC_PACK_BYTE_H */

