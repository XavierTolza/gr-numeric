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
import pmt
from tag_copier import tag_copier
import numpy as np


class qa_tag_copier(gr_unittest.TestCase):
    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        src_data = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]).astype(float)
        trg_data = np.array([-1, 1, 1, 1, -1, -1, 1, 1, -1, -1]).astype(int)

        src = blocks.vector_source_f(src_data)
        trg = blocks.vector_source_s(trg_data)
        op = blocks.burst_tagger(gr.sizeof_float)
        other = blocks.vector_source_f(trg_data)
        blk = tag_copier(gr.sizeof_float, 2, 0)
        snk = blocks.vector_sink_f()

        self.tb.connect(src, (op, 0))
        self.tb.connect(trg, (op, 1))
        self.tb.connect(other, blk)
        self.tb.connect(op, (blk, 1))
        self.tb.connect(blk, snk)
        self.tb.run()

        res = snk.tags()
        data = snk.data()
        assert np.all(np.array(data)==trg_data)
        assert len(res) == 4
        offsets = np.array([i.offset for i in res])
        assert np.all(offsets == np.array([1, 4, 6, 8]))
        values = np.array([pmt.to_python(i.value) for i in res])
        assert np.all(values == np.array([True, False, True, False]))

if __name__ == '__main__':
    gr_unittest.run(qa_tag_copier, "qa_tag_copier.xml")
