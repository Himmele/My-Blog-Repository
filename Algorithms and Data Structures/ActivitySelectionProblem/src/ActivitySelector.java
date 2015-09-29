import java.util.Collections;
import java.util.List;
import java.util.ArrayList;

public class ActivitySelector {
	private List<Activity> mActivities = new ArrayList<Activity>();
	
	public static class Activity implements Comparable<Activity> {
		private int mStartTime;
		private int mFinishTime;
		
		public Activity(int startTime, int finishTime) {
			mStartTime = startTime;
			mFinishTime = finishTime;
		}
		
		public int getStartTime() {
			return mStartTime;
		}
		
		public int getFinishTime() {
			return mFinishTime;
		}
		
		@Override
		public int compareTo(Activity activity) {
			if (this.mFinishTime == activity.mFinishTime) {
				return 0;
			} else if (this.mFinishTime > activity.mFinishTime) {
				return 1;
			} else {
				return -1;
			}
		}
	}
	
	public void addActivity(Activity activity) {
		mActivities.add(activity);
	}
	
	public List<Activity> selectActivityies() {		
		Collections.sort(mActivities);
		List<Activity> selectedActivities = new ArrayList<Activity>();
		selectedActivities.add(mActivities.get(0));
//		recursiveActivitySelector(selectedActivities, 0);
		iterativeActivitySelector(selectedActivities);
		return selectedActivities;
	}
	
	private void recursiveActivitySelector(List<Activity> selectedActivities, int i) {
		int m = i + 1;
		while (m < mActivities.size() && mActivities.get(m).getStartTime() < mActivities.get(i).getFinishTime()) {
			m++;
		}
		if (m < mActivities.size()) {
			selectedActivities.add(mActivities.get(m));
			recursiveActivitySelector(selectedActivities, m);
		}
	}
	
	private void iterativeActivitySelector(List<Activity> selectedActivities) {
		int i = 0;
		for (int m = 1; m < mActivities.size(); m++) {
			if (mActivities.get(m).getStartTime() >= mActivities.get(i).mFinishTime) {
				selectedActivities.add(mActivities.get(m));
				i = m;
			}
		}
	}
	
	public static void main(String[] args) {
		ActivitySelector as = new ActivitySelector();
		as.addActivity(new Activity(8, 12));
		as.addActivity(new Activity(5, 7));
		as.addActivity(new Activity(5, 9));
		as.addActivity(new Activity(1, 4));
		as.addActivity(new Activity(0, 6));
		as.addActivity(new Activity(3, 5));
		as.addActivity(new Activity(12, 14));
		as.addActivity(new Activity(8, 11));
		as.addActivity(new Activity(6, 10));
		as.addActivity(new Activity(3, 8));
		as.addActivity(new Activity(2, 13));
		
		List<Activity> selectedActivities = as.selectActivityies();
		
		for (Activity activity : selectedActivities) {
			System.out.print(activity.getStartTime() + ":" + activity.getFinishTime() + " -> ");
		}
		System.out.println();
	}
}
