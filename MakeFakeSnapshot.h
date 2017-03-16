typedef struct {
	int id;
	int localBalance;
	int moneyInTransfer;
} Snapshot;

JavaVMOption options[1];
JNIEnv *env;
JavaVM *jvm;
JavaVMInitArgs vm_args;
long status;
jclass cls;
jmethodID mid;
jint square;
jboolean not;
jint printit;
jmethodID constructor;

jfieldID id_fid;
jfieldID localBalance_fid;
jfieldID moneyInTransfer_fid;

jobject snapshot1;
jobject snapshot2;
jobject snapshot3;

Snapshot* snapshot1_C;
Snapshot* snapshot2_C;
Snapshot* snapshot3_C;
