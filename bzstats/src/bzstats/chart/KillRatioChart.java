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

import java.util.*;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PiePlot3D;
import org.jfree.chart.title.TextTitle;
import org.jfree.data.general.DefaultPieDataset;
import org.jfree.util.Rotation;

import bzstats.PeriodStats;
import bzstats.PlayerStats;

/**
 * @author youth
 * @version $Revision$
 * @depend - "uses" - bzstats.PeriodStats
 */
public class KillRatioChart extends AbstractChartFactory {

	private final static int MAXPLAYERS = 8;

	private PeriodStats stats;

	/**
	 * @see bzstats.chart.ChartFactory#getChart()
	 */
	protected JFreeChart getChart() {

		DefaultPieDataset dataset = new DefaultPieDataset();


		Iterator playeriter = stats.getPlayerStats().values().iterator();

		PlayerStats player;
		int playercount = 0;
		while (playeriter.hasNext() && (playercount++ < MAXPLAYERS)) {
			player = (PlayerStats) playeriter.next();
			dataset.setValue(player.getName(), player.getKillRatio());
		}

//		 create chart and plot
		JFreeChart chart = ChartFactory.createPieChart3D(
				"Killratio", // chart title
				dataset, // data
				false, // include legend
				true, false);

		chart.addSubtitle(new TextTitle("kills/deaths"));

		final PiePlot3D plot = (PiePlot3D) chart.getPlot();
		plot.setStartAngle(290);
		plot.setDirection(Rotation.CLOCKWISE);
		plot.setForegroundAlpha(0.5f);
		plot.setBackgroundAlpha(0);
		plot.setNoDataMessage("No data to display");
		plot.setDepthFactor(.1f);

		return chart;
	}

	/**
	 * @param stats The stats to set.
	 */
	public void setStats(PeriodStats stats) {
		this.stats = stats;
	}
}
