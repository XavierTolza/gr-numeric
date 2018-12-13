import numpy
from gnuradio import gr
import pmt
import numpy as np


class pack_byte_python(gr.basic_block):
    """
    docstring for block pack_byte_python
    """

    def __init__(self, msb, string_tag_name):
        gr.basic_block.__init__(self,
                                name="pack_byte_python",
                                in_sig=[numpy.uint8],
                                out_sig=[numpy.uint8])
        self.msb = msb
        self.string_tag_name = string_tag_name
        self.phase = 0
        self.counter = 0

    def forecast(self, noutput_items, ninput_items_required):
        # setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    @staticmethod
    def pop_tag(tags):
        tag = None
        if tags is not None and len(tags):
            tag = tags[0]
        else:
            tags = None
        return tag, tags

    def general_work(self, input_items, output_items):
        tags = self.get_tags_in_window(0, 0, len(input_items[0]),pmt.to_pmt(self.string_tag_name))
        noutputted = 0
        nread = self.nitems_read(0)
        # print(dict(ninputs=len(input_items[0]),noutputs=len(output_items[0]),nread=nread,ntags=len(tags)))
        tag, tags = self.pop_tag(tags)

        for sample_index, sample in enumerate(input_items[0]):
            if tag and (tag.offset - nread) == sample_index:
                self.phase = 0
                self.counter=0
                tag, tags = self.pop_tag(tags)

            if self.msb:
                mul = 2 ** (7 - self.phase)
            else:
                mul = 2 ** self.phase
            # print(dict(sample=sample,sample_index=sample_index,mul=mul,phase=self.phase,noutputted=noutputted,counter=self.counter))
            self.counter += (sample != 0) * mul
            self.phase += 1
            if self.phase == 8:
                self.phase = 0
                output_items[0][noutputted] = self.counter
                noutputted += 1
                self.counter = 0

        self.consume(0, len(input_items[0]))
        # self.consume_each(len(input_items[0]))
        return noutputted
