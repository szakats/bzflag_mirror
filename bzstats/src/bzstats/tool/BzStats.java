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

package bzstats.tool;

import java.awt.image.BufferedImage;
import java.io.*;
import java.util.Collection;

import org.apache.velocity.VelocityContext;

import bzstats.*;
import bzstats.chart.*;
import bzstats.event.EventSequence;
import bzstats.sort.Sorter;

/**
 * @author youth
 * @hidden
 *
 */
public class BzStats {

	private final static String DEFTEMPLATE = "index.vm";
	private final static String DEFOUTPUT = "index.html";

	public static void main(final String[] args) {

		if (args.length < 1) {
			System.out.println("USAGE: BZStats <inputfile> [<outputfile>] [<template>]" );
			return;
		}

		String filename = args[0];
		String outfile = (args.length > 1) ? args[1] : DEFOUTPUT;
		String template = (args.length > 2) ? args[2] : DEFTEMPLATE;

		File inputfile = new File(filename);

		LogReader reader = null;
		try {
			reader = new LogReader(new FileReader(inputfile));
		} catch (FileNotFoundException e) {
			System.err
					.println("File not found: " + inputfile.getAbsolutePath());
			return;
		}


		EventSequence  logperiod;
		try {
			logperiod = new EventSequence(reader);
		} catch (BzStatsException e4) {
			System.err.println(e4.getMessage());
			return;
		}

		try {
			reader.close();
		} catch (IOException e1) {
			System.err
			.println(e1.getMessage());
		}

		// history chart
		try {
			createHistoryChart(outfile + "-his.png", logperiod);
		} catch (BzStatsException e2) {
			System.err.println(e2.getMessage());
			return;
		}

		// pie chart
		PeriodStats stats = new PeriodStats();
		logperiod.collectStats(stats);

		try {
			createPieChart(outfile + "-pie.png", stats);
		} catch (BzStatsException e3) {
			System.err.println(e3.getMessage());
			return;
		}

		// template

		try {
			renderScoreboard(stats, outfile, template);
		} catch (BzStatsException e3) {
			System.err.println("Template rendering failed: " + e3.getMessage());
			return;
		}


	}

	/**
	 * @param filename
	 * @param stats
	 * @throws BZStatsException
	 */
	private static void createPieChart(String filename, PeriodStats stats) throws BzStatsException {
		KillRatioChart piechart = new KillRatioChart();
		piechart.setStats(stats);
		BufferedImage pieimage = piechart.getImage(800,500);
		AbstractChartFactory.savePNGImage(pieimage, filename);
	}

	/**
	 * @param filename
	 * @param logperiod
	 * @throws BZStatsException
	 */
	private static void createHistoryChart(String filename, EventSequence logperiod) throws BzStatsException {
		KillRatioHistoryChart factory = new KillRatioHistoryChart();
		factory.setPeriod(logperiod);

			BufferedImage image = factory.getImage(800,500);
			AbstractChartFactory.savePNGImage(image, filename);
	}

	private static void renderScoreboard(PeriodStats stats, String outputfile, String template) throws BzStatsException {

		Collection scores = Sorter.getSortedByScore(stats);
		Collection killratios = Sorter.getSortedByKillRatio(stats);

		VelocityContext context = new VelocityContext();
		context.put("scores", scores);
		context.put("killratios", killratios);
		context.put("historychart", outputfile +"-his.png");
		context.put("piechart", outputfile +"-pie.png");
		TemplateRenderer.render(template, outputfile, context);

	}


}
