"""optik.option_parser

Provides the OptionParser and Values classes.
"""

__revision__ = "$Id$"

# Copyright (c) 2001-2003 Gregory P. Ward.  All rights reserved.
# See the README.txt distributed with Optik for licensing terms.

# created 2001/10/17, GPW (from optik.py)

import sys, os
import types
from BZFlag.optik.option import Option, NO_DEFAULT
from BZFlag.optik.help import IndentedHelpFormatter
from BZFlag.optik.errors import OptionConflictError, OptionValueError, BadOptionError

__all__ = ['SUPPRESS_HELP', 'SUPPRESS_USAGE',
           'STD_HELP_OPTION', 'STD_VERSION_OPTION',
           'Values', 'OptionContainer', 'OptionGroup', 'OptionParser']

def get_prog_name ():
    return os.path.basename(sys.argv[0])


SUPPRESS_HELP = "SUPPRESS"+"HELP"
SUPPRESS_USAGE = "SUPPRESS"+"USAGE"

STD_HELP_OPTION = Option("-h", "--help",
                         action="help",
                         help="show this help message and exit")
STD_VERSION_OPTION = Option("--version",
                            action="version",
                            help="show program's version number and exit")


class Values:

    def __init__ (self, defaults=None):
        if defaults:
            for (attr, val) in defaults.items():
                setattr(self, attr, val)

    def __repr__ (self):
        return ("<%s at 0x%x: %r>"
                % (self.__class__.__name__, id(self), self.__dict__))

    def _update_careful (self, dict):
        """
        Update the option values from an arbitrary dictionary, but only
        use keys from dict that already have a corresponding attribute
        in self.  Any keys in dict without a corresponding attribute
        are silently ignored.
        """
        for attr in dir(self):
            if dict.has_key(attr):
                dval = dict[attr]
                if dval is not None:
                    setattr(self, attr, dval)

    def _update_loose (self, dict):
        """
        Update the option values from an arbitrary dictionary,
        using all keys from the dictionary regardless of whether
        they have a corresponding attribute in self or not.
        """
        self.__dict__.update(dict)

    def _update (self, dict, mode):
        if mode == "careful":
            self._update_careful(dict)
        elif mode == "loose":
            self._update_loose(dict)
        else:
            raise ValueError, "invalid update mode: %r" % mode

    def read_module (self, modname, mode="careful"):
        __import__(modname)
        mod = sys.modules[modname]
        self._update(vars(mod), mode)

    def read_file (self, filename, mode="careful"):
        vars = {}
        execfile(filename, vars)
        self._update(vars, mode)

    def ensure_value (self, attr, value):
        if not hasattr(self, attr) or getattr(self, attr) is None:
            setattr(self, attr, value)
        return getattr(self, attr)


