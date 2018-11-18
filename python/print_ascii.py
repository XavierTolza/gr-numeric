#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 xtolza.
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
import numpy
from gnuradio import gr


class print_ascii(gr.sync_block):
    """
    docstring for block print_ascii
    """

    def __init__(self, encode_hex=False):
        gr.sync_block.__init__(self,
                               name="print_ascii",
                               in_sig=[numpy.uint8],
                               out_sig=None)

        self.encode_hex = encode_hex

    def work(self, input_items, output_items):
        in0 = input_items[0]
        data = struct.pack("B" * len(in0), *in0)
        if self.encode_hex:
            data = data.encode("hex")
        print data
        return len(input_items[0])
