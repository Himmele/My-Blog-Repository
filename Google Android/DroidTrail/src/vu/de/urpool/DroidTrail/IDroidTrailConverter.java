package vu.de.urpool.DroidTrail;

import java.io.DataOutputStream;

public interface IDroidTrailConverter {
	public boolean writeHeader(DataOutputStream output, String trailName);
	public boolean writeFooter(DataOutputStream output);
	public boolean writeCoordinates(DataOutputStream output, double longitude, double latitude);
}
