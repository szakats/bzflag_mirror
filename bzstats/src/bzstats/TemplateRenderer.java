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

import java.io.*;
import java.io.File;
import java.io.StringWriter;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;
import org.apache.velocity.exception.*;
import org.apache.velocity.exception.ParseErrorException;
import org.apache.velocity.exception.ResourceNotFoundException;

/**
 * @author youth
 * @version $Revision$
 */
public abstract class TemplateRenderer {

	private static VelocityEngine engine = null;

	public static void render(final String templatename,
			final String outputname, final VelocityContext context)
			throws BzStatsException {

		final Template template = getTemplate(templatename);

		final StringWriter writer = new StringWriter();

		try {
			template.merge(context, writer);
		} catch (ResourceNotFoundException e3) {
			throw new BzStatsException("Resource not found.", e3);
		} catch (ParseErrorException e3) {
			throw new BzStatsException("Syntax error in template.", e3);
		} catch (MethodInvocationException e3) {
			throw new BzStatsException("Failed to merge template.", e3);
		} catch (Exception e3) {
			throw new BzStatsException("Failed to merge template.", e3);
		}

		saveToFile(outputname, writer.toString());

	}

	private static VelocityEngine getEngine() throws BzStatsException {
		if (engine == null) {
			engine = new VelocityEngine();
			try {
				engine.init();
			} catch (Exception e) {
				throw new BzStatsException(
						"Could not initialize template engine.", e);
			}
		}
		return engine;
	}

	private static Template getTemplate(final String filename)
			throws BzStatsException {

		Template template;

		try {
			template = getEngine().getTemplate(filename);
		} catch (ResourceNotFoundException e1) {
			throw new BzStatsException("Cannot find template: " + filename, e1);
		} catch (ParseErrorException e1) {
			throw new BzStatsException("Syntax error in template: " + filename,
					e1);
		} catch (Exception e1) {
			throw new BzStatsException("Could not get template.", e1);
		}

		return template;

	}

	private static void saveToFile(final String filename, final String output)
			throws BzStatsException {

		final File outputfile = new File(filename);

		// overwrite
		if (outputfile.exists()) {
			if (outputfile.canWrite()) {
				outputfile.delete();
			} else {
				throw new BzStatsException("Cannot overwrite " + filename);
			}
		}

		try {
			if (!outputfile.createNewFile()) {
				throw new BzStatsException("Output file already exists: "
						+ filename);
			}
		} catch (IOException e2) {
			throw new BzStatsException(
					"Cannot create output file: " + filename, e2);
		}

		final Writer outwriter;

		try {
			outwriter = new FileWriter(outputfile);
		} catch (IOException e4) {
			throw new BzStatsException("Cannot write to file: " + filename, e4);
		}

		try {
			outwriter.write(output);
		} catch (IOException e5) {
			throw new BzStatsException("Cannot write to file: " + filename, e5);
		} finally {
			try {
				outwriter.close();
			} catch (IOException e6) {
				throw new BzStatsException("Cannot close output file: "
						+ filename, e6);
			}
		}
	}
}

