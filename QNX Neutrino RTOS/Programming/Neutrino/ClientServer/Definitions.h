struct my_msg
{
   short type;
   struct sigevent event;
};

#define MY_PULSE_CODE _PULSE_CODE_MINAVAIL+7
#define MSG_GIVE_PULSE _IO_MAX+5
#define MY_SERV "QNXServer"