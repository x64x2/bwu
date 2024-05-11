#!/usr/bin/env python3
import click
import os
import sys
from stor.logging import log
from stor.daemon import daemon
from stor.client import send_action
from stor.common import chunks

@click.group(invoke_without_command=True)
@click.option("--config", default=None, type=click.Path(exists=True))
@click.pass_context
def cli(ctx, config):
    from stor.config import load_config, load_default_config
    if config:
        if not load_config(config):
            sys.stderr.write("Unable to load config '{}'\n".format(config))
            sys.exit(1)
    else:
        load_default_config()
    if ctx.invoked_subcommand is None:
        status([])

@cli.command(help="Runs the stor daemon")
@click.option("--fork", default=False, is_flag=True)
def daemon(fork):
    if fork:
        log.info("Forking to background")
        if os.fork() != 0:
            sys.exit()
    log.info("Running as daemon")
    _daemon()

@cli.command(help="Add a new torrent")
@click.argument('f', type=click.Path(exists=True))
@click.option("--paused", default=False, is_flag=True)
def add(f, paused):
    if not os.path.isabs(f):
        f = os.path.join(os.getcwd(), f)
    payload = {
        "paused": paused,
        "path": f
    }
    output = send_action("add_torrent", payload)
    if output["success"]:
        sys.stdout.write("Added {}\n".format(output["info_hash"]))
    else:
        sys.stderr.write("Error: {}\n".format(output["error"]))

def meta_status(status, torrents):
    print("stor is running on pid {} (running for {} seconds)"\
        .format(status["session"]["pid"], status["session"]["uptime"]))
    print(":: [{} downloading] [{} seeding] [{} idle]"\
        .format(status["downloading"], status["seeding"], status["idle"]))
    print(":: [{} kb/s up] [{} kb/s down] [{} peers] [{:.2f} ratio]"\
        .format(status["session"]["upload_rate"] / 1000,
            status["session"]["download_rate"] / 1000,
            status["session"]["num_peers"],
            status["session"]["ratio"]))
    for torrent in torrents["torrents"]:
        print()
        print("{}".format(torrent["name"]))
        state = torrent["state"]
        state = state[0:1].upper() + state[1:]
        if state == "Downloading":
            print(":: {} since {} ({:.0f}%)".format(state, "TODO", torrent["progress"] * 100))
        print(":: Info hash: {}".format(torrent["info_hash"]))
        total = len(torrent["pieces"])
        sys.stdout.write(":: Progress:\n:: [")
        for pieces in chunks(torrent["pieces"], int(total / 49)):
            if all(pieces):
                sys.stdout.write(":")
            elif any(pieces):
                sys.stdout.write(".")
            else:
                sys.stdout.write(" ")
        sys.stdout.write("]\n")

def torrent_status(torrents):
    for torrent in torrents:
        if torrent != torrents[0]:
            print()
        print("{}".format(torrent["name"]))
        print()
        state = torrent["state"]
        state = state[0:1].upper() + state[1:]
        if state == "Downloading":
            print("{} since {} ({:.0f}%)".format(state, "TODO", torrent["progress"] * 100))
        print("Info hash: {}".format(torrent["info_hash"]))
        total = len(torrent["pieces"])
        sys.stdout.write("Progress:\n[")
        for pieces in chunks(torrent["pieces"], int(total / 49)):
            if all(pieces):
                sys.stdout.write(":")
            elif any(pieces):
                sys.stdout.write(".")
            else:
                sys.stdout.write(" ")
        sys.stdout.write("]\n")

@cli.command(help="Show status information")
@click.argument('what', nargs=-1)
@click.option('--show-all', default=False, is_flag=True)
def status(what, show_all):
    cwd = os.getcwd()
    status = send_action("status")
    torrents = send_action("list_torrents")

    matching = [t for t in torrents["torrents"] \
        if t["info_hash"] in list(what) \
            or t["path"] in [os.path.realpath(p) for p in what] \
            or (t["path"] == cwd and len(what) == 0)]

    if any(matching) and not show_all:
        if len(what) == 0:
            print("Only showing torrents being downloaded to this directory")
            print("Override this behavior with --show-all")
            print()
        if len(matching) > 1:
            meta_status(status, torrents)
        else:
            torrent_status(matching)
    else:
        meta_status(status, torrents)

@cli.command(help="Run interactive console on daemon")
def interact():
    send_action("interact")

if __name__ == '__main__':
    cli()
