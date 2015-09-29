package vu.de.urpool.DroidTrail;

interface IDroidTrailService {	
	boolean startLogging(String logFileName);
	void stopLogging();
	boolean isLogging();
	void setMinTime(long minTime);
	long getMinTime();
	void setMinDistance(float minDistance);
	float getMinDistance();
	String getLogFileName();
}