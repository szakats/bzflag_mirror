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

/**
 * Represents statistics for one player over a period of time.
 *
 * @author youth
 *
 */
public class PlayerStats {

	private final String name;

	private String latestteam;

	private int kills;

	private int deaths;

	private int suicides;

	/**
	 * Constructor.
	 *
	 * @param name
	 *            The player's name
	 */
	public PlayerStats(String name) {
		this.name = name;
		latestteam = "Unknown";
		kills = 0;
		deaths = 0;
		suicides = 0;
	}

	/**
	 * The number of times the player got killed. (excluding suicides).
	 *
	 * @return Returns the deaths.
	 */
	public int getDeaths() {
		return deaths;
	}

	/**
	 * The number of times the player killed someone.
	 *
	 * @return Returns the kills.
	 */
	public int getKills() {
		return kills;
	}

	/**
	 * The number of times the player killed himself.
	 *
	 * @return Returns the suicides.
	 */
	public int getSuicides() {
		return suicides;
	}

	/**
	 * The name of the player.
	 *
	 * @return Returns the name.
	 */
	public String getName() {
		return name;
	}

	/**
	 * The score as defined in the game (kills - deaths).
	 *
	 * @return The score.
	 */
	public int getScore() {
		return getKills() - getDeaths() - getSuicides();
	}

	/**
	 * The ratio of kills and deaths (including suicides).
	 *
	 * @return Number of kills divided by number of deaths.
	 */
	public float getKillRatio() {

		float result = 0;

		if ((getDeaths()+ getSuicides()) > 0) {
			result = (float) getKills() / (float) (getDeaths() + getSuicides());
		}

		return result;
	}

	/**
	 * @return Returns the latest team.
	 */
	public String getLatestTeam() {
		return latestteam;
	}
	/**
	 * @param lastteam The lastteam to set.
	 */
	public void setLatesTeam(String lastteam) {
		this.latestteam = lastteam;
	}
	/**
	 * Add a number of kills to the statistics.
	 *
	 * @param nrkills
	 */
	public void addKills(final int nrkills) {
		kills += nrkills;
	}

	/**
	 * Add a number of deaths to the statistics.
	 *
	 * @param nrdeaths
	 */
	public void addDeaths(final int nrdeaths) {
		deaths += nrdeaths;
	}

	/**
	 * Add a number of suicides to the statistics.
	 *
	 * @param nrsuicides
	 */
	public void addSuicides(final int nrsuicides) {
		suicides += nrsuicides;
	}

}
