#include <string.h>
#include <jni.h>
#include <assert.h>
#include <stdlib.h>
#include "MakeFakeSnapshot.h"

#include "atlas_alloc.h"
#include "atlas_api.h"


uint32_t global_rgn_id;
Snapshot ** snapshotArr; 

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

	snapshot1_C = (Snapshot *) nvm_alloc(sizeof(Snapshot), global_rgn_id);
	//snapshot1_C = malloc(sizeof(*snapshot1_C));
	snapshot1_C->id = id;
	snapshot1_C->localBalance = localBalance;
	snapshot1_C->moneyInTransfer = moneyInTransfer;

	printf("%d %d %d\n",(int) snapshot1_C->id, (int) snapshot1_C->localBalance, (int) snapshot1_C->moneyInTransfer);

	id = (*env)->GetIntField(env, snapshot2, id_fid);
	localBalance = (*env)->GetIntField(env, snapshot2, localBalance_fid);
	moneyInTransfer = (*env)->GetIntField(env, snapshot2, moneyInTransfer_fid);

	snapshot2_C = (Snapshot *) nvm_alloc(sizeof(Snapshot), global_rgn_id);
	//snapshot2_C = malloc(sizeof(*snapshot2_C));
	snapshot2_C->id = id;
	snapshot2_C->localBalance = localBalance;
	snapshot2_C->moneyInTransfer = moneyInTransfer;

	printf("%d %d %d\n",(int) snapshot2_C->id, (int) snapshot2_C->localBalance, (int) snapshot2_C->moneyInTransfer);

	id = (*env)->GetIntField(env, snapshot3, id_fid);
	localBalance = (*env)->GetIntField(env, snapshot3, localBalance_fid);
	moneyInTransfer = (*env)->GetIntField(env, snapshot3, moneyInTransfer_fid);

	snapshot3_C = (Snapshot *) nvm_alloc(sizeof(Snapshot), global_rgn_id);
	//snapshot3_C = malloc(sizeof(*snapshot3_C));
	snapshot3_C->id = id;
	snapshot3_C->localBalance = localBalance;
	snapshot3_C->moneyInTransfer = moneyInTransfer;

	printf("%d %d %d\n",(int) snapshot3_C->id, (int) snapshot3_C->localBalance, (int) snapshot3_C->moneyInTransfer);
	
	snapshotArr[0] = snapshot1_C;
	snapshotArr[1] = snapshot2_C;
	snapshotArr[2] = snapshot3_C;
}

void freeStructs()
{	
	printf("Free snapshot 1 \n");
	nvm_free(snapshot1_C);
	printf("Free snapshot 2 \n");
	nvm_free(snapshot2_C);
	printf("Free snapshot 3 \n");
	nvm_free(snapshot3_C);
}

int initialize(){

	void *rgn_root = NVM_GetRegionRoot(global_rgn_id);
	if(rgn_root){
		snapshotArr = (Snapshot **) rgn_root;
		return 1;
	}else{
		snapshotArr = (Snapshot **) nvm_alloc(3*sizeof(Snapshot*),  global_rgn_id);
		return 0;
	}
}

void print_snapshots(){

	for (int i =0; i<3; i++){
	Snapshot * sn = snapshotArr[i];
		printf("Id number %d \n", (int) sn->id);
	}
}

int main()
{

	options[0].optionString = "-Djava.class.path=/home/mb/Documents/Atlas/runtime/tests/data_structures/";

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
	
		NVM_Initialize();
		global_rgn_id = NVM_FindOrCreateRegion("queue", O_RDWR, NULL);		
	
		int status = initialize();
		if (status){
			printf("Root region found \n");
			
			
			//printf("Recovered ID %d \n", (int)snapshot1_C->id);
		}
		else{
			printf("No prior root found \n");
			PopulateCstructs();
		}
		NVM_SetRegionRoot(global_rgn_id, snapshotArr);
		print_snapshots();

		// now the snapshots are stored in snapshot1_C, snapshot2_C, snapshot3_C
		printf("Destroying JVM \n");
		(*jvm)->DestroyJavaVM(jvm);
		printf("Freeing Structs\n");
		freeStructs();
		
		NVM_CloseRegion(global_rgn_id);
		NVM_Finalize();
		return 0;
	}

	return -1;
}
