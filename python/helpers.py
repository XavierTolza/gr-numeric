from threading import Thread
from time import sleep


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