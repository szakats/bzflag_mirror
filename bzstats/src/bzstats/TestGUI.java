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

import javax.swing.*;
import javax.swing.JFrame;
import javax.swing.JLabel;

import bzstats.chart.KillRatioHistoryChart;
import bzstats.event.EventSequence;
/**
 * @author youth
 * @hidden
 */
public class TestGUI extends JFrame {

	private transient javax.swing.JPanel jContentPane = null;

	private transient JLabel jLabel = null;

	private final KillRatioHistoryChart factory = new KillRatioHistoryChart();

	public static void main(final String[] args) {
		final JFrame gui = new TestGUI();
		gui.show();

	}
	/**
	 * This is the default constructor
	 */
	public TestGUI() {
		super();
		initialize();
		//setImage();

	}

	public TestGUI(EventSequence period) {
		super();
		initialize();
		factory.setPeriod(period);
		setImage();

	}

	private void setImage() {

//		ChartFactory factory = new ChartFactory();
		jLabel.setIcon(new ImageIcon(factory.getImage(800,500)));
		pack();

	}

	/**
	 * This method initializes this
	 *
	 * @return void
	 */
	private void initialize() {
		this.setResizable(false);
		this.setDefaultCloseOperation(javax.swing.JFrame.EXIT_ON_CLOSE);
		this.setSize(300,200);
		this.setContentPane(getJContentPane());
		this.setTitle("JFrame");
	}
	/**
	 * This method initializes jContentPane
	 *
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getJContentPane() {
		if(jContentPane == null) {
			jLabel = new JLabel();
			jContentPane = new javax.swing.JPanel();
			jContentPane.setLayout(new java.awt.BorderLayout());
			jLabel.setText("JLabel");
			jContentPane.add(jLabel, java.awt.BorderLayout.CENTER);
		}
		return jContentPane;
	}
}
