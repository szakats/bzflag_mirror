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

package bzstats.chart;

import java.awt.Color;
import java.util.*;

import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.StandardXYItemRenderer;
import org.jfree.chart.title.TextTitle;
import org.jfree.data.xy.DefaultTableXYDataset;
import org.jfree.data.xy.XYSeries;

import bzstats.*;
import bzstats.event.EventSequence;
import bzstats.event.GameEvent;

/**
 * @author youth
 * @depend - "uses" - bzstats.event.EventSequence
 */
public class KillRatioHistoryChart extends AbstractChartFactory {

	private final static int MAXPLAYERS = 8;

	private EventSequence period;

	private Map series;

	/**
	 * @param period
	 *            The period to set.
	 */
	public void setPeriod(EventSequence period) {
		this.period = period;
		series = new Hashtable();
	}

	protected JFreeChart getChart() {

		DefaultTableXYDataset dataset = new DefaultTableXYDataset();

		fillDataset(dataset);

		XYPlot plot = new XYPlot();

		NumberAxis xaxis = new NumberAxis("Time");
		xaxis.setTickLabelsVisible(false);
		NumberAxis yaxis = new NumberAxis("Killratio");

		plot.setDomainAxis(xaxis);
		plot.setRangeAxis(yaxis);
		plot.setDataset(dataset);
		plot.setRenderer(new StandardXYItemRenderer(
				StandardXYItemRenderer.LINES));

		JFreeChart chart = new JFreeChart("Killratio",
				JFreeChart.DEFAULT_TITLE_FONT, plot, true);

		chart.addSubtitle(new TextTitle("kills/deaths"));

		chart.setBackgroundPaint(Color.white);

		return chart;

	}

	/**
	 * Gets the XYSeries for a certain player.
	 * Creates a new one if player doesn't have one yet.
	 * @param player The player's name.
	 * @return An XYSeries for the player, or <code>null</code> if the
	 * maximum number of players has been exceeded.
	 */
	private XYSeries getSeries(String player) {

		if (!series.containsKey(player)) {
			if (series.size() < MAXPLAYERS) {
				series.put(player, new XYSeries(player, true, false));
			}
		}

		return (XYSeries) series.get(player);

	}

	private void fillDataset(DefaultTableXYDataset dataset) {

		ListIterator i = period.listIterator();

		GameEvent event;
		XYSeries playerseries;
		PeriodStats stats = new PeriodStats();
		int x;
		float y;
		Iterator statiter;
		Map.Entry entry;
		PlayerStats playerstats;
		while (i.hasNext()) {
			x = i.nextIndex();
			event = (GameEvent) i.next();
			event.collectStats(stats);

			statiter = stats.getPlayerStats().entrySet().iterator();
			while (statiter.hasNext()) {
				entry = (Map.Entry) statiter.next();
				playerstats = (PlayerStats) entry.getValue();
				y = playerstats.getKillRatio();
				playerseries = getSeries(playerstats.getName());

				if (playerseries != null) {
					playerseries.add(x, y);
				}
			}
		}

		// add the series to the dataset
		Iterator serieiter = series.entrySet().iterator();
		while (serieiter.hasNext()) {
			entry = (Map.Entry) serieiter.next();
			dataset.addSeries((XYSeries) entry.getValue());
		}

	}

}
