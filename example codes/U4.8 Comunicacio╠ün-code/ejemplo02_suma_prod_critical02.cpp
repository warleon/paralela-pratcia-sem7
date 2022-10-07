#include<iostream>
#include<omp.h>
using namespace std;

#define ARR_SIZE 1<<16
#define MAX_ITER 1000
int main(){

int i, sum=0, prod=1;
double a[ARR_SIZE],t1, t2;


for(int i=0;i<ARR_SIZE;i++) a[i]=i; 

t1=omp_get_wtime();
//for(int i=0;i<MAX_ITER;i++){
#pragma omp parallel default(shared) num_threads(4)
{
	int psum=0, pprod=1;
	#pragma omp for
	for(int i=0;i<ARR_SIZE;i++) {
		psum+=a[i];
		pprod*=a[i];
	}
	#pragma omp critical (section1)
	sum+=psum;
	
	#pragma omp critical (section2)
	prod*=pprod;
}
//}
t2=omp_get_wtime();
printf("tiempo: %f8",(t2-t1)/MAX_ITER);
}
