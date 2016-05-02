#define TOUCHPAD_DELAY        45

#ifdef TOUCHPAD_SUPPORT
touchpad_report_t* tpreport;
extern touchpad_info_t* tpinfo;
#endif

void initTP();//初始化TP
void endTP();//释放
void readTP();//读取TP状态
int getTouchedZone5();//把TouchPad分成5个区域，读取值
int getTouchedZone4();//把TouchPad分成4个区域，读取值
int getTouchedZone9();//把TouchPad分成9个区域，读取值
int getX_Velocity();//读取X的相对坐标
int getY_Velocity();//读取Y的相对坐标
int getX();//读取X的绝对坐标
int getY();//读取Y的绝对坐标
int isTPTouched();//TP是否被触摸
int isTPPressed();//TP是否被按下
int isTouchPad();//是否拥有Touchpad
