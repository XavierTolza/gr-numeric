#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#
import os
import struct
from time import sleep

try:
    import numeric_swig as numeric
except:
    # import numeric
    print("Warning, using numeric instead of numeric_swig")
import matplotlib.pyplot as plt
import numpy as np
import pmt
from gnuradio import blocks
from gnuradio import gr, gr_unittest

from lib import const_source, start_block, tags_to_python

try:
    plot = os.environ["DEBUG"] == "1"
except:
    plot = False

class qa_peak_tagger (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_simple_cosine (self):
        t_max = 100
        samp_rates = (0.051001,0.001)
        freq = 8 * np.pi
        trigger_level = .5

        t_cont = np.arange(0,t_max,samp_rates[1])
        t = np.arange(0,t_max,samp_rates[0])
        y = np.sin(freq * t)
        y_cont = np.sin(freq * t_cont)
        TS = np.mean(np.diff(t))

        # plt.figure()
        # plt.plot(t_cont,y_cont)
        # plt.plot(t,y)
        # plt.show()

        # Blocks
        src = blocks.vector_source_f(y)
        src_trig = blocks.vector_source_f(np.ones(y.shape) * trigger_level)
        input_size, vect_length, tag_name, forward_tags = gr.sizeof_float,1,"peak",False
        blk = numeric.peak_tagger(tag_name, forward_tags)
        snk = blocks.vector_sink_f()

        # Connexions
        self.tb.connect(src,(blk,0),snk)
        self.tb.connect(src_trig,(blk,1))

        # Go!
        start_block(self.tb,0.1)

        # check data
        result = snk.data()
        tags = {tag.offset: pmt.to_python(tag.value) for tag in snk.tags() if pmt.symbol_to_string(tag.key)=="peak"}
        assert np.isclose(result, y).all()
        n_tags_theor = np.sum(np.diff((y > trigger_level).astype(int)) > 0)-1
        res_ok = len(tags) == n_tags_theor

        # Expected maxes
        maxes_time = self.get_maxes(y_cont,trigger_level)[1:]
        maxes_ampl = y_cont[maxes_time]
        maxes_time = t_cont[maxes_time]

        # Found maxes
        offsets_int, offset_rel, offsets_ampl = np.array([[key, value["rel_index"], value["max_value"]]
                                                          for key, value in tags.iteritems()]).transpose()
        offsets_int = offsets_int.astype(int)
        offsets_time_int = t[offsets_int]
        offsets_time = offsets_time_int + offset_rel * TS

        offset_error = np.abs(offsets_time - maxes_time) / TS
        ampl_error = np.abs(offsets_ampl - maxes_ampl)
        # Error must be < 10%
        res_ok &= np.all(offset_error<(10/100.))
        res_ok &= np.all(ampl_error<(10/100.))

        if not res_ok and plot:
            # Plot result for visual verification
            fig, axes = plt.subplots(2,1)
            axe = axes[0]
            axe.set_title("Peak finding block example")
            axe.plot(t_cont,y_cont,label="Continuous signal",alpha=.3)
            axe.plot(t,y,label="Sampled input signal", marker='.')
            axe.scatter(maxes_time, maxes_ampl,label="Theor maximums",color="g",marker='^')
            axe.scatter(offsets_time_int, offsets_ampl,label="Found maximums",color="r")
            axe.scatter(offsets_time, offsets_ampl, label="Found maximums interp",
                        color="darkorange", marker="*")
            xlim = axe.get_xlim()
            axe.plot(xlim,[trigger_level]*2,ls=":",color='k',label='trigger lvl')
            axe.set_xlabel("Input samples")
            axe.set_ylabel("Signal amplitude")

            axe = axes[1]

            axe.plot(offset_error, label="Offset error")
            axe.plot(ampl_error, label="amplitude error")

            for axe in axes:
                axe.grid()
                axe.legend(loc=4)

            plt.show()
        assert res_ok

    def get_maxes(self, data, trig_level):
        if len(data)==0:
            raise ValueError("Get maxes of empty data")
        is_above = (np.array(data) > np.array(trig_level)).astype(int)
        is_invalid = np.logical_or(np.isinf(data),np.isnan(data))
        is_above = np.logical_and(is_above,np.logical_not(is_invalid)).astype(int)
        trig_on_off = np.diff(is_above)
        starts = np.where(trig_on_off > 0)[0]
        stops = np.where(trig_on_off < 0)[0]
        try:
            argmax = [np.argmax(d)+start for start, d in [(start, data[start:stop]) for start, stop in zip(starts,stops)]]
            assert len(argmax)>0
        except Exception as e:
            plt.plot(data,label="data")
            plt.plot([0,len(data)],[trig_level]*2,label="trigger")
            plt.plot(is_above,label="is_above")
            plt.legend()
            plt.show()
            raise e
        return np.array(argmax)

    def test_additional_dict(self):
        data = [0, 1, 2, 3, 2, 1, 0]
        src = blocks.vector_source_f(data)
        t_src = blocks.vector_source_f([1.5]*len(data))
        blk = numeric.peak_tagger(tag_name="peak", forward_tags=False, additional_dict=pmt.to_pmt({"key":"value"}))
        snk = blocks.vector_sink_f()

        self.tb.connect(src,blk,snk)
        self.tb.connect(t_src,(blk,1))
        start_block(self.tb,0.1)

        out_data,tags = snk.data(),snk.tags()

        assert len(out_data)>0
        assert np.all(np.array(out_data)== np.array(data))
        assert len(tags) == 1
        tag=tags[0]
        assert tag.offset==3
        value = pmt.to_python(tag.value)
        for key in "rel_index,key,max_value".split(","):
            assert key in value
        pass

    def test_time_tracker(self):
        data = [0, 1, 2, 3, 2, 1, 0]
        from lib import tag
        tags_src = [tag(o,k,v) for o,k,v in zip([0,0],"rx_rate,rx_time".split(","),[2,(42,0.1)])]
        src = blocks.vector_source_f(data,tags=tags_src)
        t_src = blocks.vector_source_f([1.5]*len(data))
        blk = numeric.peak_tagger(tag_name="peak", forward_tags=True)
        snk = blocks.vector_sink_f()

        self.tb.connect(src,blk,snk)
        self.tb.connect(t_src,(blk,1))
        start_block(self.tb,0.1)

        out_data,tags = snk.data(),snk.tags()
        tags = tags_to_python(tags)
        assert len(tags)==3
        tag = tags[-1]
        d = tag[2]
        key = "sample_time"
        assert key in d
        assert d[key][0] == 43
        assert np.isclose(d[key][1],0.6)
        pass

if __name__ == '__main__':
    gr_unittest.run(qa_peak_tagger, "qa_peak_tagger.xml")
