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

import bzstats.BzStatsException;
import bzstats.PeriodStats;
import junit.framework.TestCase;

/**
 * @author youth
 * @hidden
 */
public class EventsTest extends TestCase {

	private final static String KILL01 = "*** 'de slager' destroyed by 'DataLore'.";
	private final static String KILL02 = "*** 'it's time' destroyed by ''foo''.";
	private final static String KILL03 = "*** ''I' was destroyed by 'someone'' destroyed by '\"killer\"'.";

	private final static String SUICIDE01 = "*** 'DataLore' blew myself up.";
	private final static String SUICIDE02 = "*** 'it's time' blew myself up.";
	private final static String SUICIDE03 = "*** '*** 'I' blew myself up.' blew myself up.";

	private final static String JOIN01 = "*** 'DataLore' joined the game as Red Team.";

	public void testKillEvent() {

		KillEvent event = new KillEvent();

		try {
			assertTrue(event.parseEvent(KILL01));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}

		assertEquals("de slager", event.getVictim());
		assertEquals("DataLore", event.getKiller());

		try {
			assertTrue(event.parseEvent(KILL02));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}

		assertEquals("it's time", event.getVictim());
		assertEquals("'foo'", event.getKiller());

		try {
			assertTrue(event.parseEvent(KILL03));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}

		assertEquals("'I' was destroyed by 'someone'", event.getVictim());
		assertEquals("\"killer\"", event.getKiller());


		try {
			assertFalse(event.parseEvent(SUICIDE01));
		} catch (BzStatsException e1) {
			fail(e1.getMessage());
		}

	}

	public void testSuicideEvent() {

		SuicideEvent event = new SuicideEvent();

		try {
			assertTrue(event.parseEvent(SUICIDE01));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}
		assertEquals("DataLore", event.getVictim());

		try {
			assertTrue(event.parseEvent(SUICIDE02));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}
		assertEquals("it's time", event.getVictim());

		try {
			assertTrue(event.parseEvent(SUICIDE03));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}
		assertEquals("*** 'I' blew myself up.", event.getVictim());

	}

	public void testJoinEvent() {

		JoinEvent event = new JoinEvent();

		try {
			assertTrue(event.parseEvent(JOIN01));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}
		assertEquals("DataLore", event.getPlayer());
		assertEquals("Red Team", event.getTeam());

	}

	public void testStats() {

		PeriodStats stats = new PeriodStats();

		assertEquals(0, stats.getPlayerStats("de slager").getKills());
		assertEquals(0, stats.getPlayerStats("de slager").getDeaths());
		assertEquals(0, stats.getPlayerStats("de slager").getSuicides());

		assertEquals(0, stats.getPlayerStats("DataLore").getKills());
		assertEquals(0, stats.getPlayerStats("DataLore").getDeaths());
		assertEquals(0, stats.getPlayerStats("DataLore").getSuicides());

		GameEvent event = new SuicideEvent();
		try {
			assertTrue(event.parseEvent(SUICIDE01));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}

		event.collectStats(stats);

		assertEquals(0, stats.getPlayerStats("de slager").getKills());
		assertEquals(0, stats.getPlayerStats("de slager").getDeaths());
		assertEquals(0, stats.getPlayerStats("de slager").getSuicides());

		assertEquals(0, stats.getPlayerStats("DataLore").getKills());
		assertEquals(0, stats.getPlayerStats("DataLore").getDeaths());
		assertEquals(1, stats.getPlayerStats("DataLore").getSuicides());

		event = new KillEvent();
		try {
			assertTrue(event.parseEvent(KILL01));
		} catch (BzStatsException e) {
			fail(e.getMessage());
		}

		event.collectStats(stats);

		assertEquals(0, stats.getPlayerStats("de slager").getKills());
		assertEquals(1, stats.getPlayerStats("de slager").getDeaths());
		assertEquals(0, stats.getPlayerStats("de slager").getSuicides());

		assertEquals(1, stats.getPlayerStats("DataLore").getKills());
		assertEquals(0, stats.getPlayerStats("DataLore").getDeaths());
		assertEquals(1, stats.getPlayerStats("DataLore").getSuicides());

	}

}
