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

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.log4j.Logger;

import bzstats.BzStatsException;
import bzstats.PeriodStats;

/**
 * @author youth
 *
 */
public class KillEvent extends GameEvent {

	private final static Logger LOG = Logger.getLogger(KillEvent.class);
	private final static Pattern PATTERN = Pattern.compile("^\\*\\*\\* '(.*)' destroyed by '(.*)'.*");

	private String killer;
	private String victim;

	/**
	 * @return Returns the killer.
	 */
	public String getKiller() {
		return killer;
	}
	/**
	 * @return Returns the victim.
	 */
	public String getVictim() {
		return victim;
	}


	public boolean parseEvent(final String logline) throws BzStatsException {

		final Matcher match = PATTERN.matcher(logline);
		final boolean found = match.matches();

		if (found) {
			victim = match.group(1);
			killer = match.group(2);
		}

		return found;
	}

	public void collectStats(final PeriodStats stats) {
		stats.getPlayerStats(killer).addKills(1);
		stats.getPlayerStats(victim).addDeaths(1);
	}
}
