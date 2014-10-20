#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<climits>
#include<ctime>
using namespace std;


#define LOWER_BOUND -1
#define UPPER_BOUND 1
#define num_samples 100 //number of samples we want to generate in every iteration, our population count
#define num_update 5    //number of top members of population we are taking to modify our distribution
#define max_iter 500	//total number of iterations
#define problem_size 3	// total number of variables in our equation
#define m 2	// number of equaitons
#define n 3	// same as problem_size
#define alpha 0.7 //learning rate

struct sample{
	double arr[problem_size];
	double cost;
};

typedef struct sample sample;



double a[m][n] , b[m];
sample best;
sample samples[num_samples];
sample selected[num_update];
double means[problem_size];
double stdev[problem_size];



/* the following function takes the input matrices A and B */
int take_input()
{
	srand(time(NULL));
	
	for(int i =0; i<  m; i++)
		for(int j =0; j<n; j++)
			scanf("%lf",&a[i][j]);


	for(int  i =0; i< m; i++)
		scanf("%lf",&b[i]);

}



/* simple comparision function to compare cost */	
bool cmp( sample   a , sample  b )
{
	if( a.cost < b.cost )
		return true;
	else
		return false;

}


/* find average of means of best part of population */
double mean_attr(int x )
{
	double sum = 0.0;
	for(int i=0; i< num_update; i++)
		sum += selected[i].arr[x];
	return sum / num_update;
}


/* find the average standard deviation for best part of population */
double stdev_attr(int x,double mean)
{
	double sum = 0.0;
	
	for(int i =0; i<num_update; i++)
		sum += (selected[i].arr[x] - mean)*(selected[i].arr[x] - mean);
	return sqrt( sum / num_update);
}

/* returns a random number between a to b  */
double random_variable(double a ,double b){
	
	return  a + (( b - a )*((double)rand()/RAND_MAX) );
}



/* returns a value depending on mean and stdev according to gaussian distribution fucntion */
double random_gaussian(double mean , double stdev)
{
	double u1,u2,w;
	do {
		u1 = 2*((double)rand()/RAND_MAX) - 1;
		u2 = 2*((double)rand()/RAND_MAX) - 1;
		w = u1*u1 + u2 * u2;
	}	while( w >= 1 );

	w = sqrt(( -2.0 * log(w ))/w);
	return mean + ( u2* w ) * stdev;
}



/* evaluate our samples , returns the cost of our solution */
double evaluate(int x)
{

	double sum = 0.0;

	double delta_array[m];

	
	
	for( int i =0; i< m; i++)
	{

		delta_array[i] = 0.0;
		for(int j =0; j<n; j++)
			delta_array[i] = ( delta_array[i] + ( (a[i][j]) *samples[x].arr[j] ));
		delta_array[i] =delta_array[i] - b[i];
		sum += delta_array[i] * delta_array[i];

	}

	return sum;
}




int main()
{

	
	take_input();

		
	best.cost = -1; // intially we don't have any best , so its initial value is -1
	


	for(int i =0; i< problem_size; i++){
		 means[i] = random_variable(LOWER_BOUND,UPPER_BOUND);
		 stdev[i] = UPPER_BOUND - LOWER_BOUND;
	}
	
	
		
	for(int iter =0; iter < max_iter ; iter++ )
	{

		

		for(int  i =0; i< num_samples; i++)
		{

			for(int j=0; j< problem_size; j++)
			{

				samples[i].arr[j] = random_gaussian(means[j],stdev[j]);
				
				if( samples[i].arr[j] < LOWER_BOUND ) samples[i].arr[j] = LOWER_BOUND;
				if( samples[i].arr[j] > UPPER_BOUND ) samples[i].arr[j]  = UPPER_BOUND;
			}
			samples[i].cost = evaluate(i);
		}

		

		sort( samples,samples+num_samples,cmp);



		if( best.cost == -1  || best.cost > samples[0].cost )
		{
			best.cost = samples[0].cost;
				for(int i=0; i<problem_size; i++)
					best.arr[i] = samples[0].arr[i];
		}
	



		for(int j=0; j< num_update;j++){
			selected[j].cost = samples[j].cost;
			for(int k =0; k< problem_size; k++)
				selected[j].arr[k] = samples[j].arr[k];
		}
		
				


		
		for(int j=0; j< problem_size; j++)
		{
			means[j] = alpha*means[j] + ( 1.0 - alpha) * mean_attr(j);
			stdev[j] = alpha*stdev[j] + ( 1.0 - alpha) * stdev_attr(j,means[j]);
		}

		
		printf(" > iteration = %d , fitness = %lf\n",iter,best.cost );
		fflush(stdout);

	}
	
				
	
	printf("Solution : f = %lf\n",best.cost );
	for(int i =0; i< problem_size; i++)
		printf("%f ",best.arr[i]);

	printf("\n");



	return 0;
}
