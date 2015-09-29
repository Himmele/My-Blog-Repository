package vu.de.urpool.DroidTrail;

import java.io.DataOutputStream;
import java.io.IOException;

public class GoogleEarthConverter implements IDroidTrailConverter {
	public boolean writeHeader(DataOutputStream output, String trailName) {
		try {
			output.writeChars("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			output.writeChars("<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
			output.writeChars("  <Document>\n");
			output.writeChars("    <name>" + trailName + "</name>\n");
			output.writeChars("    <Style id=\"pathStyle\">\n");
			output.writeChars("      <LineStyle>\n");
			output.writeChars("        <color>7fff0000</color>\n");
			output.writeChars("        <width>4</width>\n");
			output.writeChars("      </LineStyle>\n");
			output.writeChars("    </Style>\n");
			output.writeChars("    <Placemark>\n");
			output.writeChars("      <name>Path</name>\n");
			output.writeChars("      <styleUrl>#pathStyle</styleUrl>\n");
			output.writeChars("      <LineString>\n");			
			output.writeChars("        <coordinates>\n");
		} catch (IOException e) {
			return false;
		}
		return true;
	}
	
	public boolean writeCoordinates(DataOutputStream output, double longitude, double latitude) {
		try {
			output.writeChars("          " + String.valueOf(longitude) + "," + String.valueOf(latitude) + "\n");
		} catch (IOException e) {
			return false;
		}
		return true;
	}
	
	public boolean writeFooter(DataOutputStream output) {
		try {
			output.writeChars("        </coordinates>\n");		
			output.writeChars("      </LineString>\n");
			output.writeChars("    </Placemark>\n");
			output.writeChars("  </Document>\n");
			output.writeChars("</kml>\n");
		} catch (IOException e) {
			return false;
		}
		return true;
	}
}
