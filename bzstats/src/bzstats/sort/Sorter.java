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

package bzstats.sort;

import java.util.*;

import bzstats.PeriodStats;
import bzstats.PlayerStats;

/**
 * @author youth
 *
 */

public abstract class Sorter {

	public static Collection getSortedByKillRatio(PeriodStats stats) {
		ArrayList list = new ArrayList(stats.getPlayerStats().values());
		Collections.sort(list, new KillRatio());
		return list;
	}

	public static Collection getSortedByScore(PeriodStats stats) {
		ArrayList list = new ArrayList(stats.getPlayerStats().values());
		Collections.sort(list, new Score());
		return list;
	}


	private static class KillRatio implements Comparator {

		/**
		 * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
		 */
		public int compare(Object arg0, Object arg1) {

			PlayerStats player1 = (PlayerStats) arg0;
			PlayerStats player2 = (PlayerStats) arg1;

			if (player1.getKillRatio() < player2.getKillRatio()) {
				return 1;
			}
			else if (player1.getKillRatio() > player2.getKillRatio()) {
				return -1;
			}
			else {
				return 0;
			}
		}

	}

	private static class Score implements Comparator {

		/**
		 * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
		 */
		public int compare(Object arg0, Object arg1) {

			PlayerStats player1 = (PlayerStats) arg0;
			PlayerStats player2 = (PlayerStats) arg1;

			if (player1.getScore() < player2.getScore()) {
				return 1;
			}
			else if (player1.getScore() > player2.getScore()) {
				return -1;
			}
			else {
				return 0;
			}
		}

	}

}
