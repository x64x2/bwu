

# stor

stor is a bittorrent client daemon. It's designed to handle your torrents in the background, and you interface with it via the
`stor` command. It should be easily possible to make fancier frontends, such as a curses or web interface

stor is a work in progress, and is not currently recommended for everyday use.

## Usage

First, run `stor genconfig` as root. It will create a config file in
`/etc/stor.conf` for you to edit as you please.

Run this command to start the daemon:

    stor daemon

It is suggested that you run this through your init system. You can also run it
through your xinitrc or similar:

    stor daemon --fork

### Add torrents

    $ stor add example.torrent
    Added 4139bde549fb8a6c41122088e731009ca5eca883

This gives you back an info hash. These are used as what is effectively your
torrent's ID. You can use it again later to query your torrent for status or
modify information about it.

### Status Information

Running `stor` will tell you the current status of the daemon. If you navigate
to a directory a torrent is downloading in, and run `stor` again, you'll receive
information about that torrent in particular. You can get the status of a
particular torrent without changing your working directory, too - just run `stor
[info hash]` or `stor [download path]`.