class OptionContainer:

    """
    Abstract base class.

    Class attributes:
      standard_option_list : [Option]
        list of standard options that will be accepted by all instances
        of this parser class (intended to be overridden by subclasses).

    Instance attributes:
      option_list : [Option]
        the list of Option objects contained by this OptionContainer
      _short_opt : { string : Option }
        dictionary mapping short option strings, eg. "-f" or "-X",
        to the Option instances that implement them.  If an Option
        has multiple short option strings, it will appears in this
        dictionary multiple times. [1]
      _long_opt : { string : Option }
        dictionary mapping long option strings, eg. "--file" or
        "--exclude", to the Option instances that implement them.
        Again, a given Option can occur multiple times in this
        dictionary. [1]
      defaults : { string : any }
        dictionary mapping option destination names to default
        values for each destination [1]

    [1] These mappings are common to (shared by) all components of the
        controlling OptionParser, where they are initially created.

    """

    def __init__ (self, option_class, conflict_handler, description):
        # Initialize the option list and related data structures.
        # This method must be provided by subclasses, and it must
        # initialize at least the following instance attributes:
        # option_list, _short_opt, _long_opt, defaults.
        self._create_option_list()

        self.option_class = option_class
        self.set_conflict_handler(conflict_handler)
        self.set_description(description)

    def _create_option_mappings (self):
        # For use by OptionParser constructor -- create the master
        # option mappings used by this OptionParser and all
        # OptionGroups that it owns.
        self._short_opt = {}            # single letter -> Option instance
        self._long_opt = {}             # long option -> Option instance
        self.defaults = {}              # maps option dest -> default value


    def _share_option_mappings (self, parser):
        # For use by OptionGroup constructor -- use shared option
        # mappings from the OptionParser that owns this OptionGroup.
        self._short_opt = parser._short_opt
        self._long_opt = parser._long_opt
        self.defaults = parser.defaults

    def set_conflict_handler (self, handler):
        if handler not in ("ignore", "error", "resolve"):
            raise ValueError, "invalid conflict_resolution value %r" % handler
        self.conflict_handler = handler

    def set_description (self, description):
        self.description = description


    # -- Option-adding methods -----------------------------------------

    def _check_conflict (self, option):
        conflict_opts = []
        for opt in option._short_opts:
            if self._short_opt.has_key(opt):
                conflict_opts.append((opt, self._short_opt[opt]))
        for opt in option._long_opts:
            if self._long_opt.has_key(opt):
                conflict_opts.append((opt, self._long_opt[opt]))

        if conflict_opts:
            handler = self.conflict_handler
            if handler == "ignore":     # behaviour for Optik 1.0, 1.1
                pass
            elif handler == "error":    # new in 1.2
                raise OptionConflictError(
                    "conflicting option string(s): %s"
                    % ", ".join([co[0] for co in conflict_opts]),
                    option)
            elif handler == "resolve":  # new in 1.2
                for (opt, c_option) in conflict_opts:
                    if opt.startswith("--"):
                        c_option._long_opts.remove(opt)
                        del self._long_opt[opt]
                    else:
                        c_option._short_opts.remove(opt)
                        del self._short_opt[opt]
                    if not (c_option._short_opts or c_option._long_opts):
                        c_option.container.option_list.remove(c_option)

    def add_option (self, *args, **kwargs):
        """add_option(Option)
           add_option(opt_str, ..., kwarg=val, ...)
        """
        if type(args[0]) is types.StringType:
            option = self.option_class(*args, **kwargs)
        elif len(args) == 1 and not kwargs:
            option = args[0]
            if not isinstance(option, Option):
                raise TypeError, "not an Option instance: %r" % option
        else:
            raise TypeError, "invalid arguments"

        self._check_conflict(option)

        self.option_list.append(option)
        option.container = self
        for opt in option._short_opts:
            self._short_opt[opt] = option
        for opt in option._long_opts:
            self._long_opt[opt] = option

        if option.dest is not None:     # option has a dest, we need a default
            if option.default is not NO_DEFAULT:
                self.defaults[option.dest] = option.default
            elif not self.defaults.has_key(option.dest):
                self.defaults[option.dest] = None

        return option

    def add_options (self, option_list):
        for option in option_list:
            self.add_option(option)

    # -- Option query/removal methods ----------------------------------

    def get_option (self, opt_str):
        return (self._short_opt.get(opt_str) or
                self._long_opt.get(opt_str))

    def has_option (self, opt_str):
        return (self._short_opt.has_key(opt_str) or
                self._long_opt.has_key(opt_str))

    def remove_option (self, opt_str):
        option = self._short_opt.get(opt_str)
        if option is None:
            option = self._long_opt.get(opt_str)
        if option is None:
            raise ValueError("no such option %r" % opt_str)

        for opt in option._short_opts:
            del self._short_opt[opt]
        for opt in option._long_opts:
            del self._long_opt[opt]
        option.container.option_list.remove(option)


    # -- Help-formatting methods ---------------------------------------

    def format_option_help (self, formatter):
        if not self.option_list:
            return ""
        result = []
        for option in self.option_list:
            if not option.help is SUPPRESS_HELP:
                result.append(formatter.format_option(option))
        return "".join(result)

    def format_description (self, formatter):
        if self.description:
            return formatter.format_description(self.description)
        else:
            return ""

    def format_help (self, formatter):
        if self.description:
            desc = self.format_description(formatter) + "\n"
        else:
            desc = ""
        return desc + self.format_option_help(formatter)


class OptionGroup (OptionContainer):

    def __init__ (self, parser, title, description=None):
        self.parser = parser
        OptionContainer.__init__(
            self, parser.option_class, parser.conflict_handler, description)
        self.title = title

    def _create_option_list (self):
        self.option_list = []
        self._share_option_mappings(self.parser)

    def set_title (self, title):
        self.title = title

    # -- Help-formatting methods ---------------------------------------

    def format_help (self, formatter):
        result = formatter.format_heading(self.title)
        formatter.indent()
        result += OptionContainer.format_help(self, formatter)
        formatter.dedent()
        return result


