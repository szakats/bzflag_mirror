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

import bzstats.BzStatsException;
import bzstats.PeriodStats;

/**
 * @author youth
 */
public class JoinEvent extends GameEvent {

	private final static Pattern PATTERN = Pattern.compile("^\\*\\*\\* '(.*)' joined the game as (.*)\\.$");

	private String player;
	private String team;

	/**
	 * @return Returns the player.
	 */
	public String getPlayer() {
		return player;
	}
	/**
	 * @return Returns the team.
	 */
	public String getTeam() {
		return team;
	}
	public boolean parseEvent(String logline) throws BzStatsException {

		final Matcher match = PATTERN.matcher(logline);
		final boolean found = match.matches();

		if (found) {
			player = match.group(1);
			team = match.group(2);
		}

		return found;
	}

	/**
	 * @see bzstats.event.GameEvent#collectStats(bzstats.PeriodStats)
	 */
	public void collectStats(PeriodStats stats) {
		stats.getPlayerStats(player).setLatesTeam(team);

	}

}
