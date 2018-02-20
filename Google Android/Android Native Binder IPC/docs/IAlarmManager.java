/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: frameworks/base/core/java/android/app/IAlarmManager.aidl
 */
package android.app;

/**
 * System private API for talking with the alarm manager service.
 *
 * {@hide}
 */
public interface IAlarmManager extends android.os.IInterface {
	/** Local-side IPC implementation stub class. */
	public static abstract class Stub extends android.os.Binder implements android.app.IAlarmManager {
		private static final java.lang.String DESCRIPTOR = "android.app.IAlarmManager";

		/** Construct the stub at attach it to the interface. */
		public Stub() {
			this.attachInterface(this, DESCRIPTOR);
		}

		/**
		 * Cast an IBinder object into an android.app.IAlarmManager interface,
		 * generating a proxy if needed.
		 */
		public static android.app.IAlarmManager asInterface(android.os.IBinder obj) {
			if ((obj == null)) {
				return null;
			}
			android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
			if (((iin != null) && (iin instanceof android.app.IAlarmManager))) {
				return ((android.app.IAlarmManager) iin);
			}
			return new android.app.IAlarmManager.Stub.Proxy(obj);
		}

		@Override
		public android.os.IBinder asBinder() {
			return this;
		}

		@Override
		public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags)
				throws android.os.RemoteException {
			switch (code) {
			case INTERFACE_TRANSACTION: {
				reply.writeString(DESCRIPTOR);
				return true;
			}
			case TRANSACTION_set: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				int _arg1;
				_arg1 = data.readInt();
				long _arg2;
				_arg2 = data.readLong();
				long _arg3;
				_arg3 = data.readLong();
				long _arg4;
				_arg4 = data.readLong();
				int _arg5;
				_arg5 = data.readInt();
				android.app.PendingIntent _arg6;
				if ((0 != data.readInt())) {
					_arg6 = android.app.PendingIntent.CREATOR.createFromParcel(data);
				} else {
					_arg6 = null;
				}
				android.app.IAlarmListener _arg7;
				_arg7 = android.app.IAlarmListener.Stub.asInterface(data.readStrongBinder());
				java.lang.String _arg8;
				_arg8 = data.readString();
				android.os.WorkSource _arg9;
				if ((0 != data.readInt())) {
					_arg9 = android.os.WorkSource.CREATOR.createFromParcel(data);
				} else {
					_arg9 = null;
				}
				android.app.AlarmManager.AlarmClockInfo _arg10;
				if ((0 != data.readInt())) {
					_arg10 = android.app.AlarmManager.AlarmClockInfo.CREATOR.createFromParcel(data);
				} else {
					_arg10 = null;
				}
				this.set(_arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6, _arg7, _arg8, _arg9, _arg10);
				reply.writeNoException();
				return true;
			}
			case TRANSACTION_setTime: {
				data.enforceInterface(DESCRIPTOR);
				long _arg0;
				_arg0 = data.readLong();
				boolean _result = this.setTime(_arg0);
				reply.writeNoException();
				reply.writeInt(((_result) ? (1) : (0)));
				return true;
			}
			case TRANSACTION_setTimeZone: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				this.setTimeZone(_arg0);
				reply.writeNoException();
				return true;
			}
			case TRANSACTION_remove: {
				data.enforceInterface(DESCRIPTOR);
				android.app.PendingIntent _arg0;
				if ((0 != data.readInt())) {
					_arg0 = android.app.PendingIntent.CREATOR.createFromParcel(data);
				} else {
					_arg0 = null;
				}
				android.app.IAlarmListener _arg1;
				_arg1 = android.app.IAlarmListener.Stub.asInterface(data.readStrongBinder());
				this.remove(_arg0, _arg1);
				reply.writeNoException();
				return true;
			}
			case TRANSACTION_getNextWakeFromIdleTime: {
				data.enforceInterface(DESCRIPTOR);
				long _result = this.getNextWakeFromIdleTime();
				reply.writeNoException();
				reply.writeLong(_result);
				return true;
			}
			case TRANSACTION_getNextAlarmClock: {
				data.enforceInterface(DESCRIPTOR);
				int _arg0;
				_arg0 = data.readInt();
				android.app.AlarmManager.AlarmClockInfo _result = this.getNextAlarmClock(_arg0);
				reply.writeNoException();
				if ((_result != null)) {
					reply.writeInt(1);
					_result.writeToParcel(reply, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
				} else {
					reply.writeInt(0);
				}
				return true;
			}
			}
			return super.onTransact(code, data, reply, flags);
		}

		private static class Proxy implements android.app.IAlarmManager {
			private android.os.IBinder mRemote;

			Proxy(android.os.IBinder remote) {
				mRemote = remote;
			}

			@Override
			public android.os.IBinder asBinder() {
				return mRemote;
			}

			public java.lang.String getInterfaceDescriptor() {
				return DESCRIPTOR;
			}

			/**
			 * windowLength == 0 means exact; windowLength < 0 means the let the
			 * OS decide
			 */
			@Override
			public void set(java.lang.String callingPackage, int type, long triggerAtTime, long windowLength,
					long interval, int flags, android.app.PendingIntent operation, android.app.IAlarmListener listener,
					java.lang.String listenerTag, android.os.WorkSource workSource,
					android.app.AlarmManager.AlarmClockInfo alarmClock) throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(callingPackage);
					_data.writeInt(type);
					_data.writeLong(triggerAtTime);
					_data.writeLong(windowLength);
					_data.writeLong(interval);
					_data.writeInt(flags);
					if ((operation != null)) {
						_data.writeInt(1);
						operation.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					_data.writeStrongBinder((((listener != null)) ? (listener.asBinder()) : (null)));
					_data.writeString(listenerTag);
					if ((workSource != null)) {
						_data.writeInt(1);
						workSource.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					if ((alarmClock != null)) {
						_data.writeInt(1);
						alarmClock.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					mRemote.transact(Stub.TRANSACTION_set, _data, _reply, 0);
					_reply.readException();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
			}

			@Override
			public boolean setTime(long millis) throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				boolean _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeLong(millis);
					mRemote.transact(Stub.TRANSACTION_setTime, _data, _reply, 0);
					_reply.readException();
					_result = (0 != _reply.readInt());
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public void setTimeZone(java.lang.String zone) throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(zone);
					mRemote.transact(Stub.TRANSACTION_setTimeZone, _data, _reply, 0);
					_reply.readException();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
			}

			@Override
			public void remove(android.app.PendingIntent operation, android.app.IAlarmListener listener)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					if ((operation != null)) {
						_data.writeInt(1);
						operation.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					_data.writeStrongBinder((((listener != null)) ? (listener.asBinder()) : (null)));
					mRemote.transact(Stub.TRANSACTION_remove, _data, _reply, 0);
					_reply.readException();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
			}

			@Override
			public long getNextWakeFromIdleTime() throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				long _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_getNextWakeFromIdleTime, _data, _reply, 0);
					_reply.readException();
					_result = _reply.readLong();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public android.app.AlarmManager.AlarmClockInfo getNextAlarmClock(int userId)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				android.app.AlarmManager.AlarmClockInfo _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeInt(userId);
					mRemote.transact(Stub.TRANSACTION_getNextAlarmClock, _data, _reply, 0);
					_reply.readException();
					if ((0 != _reply.readInt())) {
						_result = android.app.AlarmManager.AlarmClockInfo.CREATOR.createFromParcel(_reply);
					} else {
						_result = null;
					}
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}
		}

		static final int TRANSACTION_set = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
		static final int TRANSACTION_setTime = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
		static final int TRANSACTION_setTimeZone = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
		static final int TRANSACTION_remove = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
		static final int TRANSACTION_getNextWakeFromIdleTime = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
		static final int TRANSACTION_getNextAlarmClock = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
	}

	/**
	 * windowLength == 0 means exact; windowLength < 0 means the let the OS
	 * decide
	 */
	public void set(java.lang.String callingPackage, int type, long triggerAtTime, long windowLength, long interval,
			int flags, android.app.PendingIntent operation, android.app.IAlarmListener listener,
			java.lang.String listenerTag, android.os.WorkSource workSource,
			android.app.AlarmManager.AlarmClockInfo alarmClock) throws android.os.RemoteException;

	public boolean setTime(long millis) throws android.os.RemoteException;

	public void setTimeZone(java.lang.String zone) throws android.os.RemoteException;

	public void remove(android.app.PendingIntent operation, android.app.IAlarmListener listener)
			throws android.os.RemoteException;

	public long getNextWakeFromIdleTime() throws android.os.RemoteException;

	public android.app.AlarmManager.AlarmClockInfo getNextAlarmClock(int userId) throws android.os.RemoteException;
}