class OptionParser (OptionContainer):

    """
    Class attributes:
      standard_option_list : [Option]
        list of standard options that will be accepted by all instances
        of this parser class (intended to be overridden by subclasses).

    Instance attributes:
      usage : string
        a usage string for your program.  Before it is displayed
        to the user, "%prog" will be expanded to the name of
        your program (self.prog or os.path.basename(sys.argv[0])).
      prog : string
        the name of the current program (to override
        os.path.basename(sys.argv[0])).

      allow_interspersed_args : boolean = true
        if true, positional arguments may be interspersed with options.
        Assuming -a and -b each take a single argument, the command-line
          -ablah foo bar -bboo baz
        will be interpreted the same as
          -ablah -bboo -- foo bar baz
        If this flag were false, that command line would be interpreted as
          -ablah -- foo bar -bboo baz
        -- ie. we stop processing options as soon as we see the first
        non-option argument.  (This is the tradition followed by
        Python's getopt module, Perl's Getopt::Std, and other argument-
        parsing libraries, but it is generally annoying to users.)

      rargs : [string]
        the argument list currently being parsed.  Only set when
        parse_args() is active, and continually trimmed down as
        we consume arguments.  Mainly there for the benefit of
        callback options.
      largs : [string]
        the list of leftover arguments that we have skipped while
        parsing options.  If allow_interspersed_args is false, this
        list is always empty.
      values : Values
        the set of option values currently being accumulated.  Only
        set when parse_args() is active.  Also mainly for callbacks.

    Because of the 'rargs', 'largs', and 'values' attributes,
    OptionParser is not thread-safe.  If, for some perverse reason, you
    need to parse command-line arguments simultaneously in different
    threads, use different OptionParser instances.

    """

    standard_option_list = []

    def __init__ (self,
                  usage=None,
                  option_list=None,
                  option_class=Option,
                  version=None,
                  conflict_handler="error",
                  description=None,
                  formatter=None,
                  add_help_option=1,
                  prog=None):
        OptionContainer.__init__(
            self, option_class, conflict_handler, description)
        self.set_usage(usage)
        self.prog = prog
        self.version = version
        self.allow_interspersed_args = 1
        if formatter is None:
            formatter = IndentedHelpFormatter()
        self.formatter = formatter

        # Populate the option list; initial sources are the
        # standard_option_list class attribute, the 'option_list'
        # argument, and the STD_VERSION_OPTION (if 'version' supplied)
        # and STD_HELP_OPTION globals.
        self._populate_option_list(option_list,
                                   add_help=add_help_option)

        self._init_parsing_state()

    # -- Private methods -----------------------------------------------
    # (used by our or OptionContainer's constructor)

    def _create_option_list (self):
        self.option_list = []
        self.option_groups = []
        self._create_option_mappings()

    def _populate_option_list (self, option_list, add_help=1):
        if self.standard_option_list:
            self.add_options(self.standard_option_list)
        if option_list:
            self.add_options(option_list)
        if self.version:
            self.add_option(STD_VERSION_OPTION)
        if add_help:
            self.add_option(STD_HELP_OPTION)

    def _init_parsing_state (self):
        # These are set in parse_args() for the convenience of callbacks.
        self.rargs = None
        self.largs = None
        self.values = None


    # -- Simple modifier methods ---------------------------------------

    def set_usage (self, usage):
        if usage is None:
            self.usage = "%prog [options]"
        elif usage is SUPPRESS_USAGE:
            self.usage = None
        elif usage.startswith("usage: "):
            # for backwards compatibility with Optik 1.3 and earlier
            self.usage = usage[7:]
        else:
            self.usage = usage

    def enable_interspersed_args (self):
        self.allow_interspersed_args = 1

    def disable_interspersed_args (self):
        self.allow_interspersed_args = 0

    def set_default (self, dest, value):
        self.defaults[dest] = value

    def set_defaults (self, **kwargs):
        self.defaults.update(kwargs)

    def get_default_values (self):
        return Values(self.defaults)


    # -- OptionGroup methods -------------------------------------------

    def add_option_group (self, *args, **kwargs):
        # XXX lots of overlap with OptionContainer.add_option()
        if type(args[0]) is types.StringType:
            group = OptionGroup(self, *args, **kwargs)
        elif len(args) == 1 and not kwargs:
            group = args[0]
            if not isinstance(group, OptionGroup):
                raise TypeError, "not an OptionGroup instance: %r" % group
            if group.parser is not self:
                raise ValueError, "invalid OptionGroup (wrong parser)"
        else:
            raise TypeError, "invalid arguments"

        self.option_groups.append(group)
        return group

    def get_option_group (self, opt_str):
        option = (self._short_opt.get(opt_str) or
                  self._long_opt.get(opt_str))
        if option and option.container is not self:
            return option.container
        return None


    # -- Option-parsing methods ----------------------------------------

    def _get_args (self, args):
        if args is None:
            return sys.argv[1:]
        else:
            return args[:]              # don't modify caller's list

    def parse_args (self, args=None, values=None):
        """
        parse_args(args : [string] = sys.argv[1:],
                   values : Values = None)
        -> (values : Values, args : [string])

        Parse the command-line options found in 'args' (default:
        sys.argv[1:]).  Any errors result in a call to 'error()', which
        by default prints the usage message to stderr and calls
        sys.exit() with an error message.  On success returns a pair
        (values, args) where 'values' is an Values instance (with all
        your option values) and 'args' is the list of arguments left
        over after parsing options.
        """
        rargs = self._get_args(args)
        if values is None:
            values = self.get_default_values()

        # Store the halves of the argument list as attributes for the
        # convenience of callbacks:
        #   rargs
        #     the rest of the command-line (the "r" stands for
        #     "remaining" or "right-hand")
        #   largs
        #     the leftover arguments -- ie. what's left after removing
        #     options and their arguments (the "l" stands for "leftover"
        #     or "left-hand")
        self.rargs = rargs
        self.largs = largs = []
        self.values = values

        try:
            stop = self._process_args(largs, rargs, values)
        except (BadOptionError, OptionValueError), err:
            self.error(err.msg)

        args = largs + rargs
        return self.check_values(values, args)

    def check_values (self, values, args):
        """
        check_values(values : Values, args : [string])
        -> (values : Values, args : [string])

        Check that the supplied option values and leftover arguments are
        valid.  Returns the option values and leftover arguments
        (possibly adjusted, possibly completely new -- whatever you
        like).  Default implementation just returns the passed-in
        values; subclasses may override as desired.
        """
        return (values, args)

    def _process_args (self, largs, rargs, values):
        """_process_args(largs : [string],
                         rargs : [string],
                         values : Values)

        Process command-line arguments and populate 'values', consuming
        options and arguments from 'rargs'.  If 'allow_interspersed_args' is
        false, stop at the first non-option argument.  If true, accumulate any
        interspersed non-option arguments in 'largs'.
        """
        while rargs:
            arg = rargs[0]
            # We handle bare "--" explicitly, and bare "-" is handled by the
            # standard arg handler since the short arg case ensures that the
            # len of the opt string is greater than 1.
            if arg == "--":
                del rargs[0]
                return
            elif arg[0:2] == "--":
                # process a single long option (possibly with value(s))
                self._process_long_opt(rargs, values)
            elif arg[:1] == "-" and len(arg) > 1:
                # process a cluster of short options (possibly with
                # value(s) for the last one only)
                self._process_short_opts(rargs, values)
            elif self.allow_interspersed_args:
                largs.append(arg)
                del rargs[0]
            else:
                return                  # stop now, leave this arg in rargs

        # Say this is the original argument list:
        # [arg0, arg1, ..., arg(i-1), arg(i), arg(i+1), ..., arg(N-1)]
        #                            ^
        # (we are about to process arg(i)).
        #
        # Then rargs is [arg(i), ..., arg(N-1)] and largs is a *subset* of
        # [arg0, ..., arg(i-1)] (any options and their arguments will have
        # been removed from largs).
        #
        # The while loop will usually consume 1 or more arguments per pass.
        # If it consumes 1 (eg. arg is an option that takes no arguments),
        # then after _process_arg() is done the situation is:
        #
        #   largs = subset of [arg0, ..., arg(i)]
        #   rargs = [arg(i+1), ..., arg(N-1)]
        #
        # If allow_interspersed_args is false, largs will always be
        # *empty* -- still a subset of [arg0, ..., arg(i-1)], but
        # not a very interesting subset!

    def _match_long_opt (self, opt):
        """_match_long_opt(opt : string) -> string

        Determine which long option string 'opt' matches, ie. which one
        it is an unambiguous abbrevation for.  Raises BadOptionError if
        'opt' doesn't unambiguously match any long option string.
        """
        return _match_abbrev(opt, self._long_opt)

    def _process_long_opt (self, rargs, values):
        arg = rargs.pop(0)

        # Value explicitly attached to arg?  Pretend it's the next
        # argument.
        if "=" in arg:
            (opt, next_arg) = arg.split("=", 1)
            rargs.insert(0, next_arg)
            had_explicit_value = 1
        else:
            opt = arg
            had_explicit_value = 0

        opt = self._match_long_opt(opt)
        option = self._long_opt[opt]
        if option.takes_value():
            nargs = option.nargs
            if len(rargs) < nargs:
                if nargs == 1:
                    self.error("%s option requires a value" % opt)
                else:
                    self.error("%s option requires %d values"
                               % (opt, nargs))
            elif nargs == 1:
                value = rargs.pop(0)
            else:
                value = tuple(rargs[0:nargs])
                del rargs[0:nargs]

        elif had_explicit_value:
            self.error("%s option does not take a value" % opt)

        else:
            value = None

        option.process(opt, value, values, self)

    def _process_short_opts (self, rargs, values):
        arg = rargs.pop(0)
        stop = 0
        i = 1
        for ch in arg[1:]:
            opt = "-" + ch
            option = self._short_opt.get(opt)
            i += 1                      # we have consumed a character

            if not option:
                self.error("no such option: %s" % opt)
            if option.takes_value():
                # Any characters left in arg?  Pretend they're the
                # next arg, and stop consuming characters of arg.
                if i < len(arg):
                    rargs.insert(0, arg[i:])
                    stop = 1

                nargs = option.nargs
                if len(rargs) < nargs:
                    if nargs == 1:
                        self.error("%s option requires a value" % opt)
                    else:
                        self.error("%s option requires %s values"
                                   % (opt, nargs))
                elif nargs == 1:
                    value = rargs.pop(0)
                else:
                    value = tuple(rargs[0:nargs])
                    del rargs[0:nargs]

            else:                       # option doesn't take a value
                value = None

            option.process(opt, value, values, self)

            if stop:
                break


    # -- Feedback methods ----------------------------------------------

    def error (self, msg):
        """error(msg : string)

        Print a usage message incorporating 'msg' to stderr and exit.
        If you override this in a subclass, it should not return -- it
        should either exit or raise an exception.
        """
        self.print_usage(sys.stderr)
        sys.exit("%s: error: %s" % (get_prog_name(), msg))

    def get_usage (self):
        if self.usage:
            return self.formatter.format_usage(
                self.usage.replace("%prog", get_prog_name()))
        else:
            return ""

    def print_usage (self, file=None):
        """print_usage(file : file = stdout)

        Print the usage message for the current program (self.usage) to
        'file' (default stdout).  Any occurence of the string "%prog" in
        self.usage is replaced with the name of the current program
        (basename of sys.argv[0]).  Does nothing if self.usage is empty
        or not defined.
        """
        if self.usage:
            print >>file, self.get_usage()

    def get_version (self):
        if self.version:
            return self.version.replace("%prog", get_prog_name())
        else:
            return ""

    def print_version (self, file=None):
        """print_version(file : file = stdout)

        Print the version message for this program (self.version) to
        'file' (default stdout).  As with print_usage(), any occurence
        of "%prog" in self.version is replaced by the current program's
        name.  Does nothing if self.version is empty or undefined.
        """
        if self.version:
            print >>file, self.get_version()

    def format_option_help (self, formatter=None):
        if formatter is None:
            formatter = self.formatter
        formatter.store_option_strings(self)
        result = []
        result.append(formatter.format_heading("options"))
        formatter.indent()
        if self.option_list:
            result.append(OptionContainer.format_option_help(self, formatter))
            result.append("\n")
        for group in self.option_groups:
            result.append(group.format_help(formatter))
            result.append("\n")
        formatter.dedent()
        # Drop the last "\n", or the header if no options or option groups:
        return "".join(result[:-1])

    def format_help (self, formatter=None):
        if formatter is None:
            formatter = self.formatter
        result = []
        if self.usage:
            result.append(self.get_usage() + "\n")
        if self.description:
            result.append(self.format_description(formatter) + "\n")
        result.append(self.format_option_help(formatter))
        return "".join(result)

    def print_help (self, file=None):
        """print_help(file : file = stdout)

        Print an extended help message, listing all options and any
        help text provided with them, to 'file' (default stdout).
        """
        if file is None:
            file = sys.stdout
        file.write(self.format_help())

# class OptionParser


def _match_abbrev (s, wordmap):
    """_match_abbrev(s : string, wordmap : {string : Option}) -> string

    Return the string key in 'wordmap' for which 's' is an unambiguous
    abbreviation.  If 's' is found to be ambiguous or doesn't match any of
    'words', raise BadOptionError.
    """
    # Is there an exact match?
    if wordmap.has_key(s):
        return s
    else:
        # Isolate all words with s as a prefix.
        possibilities = [word for word in wordmap.keys()
                         if word.startswith(s)]
        # No exact match, so there had better be just one possibility.
        if len(possibilities) == 1:
            return possibilities[0]
        elif not possibilities:
            raise BadOptionError("no such option: %s" % s)
        else:
            # More than one possible completion: ambiguous prefix.
            raise BadOptionError("ambiguous option: %s (%s?)"
                                 % (s, ", ".join(possibilities)))
