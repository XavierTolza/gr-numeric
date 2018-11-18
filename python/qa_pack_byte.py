import struct

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import numeric.numeric_swig as numeric
import pmt

from helpers import start_block


class qa_pack_byte(gr_unittest.TestCase):
    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def tag(self, offset, value, name):
        res = gr.tag_t()
        res.offset = offset
        res.key = pmt.to_pmt("yolo")
        res.value = pmt.to_pmt(value)
        return res

    def test_001_t(self):
        tags = [
            self.tag(2, 0, "tag")
        ]
        src = blocks.vector_source_b([0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                                     tags=tags, repeat=False)
        blk = numeric.pack_byte(True, "tag")
        snk = blocks.vector_sink_b()

        self.tb.connect(src, blk, snk)
        start_block(self.tb, 0.01)

        res = snk.data()
        res = struct.pack("B"*len(res),*res).encode("hex")
        print res
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_pack_byte, "qa_pack_byte.xml")
