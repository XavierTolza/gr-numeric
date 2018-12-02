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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "synchronized_decimator_impl.h"

namespace gr {
  namespace numeric {

      synchronized_decimator::sptr
      synchronized_decimator::make(unsigned decimation_ratio, char* tag_name,
                                   uint8_t item_size, bool propagate_tags, bool tag_value_must_be_true)
      {
          return gnuradio::get_initial_sptr
                  (new synchronized_decimator_impl(decimation_ratio, tag_name, item_size, propagate_tags,
                                                   tag_value_must_be_true));
      }

      /*
       * The private constructor
       */
      synchronized_decimator_impl::synchronized_decimator_impl(unsigned decimation_ratio, char* tag_name,
                                                               uint8_t item_size, bool propagate_tags,
                                                               bool tag_value_must_be_true)
              : gr::block("synchronized_decimator",
                          gr::io_signature::make(1, 1, sizeof(ITYPE)*item_size),
                          gr::io_signature::make(1, 1, sizeof(OTYPE)*item_size))
      {
          d_decim_ratio = decimation_ratio;
          d_tag_name = tag_name;
          d_selected_phase=0;
          d_item_size=item_size;
          d_match_if_tag_true = tag_value_must_be_true;
          d_propagate_tags = propagate_tags;

          // Manual tag propagation
          set_tag_propagation_policy(TPP_DONT);
      }

      /*
       * Our virtual destructor.
       */
      synchronized_decimator_impl::~synchronized_decimator_impl()
      {
      }

      void
      synchronized_decimator_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
      {
          ninput_items_required[0] = noutput_items*d_decim_ratio;
      }

      int
      synchronized_decimator_impl::general_work(int noutput_items, gr_vector_int &ninput_items,
                                                gr_vector_const_void_star &input_items,
                                                gr_vector_void_star &output_items)
      {
          const ITYPE *in = (const ITYPE *) input_items[DEFAULT_INPUT];
          OTYPE *out = (OTYPE *) output_items[DEFAULT_INPUT];

          // Variables declaration
          unsigned sample_index,n_item_read, sample_index_abs, n_outputted;
          unsigned n_in;
          std::vector<tag_t> tags;
          std::vector<tag_t>::iterator tag_iterator;
          bool tags_in_packet;
          unsigned phase;
          std::string tag_key;
          bool tag_value;

          //Variables initialization
          n_in = ninput_items[DEFAULT_INPUT];
          n_item_read = nitems_read(DEFAULT_INPUT);
          n_outputted=0;

          // Gettings tags
          this->get_tags_in_range(tags,DEFAULT_INPUT,n_item_read,n_item_read+n_in,pmt::string_to_symbol(d_tag_name));
          tag_iterator = tags.begin();
          tags_in_packet = !tags.empty();

          for(sample_index=0;sample_index<n_in & n_outputted< noutput_items;sample_index++){
              sample_index_abs = n_item_read+sample_index;
              phase = sample_index_abs % d_decim_ratio;

              if(tags_in_packet){
                  while((*tag_iterator).offset<sample_index_abs & tag_iterator!=tags.end()){
                      tag_iterator++;
                  }
                  if((*tag_iterator).offset==sample_index_abs && tag_iterator!=tags.end()) {
                      tag_key=pmt::symbol_to_string((*tag_iterator).key);
                      // Is the tag is a bool, check if is it true
                      if(pmt::is_bool(tag_iterator->value)){
                          tag_value = pmt::to_bool(tag_iterator->value);
                      }
                      else{
                          tag_value = false;
                      }
                      if(tag_value | !d_match_if_tag_true){
                          // The correct tag is detected at this sample
                          d_selected_phase = phase;
                      }

                      if(d_propagate_tags){
                          this->add_item_tag(0,n_outputted+this->nitems_written(0),
                                             tag_iterator->key,
                                             tag_iterator->value);
                      }
                  }
              }

              if(d_selected_phase==phase){
                  memcpy(out+n_outputted*d_item_size,in+sample_index*d_item_size,d_item_size);
                  n_outputted++;
              }
          }

          consume_each(sample_index);

          // Tell runtime system how many output items we produced.
          return n_outputted;
      }

  } /* namespace numeric */
} /* namespace gr */

