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
import struct

from gnuradio import gr, gr_unittest
from gnuradio import blocks
from pack_byte_python import pack_byte_python as pack_byte
from lib import start_block
import pmt


class qa_pack_byte_python(gr_unittest.TestCase):
    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def tag(self, offset, value, name):
        res = gr.tag_t()
        res.offset = offset
        res.key = pmt.to_pmt(name)
        res.value = pmt.to_pmt(value)
        return res

    def test_001_t(self):
        tags = [
            self.tag(2, 0, "tag")
        ]
        src = blocks.vector_source_b([0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                                     tags=tags, repeat=False)
        blk = pack_byte(True, "tag")
        snk = blocks.vector_sink_b()

        self.tb.connect(src, blk, snk)
        start_block(self.tb, 0.01)

        res = snk.data()
        tags = snk.tags()
        print "Ntags: %i :" % len(tags)
        for tag in tags:
            print "   - %s" % tag.key
        res = struct.pack("B"*len(res),*res).encode("hex")
        print res
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_pack_byte_python, "qa_pack_byte_python.xml")
