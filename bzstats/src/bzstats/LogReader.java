/***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

The Original Code is BZStats.

The Initial Developer of the Original Code is
Michel de Bree (youth).
Portions created by the Initial Developer are Copyright (C) 2005
the Initial Developer. All Rights Reserved.

Contributor(s):

***** END LICENSE BLOCK *****/

package bzstats;

import java.io.*;

import org.apache.log4j.Logger;

import bzstats.event.*;

/**
 * Can read events from a logfile.
 * @author youth
 * @depend 1 produces * bzstats.event.GameEvent
 */
public class LogReader extends BufferedReader {

	private final static Logger LOG = Logger.getLogger(LogReader.class);

	public LogReader(Reader reader) {
		super(reader);
	}

	/**
	 * Get the next recognized event from the logfile.
	 * @return A GameEvent, or <code>null</code> if the end of the file has been reached.
	 * @throws IOException
	 * @throws BzStatsException
	 */
	public GameEvent nextEvent() throws IOException, BzStatsException {

		String logline;
		GameEvent event = null;

		boolean eof = false;

		while ((event == null) && !eof) {

			logline = readLine();
			eof = (logline == null);

			if (!eof) {
				event = getEvent(logline);
			}

		}

		return event;

	}

	/**
	 * Parses a logline and returns a GameEvent if found, else <code>null</code>
	 * @param logline One line from the BzFlag logfile
	 * @return A GameEvent, or <code>null</code> if no GameEvent found.
	 * @throws BZStatsException
	 */
	private static GameEvent getEvent(final String logline) throws BzStatsException {

		final GameEvent killevent = new KillEvent();
		final GameEvent suicideevent = new SuicideEvent();
		final GameEvent joinevent = new JoinEvent();

		GameEvent result = null;

		if (killevent.parseEvent(logline)) {
			result = killevent;
		}
		else if (suicideevent.parseEvent(logline)) {
			result = suicideevent;
		}
		else if (joinevent.parseEvent(logline)) {
			result = joinevent;
		}

		return result;

	}

}
