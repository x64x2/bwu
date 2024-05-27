import asyncio
import json
from stor.config import _cfg
from stor.protocol import STOROneOffClient

class StorClient:
    """
    Client class for entire project.

    Contains the game loop, and contains
    the event_loop which passes events to States as needed.

    Parameters:
        config: The config for cli.

    """

    def __init__(self, index : int,blocks : list, config: StorConfig) -> None:
        self.config = config
        self.index : int = index
        self.blocks: list = blocks
        self.downloaded_blocks = BitArray(bin='0'*len(blocks))
    
    def flush(self):
        [block.flush() for block in self.blocks]
    
    def is_complete(self):
        '''Returns true if all the blocks have been downloaded'''
        return all(self.downloaded_blocks)
    
    def save_block(self, begin : int, data : bytes):
        '''Writes data in block object'''
        for block_idx, block in enumerate(self.blocks):
            if block.begin == begin:
                block.data = data
                self.downloaded_blocks[block_idx] = True
    
    def flush(self):
            [block.flush() for block in self.blocks]
    
    def is_complete(self):
        '''Returns true if all the blocks have been downloaded'''
        return all(self.downloaded_blocks)
    
    def save_block(self, begin : int, data : bytes):
        '''Writes data in block object'''
        for block_idx, block in enumerate(self.blocks):
            if block.begin == begin:
                block.data = data
                self.downloaded_blocks[block_idx] = True
                  
@asyncio.coroutine
def wait(c, future):
    asyncio(c)
    result = yield from future
    return result



def send_command(cmd):
    if "\n" not in cmd:
        cmd += "\n"
    loop = asyncio.get_event_loop()
    ep = _cfg("listen", "ep")
    future = asyncio.Future()
    client = storOneOffClient(cmd, future=future)
    if ep.startswith("unix://"):
        path = ep[len("unix://"):]
        c = loop.create_unix_connection(lambda: client, path=path)
    elif ep.startswith("tcp://"):
        parts = ep[len("tcp://"):].split(":")
        iface = parts[0]
        if len(parts) > 1:
            port = int(parts[1])
        else:
            port = 50932
        c = loop.create_connection(lambda: client, iface, port)
    return loop.run_until_complete(wait(c, future))

def send_action(action, payload={}):
    payload["action"] = action
    return json.loads(send_command(json.dumps(payload)))
