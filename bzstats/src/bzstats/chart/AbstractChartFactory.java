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

import java.awt.image.BufferedImage;
import java.io.*;

import org.jfree.chart.JFreeChart;
import org.jfree.chart.encoders.SunPNGEncoderAdapter;

import bzstats.BzStatsException;

/**
 * Abstract factory that produces a chart image from game information.
 *
 * @author youth
 * @version $Revision$
 */
public abstract class AbstractChartFactory {

	/**
	 * The actual chart that is created for the data.
	 *
	 * @return A JFreeChart object.
	 */
	protected abstract JFreeChart getChart();

	/**
	 * Get an image for the chart.
	 *
	 * @param width
	 *            Width in pixels for the image.
	 * @param height
	 *            Height in pixels for the image.
	 * @return The image.
	 */
	public final BufferedImage getImage(int width, int height) {
		return getChart().createBufferedImage(width, height);
	}

	/**
	 * Utility function to save an image as a PNG file.
	 *
	 * @param image
	 *            The image to save.
	 * @param filename
	 *            The filename to save the image as.
	 * @throws BZStatsException
	 *             If the file could not be written.
	 */
	public static final void savePNGImage(BufferedImage image, String filename)
			throws BzStatsException {

		File outputfile = new File(filename);

		// overwrite
		if (outputfile.exists()) {
			if (outputfile.canWrite()) {
				outputfile.delete();
			} else {
				throw new BzStatsException("Cannot overwrite " + filename);
			}
		}

		boolean filecreated;
		try {
			filecreated = outputfile.createNewFile();
		} catch (IOException e2) {
			throw new BzStatsException("Failed to create file " + filename);
		}

		if (filecreated) {

			OutputStream out = null;
			try {
				out = new FileOutputStream(outputfile);
			} catch (FileNotFoundException e3) {
				throw new BzStatsException("Failed to write to file "
						+ filename);
			}

			SunPNGEncoderAdapter encoder = new SunPNGEncoderAdapter();

			try {
				encoder.encode(image, out);
			} catch (IOException e) {
				throw new BzStatsException("Failed to encode file " + filename);
			} finally {
				try {
					out.close();
				} catch (IOException e1) {
					throw new BzStatsException("Failed to close file "
							+ filename);
				}
			}
		} else {
			throw new BzStatsException("Failed to create file " + filename);
		}

	}

}
