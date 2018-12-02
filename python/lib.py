import multiprocessing
from datetime import datetime

import numpy as np
import pmt
from gnuradio import filter, analog, gr
from threading import Thread
from time import sleep


def gmsk_taps(sps,bt):
    ntaps = sps*4
    gauss_filter = filter.firdes.gaussian(1,sps, bt, ntaps)
    rect_w = [1]*sps
    return np.convolve(rect_w,gauss_filter)

def const_source(item_size, value):
    res=None
    if item_size == gr.sizeof_float:
        res = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, value)
    elif item_size == gr.sizeof_gr_complex:
        res = analog.sig_source_c(0, analog.GR_CONST_WAVE, 0, 0, value)
    return res


class vector_sink_message(gr.sync_block):
    def __init__(self):
        self.data = []
        gr.sync_block.__init__(self,
                               name="vector_sink_message",
                               in_sig=None,
                               out_sig=None)

        port_name = "in"
        port_name_pmt = pmt.intern(port_name)
        self.message_port_register_in(port_name_pmt)
        self.set_msg_handler(port_name_pmt, self.msg_handler)

    def msg_handler(self, msg):
        self.data.append(pmt.to_python(msg))

def tag(offset,key,value):
    res = gr.tag_t()
    res.offset = offset
    res.key = pmt.to_pmt(key)
    res.value = pmt.to_pmt(value)
    return res

def tags_to_python(tags):
    return [(tag.offset,pmt.to_python(tag.key),pmt.to_python(tag.value)) for tag in tags]

def start_block(block, timeout):

    class BlockStopper(Thread):
        def __init__(self, block, timeout):
            super(BlockStopper, self).__init__()
            self.timeout = timeout
            self.block = block

        def run(self):
            sleep(self.timeout)
            self.block.stop()

    t = BlockStopper(block,timeout)
    t.start()
    block.run()
    t.join()

def is_power_of_two(numbr):
    return numbr & (numbr - 1) == False

def log_speedtest(name,time,nitems):
    with open("/tmp/gr_speedtest.txt", "a+") as file:
        now = unicode(datetime.now()).split(".")[0]
        using_sec = time >= 0.1
        unit = "sec" if using_sec else "ms"
        if not using_sec:
            time *= 1000
        file.write("{:20}:  {:30}: duration={:3.3f}{:3} ; nitems={:6d} ; throughput={:4.3f} items/sec\n"
                   .format(now,name,time,unit,nitems,nitems/float(time)))

n_threads = multiprocessing.cpu_count()