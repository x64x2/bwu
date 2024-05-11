import asyncio
import json
from stor.config import _cfg
from stor.protocol import storOneOffClient

async def connect(self,
                    first: bool=None,
                    uploaded: int=0,
                    downloaded: int=0):
    params = { ...}
    url = self.torrent.announce + '?' + urlencode(params)

    async with self.http_client.get(url) as response:
        if not response.status == 200:
            raise ConnectionError('Unable to connect to tracker')
        data = await response.read()
        return TrackerResponse(bencoding.Decoder(data).decode())
    
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
