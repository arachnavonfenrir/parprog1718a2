// simple quicksort implementation
// compile with: gcc -O2 -Wall quicksort.c -o quicksort -DLIMIT=10.000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


#define N 1000000
#define CUTOFF 10




void inssort(double *a,int n) {
	int i,j;
	double t;
  
	for (i=1;i<n;i++) {
    	j = i;
    	while ((j>0) && (a[j-1]>a[j])) {
	    	t = a[j-1];  a[j-1] = a[j];  a[j] = t;
	        j--;
    	}
  }

}


int partition(double *a,int n){
// take first, last and middle positions
	int i,j; //counters  
	int first,middle,last;
	double t,p;

	first = 0;
	middle = n-1;
	last = n/2;  
  
	// put median-of-3 in the middle
	if (a[middle]<a[first]) { t = a[middle]; a[middle] = a[first]; a[first] = t; }
	if (a[last]<a[middle]) { t = a[last]; a[last] = a[middle]; a[middle] = t; }
	if (a[middle]<a[first]) { t = a[middle]; a[middle] = a[first]; a[first] = t; }
    
	// partition (first and last are already in correct half)
	p = a[middle]; // pivot
	for (i=1,j=n-2;;i++,j--) {
    while (a[i]<p) i++;
    while (p<a[j]) j--;
    if (i>=j) break;

    t = a[i]; a[i] = a[j]; a[j] = t;      
  }
return i;
}

void quicksort(double *a,int n) {


int i;

  // check if below cutoff limit
	if (n<=CUTOFF) {
    inssort(a,n);
    return;
  }
  
	i=partition(a,n);
   
  // recursively sort halves
	quicksort(a,i);
	quicksort(a+i,n-i);
  
}

struct thread_params{
	int n;
	double *a;
};

void *work(void *tp){

	
	
	double *a;
	int n,i;
	struct thread_params *tparm;
	tparm=(struct thread_params *)tp;
	
	printf("hello\n");
	
		
	a=tparm->a;
	n=tparm->n;
	if(n<=LIMIT){
		quicksort(a,n);
	}
	else{
		i=partition(a,n);
		//dimiourgw 2 paidia(threads)
		pthread_t tid1,tid2;
		struct thread_params tp1,tp2;
		
		tp1.a=a;
		tp1.n=i;
		pthread_create(&tid1,NULL,work,&tp1);
		tp2.a=a+1;
		tp2.n=n-i;
		pthread_create(&tid2,NULL,work,&tp2);
		//kanw join ta paidia
		pthread_join(tid1,NULL);
		pthread_join(tid2,NULL);
		
		
	}
	pthread_exit(NULL);


}

int main() {
	double *a;

	int i;
	
	pthread_t tid;
	struct thread_params tp;
	

 
	a = (double *)malloc(N*sizeof(double));
	if (a==NULL) {
	    printf("error in malloc\n");
	    exit(1);
  }

  // fill array with random numbers
	srand(time(NULL));
	for (i=0;i<N;i++) {
		a[i] = (double)rand()/RAND_MAX;
	}

  // simple quicksort
	quicksort(a,N);

	tp.a=a;
	tp.n=N;
	pthread_create(&tid,NULL,work,&tp);
	pthread_join(tid,NULL);

  // check sorting
	for (i=0;i<(N-1);i++) {
		if (a[i]>a[i+1]) {
			printf("Sort failed!\n");
	        break;
    	}
   }  

  free(a);
  
  return 0;
}