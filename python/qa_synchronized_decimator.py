#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 gr-numeric author.
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

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import numeric.numeric_swig as numeric
import numpy as np


class qa_synchronized_decimator(gr_unittest.TestCase):
    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def generic(self, tag_must_be_true):
        src_data = np.array([ 1,  2,  3,  4,  5,  6,  7,  8,  9,  10]).astype(float)
        trg_data = np.array([-1,  1,  1,  1, -1, -1,  1,  1, -1,  -1]).astype(int)

        src = blocks.vector_source_f(src_data)
        trg = blocks.vector_source_s(trg_data)
        op = blocks.burst_tagger(gr.sizeof_float)
        blk = numeric.synchronized_decimator(2,"burst", gr.sizeof_float, True, tag_must_be_true)
        snk = blocks.vector_sink_f()

        self.tb.connect(src, (op,0))
        self.tb.connect(trg, (op,1))
        self.tb.connect(op, blk)
        self.tb.connect(blk, snk)
        self.tb.run()

        result = np.array(snk.data())
        return result

    def test_001_t (self):
        result = self.generic(True)
        expected_result = [1, 2, 4, 6, 7, 9]
        assert (result == expected_result).all()

    def test_002_t (self):
        result = self.generic(False)
        expected_result = [1, 2, 4, 5, 7, 9]
        assert (result == expected_result).all()


if __name__ == '__main__':
    gr_unittest.run(qa_synchronized_decimator, "qa_synchronized_decimator.xml")
