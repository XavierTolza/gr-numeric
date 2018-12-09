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

from gnuradio import gr,blocks

class tag_copier(gr.hier_block2):
    """
    docstring for block tag_copier
    """
    def __init__(self, item_size, n_streams=2, output_stream=False):
        if output_stream is False:
            # toutes les sorties
            only_one_output = False
            n_streams_out = n_streams
        else:
            only_one_output=True
            n_streams_out = 1

        gr.hier_block2.__init__(self,
                                "tag_copier",
                                gr.io_signaturev(n_streams, n_streams, [item_size]*n_streams),  # Input signature
                                gr.io_signaturev(n_streams_out, n_streams_out, [item_size]*n_streams_out)) # Output signature

        blk = blocks.delay(item_size, 0)

        for i in range(n_streams):
            self.connect((self, i),(blk,i))
            if only_one_output:
                if i != output_stream:
                    snk = blocks.null_sink(item_size)
                    self.connect((blk,i), snk)
                else:
                    self.connect((blk,i), (self, 0))
            else:
                self.connect((blk,i), (self, i))
