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

package bzstats.event;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

import bzstats.*;

/**
 * A sequential list of game events.
 * @author youth
 * @has 1 - * bzstats.event.GameEvent
 * @depend - "uses" - bzstats.LogReader
 *
 */
public class EventSequence extends ArrayList implements Period {

	/**
	 * Add a new event to the end.
	 * @param event The event to add.
	 */
	public void addEvent(final GameEvent event) {
		add(event);
	}

	/**
	 * Construct a gameperiod and fill it with data from a LogReader.
	 * @param reader
	 * @throws BZStatsException
	 */
	public EventSequence (final LogReader reader) throws BzStatsException {

		GameEvent event;
		try {
			event = reader.nextEvent();
			while (event != null) {
				addEvent(event);
				event = reader.nextEvent();
			}
		} catch (IOException e1) {
			throw new BzStatsException("Could not read logfile.",e1);
		}

	}

	/**
	 * Collect statistics from a GamePeriod.
	 * @param period The GamePeriod to analyze.
	 */
	public void collectStats(final PeriodStats stats) {

		final Iterator eventiter = iterator();
		GameEvent event;
		while(eventiter.hasNext()) {
			event = (GameEvent) eventiter.next();
			event.collectStats(stats);
		}

	}

}
