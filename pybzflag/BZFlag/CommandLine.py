""" BZFlag.CommandLine

Utilities for processing command line options for different categories
of BZFlag-related utilities.
"""
#
# Python BZFlag Protocol Package
# Copyright (C) 2003 Micah Dowty <micahjd@users.sourceforge.net>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

from BZFlag import optik, Client, Player, Server, Protocol
import sys


# Override Optik's default options (for consistent grammar and capitalization)
optik.STD_HELP_OPTION.help    = "Shows this help message and exits."
optik.STD_VERSION_OPTION.help = "Shows the version number and exits."


class Parser(optik.OptionParser):
    """OptionParser subclass to assist with creating Client or Server instances
       using a standard set of command line options.
       """
    def __init__(self, cls, **extraDefaults):
        optik.OptionParser.__init__(self)
        self.inst = cls()
        availableOpts = self.inst.options.keys()

        defaults = {
            'team':       'rogue',
            'email':      'PyBZFlag',
            'playerType': 'tank',
            'interface':  ':%s' % Protocol.Common.defaultPort,
            }
        defaults.update(extraDefaults)

        def add(*names, **kw):
            """Wrapper around self.add_option that enforces our conventions about defaults"""
            default = defaults.get(kw['dest'])
            kw['default'] = default
            if default is not None:
                kw['help'] += " [%s]" % default
            self.add_option(*names, **kw)

        if 'server' in availableOpts:
            add("-s", "--server", dest="server", metavar="HOST",
                help="Sets the BZFlag server to connect to on startup.")

        if 'interface' in availableOpts:
            add("-i", "--interface", dest="interface", metavar="HOST:PORT",
                help="Sets the host and/or the port to listen on.")

        if 'identity' in availableOpts:
            add("-c", "--call-sign", dest="callSign", metavar="NAME",
                help="Sets the player callsign to use when joining.")
            add("-t", "--team", dest="team", metavar="COLOR",
                help="Sets the team to join.")
            add("-e", "--email", dest="email", metavar="ADDRESS",
                help="Sets the optional email address to send when joining.")
            add("-p", "--player-type", dest="playerType", metavar="TYPE",
                help="Sets the player type to join the game as. This can be 'tank', 'observer', or 'computer'.")
    def parse(self, argv=sys.argv):
        (values, args)  = self.parse_args(argv[1:])
        values = values.__dict__
        options = {}

        try:
            options['server'] = values['server']
        except KeyError:
            pass

        try:
            options['identity'] = Player.Identity(
                values['callSign'], values['team'], values['email'], values['playerType'])
        except KeyError:
            pass

        try:
            options['interface'] = values['interface']
        except KeyError:
            pass

        self.inst.cmdLineValues = values
        self.inst.cmdLineArgs = args
        self.inst.setOptions(**options)
        return self.inst


def client(clientClass=Client.PlayerClient, argv=sys.argv, **extraDefaults):
    """Convenience function for instantiating a ClientParser
       and immediately parsing args with it, for the case when you don't
       need to add any extra options.
       """
    parser = Parser(clientClass, **extraDefaults)
    return parser.parse(argv)


def server(serverClass=Server.StandardServer, argv=sys.argv, **extraDefaults):
    """A similar convenience function for servers"""
    parser = Parser(serverClass, **extraDefaults)
    return parser.parse(argv)

### The End ###
