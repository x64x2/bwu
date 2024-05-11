import asyncio
import os
from stor.protocol import storServer
from stor.config import _cfg
from stor.session import Session

uid = os.getuid()
gid = os.getgid()

if uid == 0:
    # we want to drop root
    # todo: let users customize user/group
    desired_uid = pwd.getpwnam("nobody")[2]
    desired_gid = grp.getgrnam("nobody")[2]
else:
    desired_uid = uid
    desired_gid = gid

def daemon():
    session = Session()
    loop = asyncio.get_event_loop()
    ep = _cfg("listen", "ep")
    usock = None
    stor = storServer()
    stor.session = session
    if ep.startswith("unix://"):
        path = ep[len("unix://"):]
        s = loop.create_unix_server(lambda: stor, path=path)
        usock = path
    elif ep.startswith("tcp://"):
        parts = ep[len("tcp://"):].split(":")
        iface = parts[0]
        if len(parts) > 1:
            port = int(parts[1])
        else:
            port = 50932
        s = loop.create_server(lambda: stor, iface, port)
    server = loop.run_until_complete(s)
    if usock:
        os.chmod(usock, 0o775)
        os.chown(usock, desired_uid, desired_gid)
    if uid == 0:
        os.setgid(desired_gid)
        os.setuid(desired_uid)
        os.umask(0o775)
    try:
        loop.run_until_complete(server.wait_closed())
    except KeyboardInterrupt:
        pass
    finally:
        server.close()
        if usock:
            os.remove(usock)
