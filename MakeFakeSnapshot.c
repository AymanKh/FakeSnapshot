#include <string.h>
#include <jni.h>
#include <assert.h>
#include <stdlib.h>
#include "MakeFakeSnapshot.h"



void MakeSnapshotObject()
{
	snapshot1 = (*env)->NewObject(env, cls, constructor, 1, 10, 100);
	snapshot2 = (*env)->NewObject(env, cls, constructor, 2, 20, 200);
	snapshot3 = (*env)->NewObject(env, cls, constructor, 3, 30, 300);
}

void GetFieldIDs()
{
	id_fid = (*env)->GetFieldID(env, cls, "id", "I");
	localBalance_fid = (*env)->GetFieldID(env, cls, "localBalance", "I");
	moneyInTransfer_fid = (*env)->GetFieldID(env, cls, "moneyInTransfer", "I");
}

void PopulateCstructs()
{
	jint id = (*env)->GetIntField(env, snapshot1, id_fid);
	jint localBalance = (*env)->GetIntField(env, snapshot1, localBalance_fid);
	jint moneyInTransfer = (*env)->GetIntField(env, snapshot1, moneyInTransfer_fid);

	snapshot1_C = malloc(sizeof(*snapshot1_C));
	snapshot1_C->id = id;
	snapshot1_C->localBalance = localBalance;
	snapshot1_C->moneyInTransfer = moneyInTransfer;

	printf("%d %d %d\n",(int) snapshot1_C->id, (int) snapshot1_C->localBalance, (int) snapshot1_C->moneyInTransfer);

	id = (*env)->GetIntField(env, snapshot2, id_fid);
	localBalance = (*env)->GetIntField(env, snapshot2, localBalance_fid);
	moneyInTransfer = (*env)->GetIntField(env, snapshot2, moneyInTransfer_fid);

	snapshot2_C = malloc(sizeof(*snapshot2_C));
	snapshot2_C->id = id;
	snapshot2_C->localBalance = localBalance;
	snapshot2_C->moneyInTransfer = moneyInTransfer;

	printf("%d %d %d\n",(int) snapshot2_C->id, (int) snapshot2_C->localBalance, (int) snapshot2_C->moneyInTransfer);

	id = (*env)->GetIntField(env, snapshot3, id_fid);
	localBalance = (*env)->GetIntField(env, snapshot3, localBalance_fid);
	moneyInTransfer = (*env)->GetIntField(env, snapshot3, moneyInTransfer_fid);

	snapshot3_C = malloc(sizeof(*snapshot3_C));
	snapshot3_C->id = id;
	snapshot3_C->localBalance = localBalance;
	snapshot3_C->moneyInTransfer = moneyInTransfer;

	printf("%d %d %d\n",(int) snapshot3_C->id, (int) snapshot3_C->localBalance, (int) snapshot3_C->moneyInTransfer);

}

void freeStructs()
{
	free(snapshot1_C);
	free(snapshot2_C);
	free(snapshot3_C);
}

int main()
{

	options[0].optionString = "-Djava.class.path=./";

	memset(&vm_args, 0, sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_8;
	vm_args.nOptions = 1;
	vm_args.options = options;
	status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);


	if (status != JNI_ERR)
	{
		cls = (*env)->FindClass(env, "Snapshot");
		assert(cls !=0);

		mid = (*env)->GetStaticMethodID(env, cls, "testJNI", "()V");
		assert (mid != 0);
		printit = (*env)->CallStaticBooleanMethod(env, cls, mid);

		constructor = (*env)->GetMethodID(env, cls, "<init>", "(III)V");
		assert(constructor != 0);

		MakeSnapshotObject();
		GetFieldIDs();
		PopulateCstructs();

		// now the snapshots are stored in snapshot1_C, snapshot2_C, snapshot3_C

		(*jvm)->DestroyJavaVM(jvm);
		freeStructs();
		return 0;
	}

	return -1;
}
