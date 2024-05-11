import asyncio
import json
from stor.logging import log
from stor.actions import actions

def execute(session, payload):
    action = payload.get("action")
    if not action:
        return "'action' is requried\n"
    _action = actions.get(action)
    if not _action:
        return "Unknown action '{}'\n".format(action)
    return _action(session, payload)

# TODO: Authentication
class storServer(asyncio.Protocol):
    buf = ""

    def connection_made(self, transport):
        self.transport = transport

    def data_received(self, data):
        self.buf += data.decode("utf-8")
        while '\n' in self.buf:
            j = self.buf[:self.buf.index('\n')]
            self.buf = self.buf[self.buf.index('\n') + 1:]
            try:
                payload = json.loads(j)
            except ValueError:
                self.send("Error: invalid JSON\n")
                self.transport.close()
                return
            self.execute(payload)

    def execute(self, payload):
        self.send(json.dumps(execute(self.session, payload)) + "\n")

    def send(self, s):
        if s:
            self.transport.write(s.encode("utf-8"))
            self.transport.write(b"\x00")

class storClient(asyncio.Protocol):
    buf = ""

    def __init__(self, callback, future = None):
        self.callback = callback
        self.future = future

    def connection_made(self, transport):
        self.transport = transport

    def data_received(self, data):
        self.buf += data.decode("utf-8")
        while '\x00' in self.buf:
            text = self.buf[:self.buf.index('\x00')]
            self.buf = self.buf[self.buf.index('\x00') + 1:]
            if self.future:
                self.future.set_result(text)
            callback(self.transport, text)

    def send(self, s):
        if s:
            self.transport.write(s.encode("utf-8"))

class storOneOffClient(asyncio.Protocol):
    buf = ""

    def __init__(self, command, future):
        self.command = command
        self.future = future

    def connection_made(self, transport):
        self.transport = transport
        self.send(self.command)

    def data_received(self, data):
        self.buf += data.decode("utf-8")
        while '\x00' in self.buf:
            text = self.buf[:self.buf.index('\x00')]
            self.buf = self.buf[self.buf.index('\x00') + 1:]
            if self.future:
                self.future.set_result(text)
            self.transport.close()

    def send(self, s):
        if s:
            self.transport.write(s.encode("utf-8"))
