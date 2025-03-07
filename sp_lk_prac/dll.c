
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int x[2] = {1,2};
int y[2] = {3,4};
int z[2];
int main(){
	void* handle;
	void (*addvec)(int*, int*, int*, int);
	char* error;

	handle = dlopen("./libvector.so", RTLD_LAZY);
	if(!handle){
		fprintf(stderr,"%s\n", dlerror());
		exit(1);
	}

	addvec = (void(*)(int*,int*,int*,int))dlsym(handle, "addvec");
	if((error = dlerror()) != NULL){
		fprintf(stderr,"%s\n", error);
		exit(1);
	}
	addvec(x,y,z,2);
	printf("z = [%d %d]\n", z[0],z[1]);

	// unload the shared libary
	if(dlclose(handle) < 0){
		fprintf(stderr,"%s\n",dlerror());
		exit(1);
	}
	return 0;
}
