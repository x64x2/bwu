import os
from datetime import datetime
from functools import wraps

actions = dict()

def action(f):
    @wraps(f)
    def wrapper(*args, **kwargs):
        return f(*args, **kwargs)
    actions[f.__name__] = f
    return wrapper

@action
def add_torrent(session, payload):
    path = payload.get("path")
    if not path:
        return { "success": False, "error": "'path' is required" }
    if not os.path.exists(path):
        return { "success": False, "error": "File not found" }
    t = session.add_torrent(path)
    return { "success": True, "info_hash": t.info_hash }

@action
def status(session, payload):
    status = session.status()
    tstatus = [v.status() for k, v in session.torrents.items()]
    response = {
        "downloading": len([s for s in tstatus if str(s.state) == "downloading"]),
        "seeding": len([s for s in tstatus if str(s.state) == "seeding"]),
        "idle": len([s for s in tstatus if str(s.state) == "idle"]),
        "session": {
            "total_download": status.total_download,
            "total_upload": status.total_upload,
            "ratio": status.total_upload / status.total_download \
                if status.total_upload != 0 else 0,
            "num_peers": status.num_peers,
            "download_rate": status.download_rate,
            "upload_rate": status.upload_rate,
            "uptime": (datetime.now() - session.started).seconds,
            "pid": os.getpid()
        }
    }
    return response

@action
def list_torrents(session, payload):
    return {
        "torrents": [v.json() for k, v in session.torrents.items()]
    }

# TODO: Remove this before releasing stor
@action
def interact(session, payload):
    import code
    code.interact(local=locals())
