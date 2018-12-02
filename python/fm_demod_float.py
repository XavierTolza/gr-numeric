# -*- coding: utf-8 -*-
#
# SPDX-License-Identifier: GPL-3.0
#
##################################################
# GNU Radio Python Flow Graph
# Title: Fm Demod Float
# Generated: Sun Dec  2 17:11:30 2018
# GNU Radio version: 3.7.12.0
##################################################

from gnuradio import analog
from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes
import math


class FM_demod_float(gr.hier_block2):

    def __init__(self, delta_freq=500, freq=5000, samp_rate=32000):
        gr.hier_block2.__init__(
            self, "Fm Demod Float",
            gr.io_signature(1, 1, gr.sizeof_float*1),
            gr.io_signature(1, 1, gr.sizeof_float*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.delta_freq = delta_freq
        self.freq = freq
        self.samp_rate = samp_rate

        ##################################################
        # Variables
        ##################################################

        self.taps = taps = firdes.low_pass(1.0, samp_rate, delta_freq, delta_freq, firdes.WIN_HAMMING, 6.76)


        ##################################################
        # Blocks
        ##################################################
        self.freq_xlating_fir_filter_xxx_0_0 = filter.freq_xlating_fir_filter_fcf(1, ([1]), freq, samp_rate)
        self.fft_filter_xxx_0 = filter.fft_filter_ccf(1, (taps), 1)
        self.fft_filter_xxx_0.declare_sample_delay(0)
        self.analog_quadrature_demod_cf_0 = analog.quadrature_demod_cf(samp_rate/(2*math.pi*delta_freq))



        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_quadrature_demod_cf_0, 0), (self, 0))
        self.connect((self.fft_filter_xxx_0, 0), (self.analog_quadrature_demod_cf_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0_0, 0), (self.fft_filter_xxx_0, 0))
        self.connect((self, 0), (self.freq_xlating_fir_filter_xxx_0_0, 0))

    def get_delta_freq(self):
        return self.delta_freq

    def set_delta_freq(self, delta_freq):
        self.delta_freq = delta_freq
        self.analog_quadrature_demod_cf_0.set_gain(self.samp_rate/(2*math.pi*self.delta_freq))

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.freq_xlating_fir_filter_xxx_0_0.set_center_freq(self.freq)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_quadrature_demod_cf_0.set_gain(self.samp_rate/(2*math.pi*self.delta_freq))

    def get_taps(self):
        return self.taps

    def set_taps(self, taps):
        self.taps = taps
        self.fft_filter_xxx_0.set_taps((self.taps))