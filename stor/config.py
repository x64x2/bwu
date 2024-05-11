import os
import sys
from configparser import ConfigParser

config = ["/etc/stor.conf"]

config = ConfigParser()

def load_config(path):
    if os.path.exists(path):
        config.readfp(open(path))
        return True
    else:
        return False

def load_default_config():
    for p in config_paths:
        if load_config(p):
            return
    sys.stderr.write("Unable to open config file. Use --config=path/to/config or stor genconfig\n")
    sys.exit(1)

_cfg = lambda e, k: config.get(e, k)
