import libtorrent as lt
import binascii
import os
from datetime import datetime
from stor.config import cfg
from stor.logging import log

ses = lt.session({'listen_interfaces': '0.0.0.0:6881'})

class Torrent():
    def __init__(self, info, torrent):
        self.torrent = torrent
        self.info_hash = binascii.b2a_hex(info.info_hash().to_bytes()).decode("utf-8")
        self.name = torrent.name()
        self.info = torrent.torrent_file()
        self.save_path = torrent.save_path()

    def pause(self):
        self.torrent.pause()

    def resume(self):
        self.torrent.resume()

    def status(self):
        return self.torrent.status()

    def json(self):
        s = self.status()
        return {
            "name": self.name,
            "info_hash": self.info_hash,
            "progress": s.progress,
            "pieces": s.pieces,
            #"added_time": s.added_time,
            #"finished_time": s.finished_time,
            "state": str(s.state),
            "upload_rate": s.upload_rate,
            "download_rate": s.download_rate,
            "active_connections": s.num_connections,
            "num_peers": s.num_peers,
            "priority": s.priority,
            "current_tracker": s.current_tracker,
            "trackers": self.torrent.trackers(),
            "path": os.path.realpath(self.torrent.save_path())
        }

class Session():
    def __init__(self):
        port = int(_cfg("listen", "bt"))
        self.session = lt.session()
        self.session.listen_on(port, port + 10)
        # TODO: Configure more session settings from config
        self.session.add_extension(lt.create_ut_pex_plugin)
        self.session.add_extension(lt.create_ut_metadata_plugin)
        self.session.add_extension(lt.create_metadata_plugin)
        self.session.set_severity_level(lt.alert.severity_levels.info)

        self.started = datetime.now()
        self.torrents = dict()

    def status(self):
        return self.session.status()

    def add_torrent(self, path):
        e = lt.bdecode(open(path, 'rb').read())
        info = lt.torrent_info(e)
        params = {
            "save_path": _cfg("torrents", "destination"),
            "storage_mode": lt.storage_mode_t.storage_mode_sparse,
            "ti": info
        }
        torrent = Torrent(info, self.session.add_torrent(params))
        self.torrents[torrent.info_hash] = torrent
        log.info("Added torrent {} - {}".format(torrent.info_hash, torrent.name))
        return torrent

session = None
